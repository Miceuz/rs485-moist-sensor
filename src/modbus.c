/************************************************************************
Title:    Yet another (small) Modbus (server) implementation for the avr.
Author:   Max Brueggemann, Albertas Mickenas
Hardware: AVR ATtiny441 at 16MHz
License:  GNU General Public License

LICENSE:
    Copyright (C) 2017 Max Brueggemann, Albertas Mickenas

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as 
	published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
************************************************************************/

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "modbus.h"

#define UART_TRANSMIT_COMPLETE_INTERRUPT USART0_TX_vect
#define UART_RECEIVE_INTERRUPT   USART0_RX_vect
#define UART_TRANSMIT_INTERRUPT  USART0_UDRE_vect
#define UART_STATUS   UCSR0A
#define UART_CONTROL  UCSR0B
#define UART_DATA     UDR0
#define UART_UDRIE    UDRIE0
#define UCSRC UCSR0C
#define RXCIE RXCIE0
#define TXCIE TXCIE0
#define RXEN RXEN0
#define TXEN TXEN0
#define UCSZ0 UCSZ00
#define U2X U2X0
#define UBRRH UBRR0H
#define UBRRL UBRR0L

#define ADDRESS_BROADCAST 0

typedef enum{
	STATE_INITIAL,
	STATE_IDLE,
	STATE_RECEPTION,
	STATE_CONTROL,
	STATE_EMISSION
} BusState_t;

#define maxFrameIndex 31
volatile uint8_t sendPacketTopIndex = 0;
volatile uint8_t rxbuffer[32];
uint16_t timeout15 = 0;
uint16_t timeout35 = 0;
volatile BusState_t busState;
volatile bool frameNOK = false;
volatile bool frameAvailable = false;
uint8_t ownAddress = 0;
volatile uint8_t dataPos = 0;


/* @brief: A fairly simple Modbus compliant 16 Bit CRC algorithm.
*
*  	Returns 1 if the crc check is positive, returns 0 and saves the calculated CRC bytes
*	at the end of the data array if it fails.
*  	
*/
uint8_t crc16(volatile uint8_t *ptrToArray,uint8_t inputSize) //A standard CRC algorithm
{
	uint16_t out=0xffff;
	uint16_t carry;
	unsigned char n;
	inputSize++;
	for (int l=0; l<inputSize; l++) {
		out ^= ptrToArray[l];
		for (n = 0; n < 8; n++) {
			carry = out & 1;
			out >>= 1;
			if (carry) out ^= 0xA001;
		}
	}
	//out=0x1234;
	if ((ptrToArray[inputSize]==out%256) && (ptrToArray[inputSize+1]==out/256)) //check
	{
		return 1;
	} else { 
		ptrToArray[inputSize]=out%256; //append Lo
		ptrToArray[inputSize+1]=out/256; //append Hi
		return 0;	
	}
}

void start35Timer() {
    TCNT2 = 0;
    OCR2B = timeout35;
    TIMSK2 |= _BV(OCIE2B);
    TIFR2 |= _BV(OCF2B);
    TCCR2B = _BV(CS21);
}

void start15Timer() {
    TCNT2 = 0;
    OCR2A = timeout15;
    TIMSK2 |= _BV(OCIE2A);
    TIFR2 |= _BV(OCF2A);
    TCCR2B = _BV(CS21);
}

static inline void stop15Timer() {
    TIMSK2 &= ~_BV(OCIE2A);
}

static inline bool isTimer15Off() {
	return 0 == (TIMSK2 & _BV(OCIE2A));
}

static inline void stop35Timer() {
    TIMSK2 &= ~_BV(OCIE2B);
    if(isTimer15Off()) {
    	TCCR2B = 0;
    }
}

inline static void initFrame() {
	dataPos = 0;
	frameNOK = false;
	frameAvailable = false;
}

void modbusSetAddress(uint8_t address) {
	ownAddress = address;
}

inline static bool isFrameAddressedToUs() {
	return ownAddress == rxbuffer[0] || ADDRESS_BROADCAST == rxbuffer[0];
}

inline static bool isFunction(const uint8_t fCode) {
	return rxbuffer[1] == fCode;
}

/* @brief:  Returns the amount of requested data objects (coils, discretes, registers)
*
*/
inline static uint8_t getRequestedRegisterCount(void) {
	if (rxbuffer[1]==fcPresetSingleRegister) {
		return 1;
	} else {
		return (uint8_t)(rxbuffer[5]|(rxbuffer[4]<<8));
	}
}

/* @brief: Returns the address of the first requested data object (coils, discretes, registers)
*
*/
inline static uint8_t getRequestedAddress(void) {
	return (uint8_t)(rxbuffer[3]|(rxbuffer[2]<<8));
}

bool isBroadcastFrame() {
	return ADDRESS_BROADCAST == rxbuffer[0];
}

bool validateFrame() {
	uint8_t requestedAmount = getRequestedRegisterCount();
	uint8_t requestedAdr = getRequestedAddress();
	if (isFunction(fcReadHoldingRegisters) || isFunction(fcReadInputRegisters)) {
		if(isBroadcastFrame()){
			return false;
		}
		//requested amount of bytes must fit into buffer together with address, function code, and 2 bytes of crc 
		if((requestedAmount * 2) > (maxFrameIndex - 4)) { 
			modbusSendException(ecIllegalDataAddress);
			return false;
		}
		return true;
	} else if (isFunction(fcPresetMultipleRegisters)) {
		if (rxbuffer[6] < requestedAmount * 2) {//enough data declared in the frame?
			modbusSendException(ecIllegalDataValue);
			return false;
		}
		if ((dataPos - 9) < rxbuffer[6]) {//enough data received?
			modbusSendException(ecIllegalDataValue);
			return false;
		}
		return true;
	} else if (isFunction(fcPresetSingleRegister)) {
		if((dataPos - 6) < 2) {//enough data received? should be exactly 2 bytes, but we allow more, will use just the first two
			modbusSendException(ecIllegalDataValue);
		}
	} else {
		modbusSendException(ecIllegalFunction);
		return false;
	}
}

/**
 * @brief	Character reception interrupt. Code in this interrupt handling routine plus two 
 * 			subsequent timer interrupt routines implement a RTU transmission mode state 
 * 			diagram as per "MODBUS over serial line specification and implementation 
 * 			guide V1.02" found at www.modbus.org
 */

ISR(UART_RECEIVE_INTERRUPT) {
	unsigned char data;
	uint8_t parityError = (UART_STATUS &_BV(UPE0));
	data = UART_DATA;
	switch(busState){
		case STATE_INITIAL:
			start35Timer();
			break;
		case STATE_IDLE:
			busState = STATE_RECEPTION;
			initFrame();
			rxbuffer[dataPos] = data;
			dataPos++;
			start35Timer();
			start15Timer();
			break;
		case STATE_RECEPTION:
			if(dataPos <= maxFrameIndex) {
				rxbuffer[dataPos] = data;
				dataPos++;
			} else {
				frameNOK = true;
			}
			start35Timer();
			start15Timer();
			break;
		case STATE_CONTROL:
			start35Timer();
			frameNOK = true;
			break;
		case STATE_EMISSION:
			start35Timer();
			frameNOK = true;			
	}

	// if(parityError) {
	// 	    PINA |= _BV(PA3);
	// 	frameNOK = true;
	// }
}

/**
 * @brief	1.5 char timer interrupt
 */
ISR(TIMER2_COMPA_vect) {
	stop15Timer();
	if(STATE_RECEPTION == busState) {
		busState = STATE_CONTROL;
	}
}

/**
 * @brief	3.5 char timer interrupt
 */
ISR(TIMER2_COMPB_vect) {
	stop35Timer();
	if(STATE_INITIAL == busState) {
		busState = STATE_IDLE;
	} else if (STATE_CONTROL == busState) {
		busState = STATE_IDLE;
		if(!frameNOK && isFrameAddressedToUs() && crc16(rxbuffer, dataPos - 3)) {
			if(validateFrame()) {
				frameAvailable = true;
			}
		}
	} else if (STATE_EMISSION == busState) {
		busState = STATE_IDLE;
	}
}

/* @brief: Sends a response.
*
*         Arguments: - packtop: Position of the last byte containing data.
*                               modbusSendException is a good usage example.
*/
void modbusSendMessage(unsigned char packtop) {
	if(isBroadcastFrame()) {
		return;
	}

	sendPacketTopIndex = packtop+2;
	crc16(rxbuffer,packtop);
	dataPos = 0;

	transceiver_txen();
	busState = STATE_EMISSION;
	frameAvailable = false;
	UART_CONTROL|=(1<<UART_UDRIE);
}

ISR(UART_TRANSMIT_INTERRUPT) {
	UART_DATA=rxbuffer[dataPos];
	dataPos++;
	if (dataPos == (sendPacketTopIndex+1)) {
		UART_CONTROL&=~(1<<UART_UDRIE);
	}
}

ISR(UART_TRANSMIT_COMPLETE_INTERRUPT) {
	transceiver_rxen();
	start35Timer();
}

/*calculated for 16MHz crystal on attiny441 */
uint16_t bauds[] = {1665, 832, 415, 207, 103, 51, 33, 16};
uint8_t parityModes[] = {0, 2, 3};
//for 16MHz 16bit timer running with prescaler /8
uint16_t timeouts15[] = {27500, 13750, 6875, 3437, 1719, 1500, 1500, 1500};
uint16_t timeouts35[] = {64167, 32084, 16042, 8021, 4011, 3500, 3500, 3500};

void modbusInit(uint8_t baudIdx, uint8_t parityIdx) {
	timeout15 = timeouts15[baudIdx];
	timeout35 = timeouts35[baudIdx];
	UBRRH = (unsigned char)(bauds[baudIdx]>>8);
	UBRRL = (unsigned char)bauds[baudIdx];
	UART_CONTROL = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN); // USART receiver and transmitter and receive complete interrupt
	UART_STATUS |= (1<<U2X); //double speed mode. 
	UCSRC = (3<<UCSZ0) | (parityModes[parityIdx] << UPM00); //Frame Size
	
	if(0 == parityIdx) {
		UCSRC |= _BV(USBS0); //if there is no parity, use two stop bits as per Modbus over serial spec v1.02
	}
	
	UART_CONTROL|=(1<<TXCIE); //Transmit Complete Interrupt an
	transceiver_rxen();

	busState = STATE_INITIAL;
	start35Timer();
}

bool modbusIsFrameAvailable() {
	return frameAvailable;
}

uint8_t modbusGetRegisterNumber(void) {
	return rxbuffer[3];
}

uint8_t modbusGetFunctionCode(void) {
	return rxbuffer[1];
}

/* @brief: Sends an exception response.
*
*         Arguments: - exceptionCode
*                              
*/
void modbusSendException(unsigned char exceptionCode)
{
	rxbuffer[1]|=(1<<7); //setting MSB of the function code (the exception flag)
	rxbuffer[2]=exceptionCode; //Exceptioncode. Also the last byte containing data
	modbusSendMessage(2);
}

/* @brief: copies a single or multiple bytes from one array of bytes to an array of 16-bit-words
*
*/
void intToModbusRegister(volatile uint16_t *inreg, volatile uint8_t *outreg, uint8_t amount)
{
	for (uint8_t c=0; c<amount; c++) {
			*(outreg+c*2) = (uint8_t)(*(inreg+c) >> 8);
			*(outreg+1+c*2) = (uint8_t)(*(inreg+c));
	}
}

/* @brief: copies a single or multiple 16-bit-words from one array of integers to an array of bytes
*
*/
void modbusRegisterToInt(volatile uint8_t *inreg, volatile uint16_t *outreg, uint8_t amount)
{
	for (uint8_t c=0; c<amount; c++) {
		*(outreg+c) = (*(inreg+c*2) << 8) + *(inreg+1+c*2);
	}
}


/* @brief: Handles single/multiple register reading and single/multiple register writing.
*
*         Arguments: - ptrToInArray: pointer to the user's data array containing registers
*                    - startAddress: address of the first register in the supplied array
*                    - size: input array size in the requested format (16bit-registers)
*
*/
uint8_t modbusExchangeRegisters(volatile uint16_t *ptrToInArray, uint16_t size) {
	uint8_t requestedAmount = getRequestedRegisterCount();
	uint8_t requestedAdr = getRequestedAddress();

	if ((requestedAmount + requestedAdr) <= size) {
		if (isFunction(fcReadHoldingRegisters) || isFunction(fcReadInputRegisters)){
			rxbuffer[2] = (uint8_t) (requestedAmount * 2); //payload size
			intToModbusRegister(ptrToInArray + requestedAdr, rxbuffer + 3, rxbuffer[2]);
			modbusSendMessage(2 + rxbuffer[2]);
			return 1;
		} else if (isFunction(fcPresetMultipleRegisters)) {
			modbusRegisterToInt(rxbuffer + 7, ptrToInArray + requestedAdr, requestedAmount);
			modbusSendMessage(5);
			return 1;
		} else if (isFunction(fcPresetSingleRegister)) {
			modbusRegisterToInt(rxbuffer + 4, ptrToInArray + requestedAdr, 1);
			modbusSendMessage(5);
			return 1;
		} 

		//modbusSendException(ecSlaveDeviceFailure); //inapropriate call of modbusExchangeRegisters
		return 0;
	} else {
		modbusSendException(ecIllegalDataValue);
		return 0;
	}
}

bool modbusIsIdle() {
	return STATE_IDLE == busState;
}