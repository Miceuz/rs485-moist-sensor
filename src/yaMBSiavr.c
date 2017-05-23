/*************************************************************************
Title:    Yet another (small) modbus implementation for the avr.
Author:   Max Brueggemann
Hardware: any AVR with hardware UART, tested on Atmega 88/168 at 20Mhz
License:  GNU General Public License 
          
DESCRIPTION:
    Refer to the header file yaMBSiavr.h.
    
USAGE:
    Refer to the header file yaMBSiavr.h.
                    
LICENSE:
    Copyright (C) 2016 Max Brueggemann

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
                        
*************************************************************************/

#include <avr/io.h>
#include "yaMBSiavr.h"
#include <avr/interrupt.h>

volatile unsigned char BusState = 0;
volatile uint16_t modbusTimer = 0;
volatile unsigned char rxbuffer[MaxFrameIndex+1];
volatile uint16_t DataPos = 0;
volatile unsigned char PacketTopIndex = 7;
volatile unsigned char modBusStaMaStates = 0;

uint8_t modbusGetBusState(void)
{
	return BusState;
}

#if ADDRESS_MODE == SINGLE_ADR
volatile unsigned char Address = 0x00;
uint8_t modbusGetAddress(void)
{
	return Address;
}

void modbusSetAddress(unsigned char newadr)
{
	Address = newadr;
}
#endif

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

/* @brief: copies a single or multiple words from one array of bytes to another array of bytes
*          amount must not be bigger than 255...
*
*/
void listRegisterCopy(volatile uint8_t *source, volatile uint8_t *target, uint8_t amount)
{
	for (uint8_t c=0; c<amount; c++)
	{
		*(target+c)=*(source+c);
	}
}

/* @brief: copies a single bit from one char to another char (or arrays thereof)
*
*
*/
void listBitCopy(volatile uint8_t *source, uint16_t sourceNr,volatile uint8_t *target, uint16_t targetNr)
{
	if(*(source+(sourceNr/8))&(1<<(sourceNr-((sourceNr/8)*8))))
	{
		*(target+(targetNr/8))|=(1<<(targetNr-((targetNr/8)*8)));
	} else *(target+(targetNr/8))&=~(1<<(targetNr-((targetNr/8)*8)));
}

/* @brief: Back to receiving state.
*
*/
void modbusReset(void)
{
	BusState=(1<<TimerActive); //stop receiving (error)
	modbusTimer=0;
}

void modbusTickTimer(void)
{
	if (BusState&(1<<TimerActive)) 
	{
		modbusTimer++;
		if (BusState&(1<<Receiving)) //we are in receiving mode
		{
			if ((modbusTimer==modbusInterCharTimeout)) {
				BusState|=(1<<GapDetected);
			} else if ((modbusTimer==modbusInterFrameDelayReceiveEnd)) { //end of message
				BusState=(1<<ReceiveCompleted);
				#if ADDRESS_MODE == MULTIPLE_ADR
               		 if (crc16(rxbuffer,DataPos-3)) { //perform crc check only. This is for multiple/all address mode.
                	} else modbusReset();
				#endif
				#if ADDRESS_MODE == SINGLE_ADR
				if (rxbuffer[0]==Address && crc16(rxbuffer,DataPos-3)) { //is the message for us? => perform crc check
				} else modbusReset();
				#endif
			}	
		} else if (modbusTimer==modbusInterFrameDelayReceiveStart) BusState|=(1<<BusTimedOut);
	}
}

ISR(UART_RECEIVE_INTERRUPT)
{
	unsigned char data;
	data = UART_DATA;
	modbusTimer=0; //reset timer


	if (!(BusState & (1<<ReceiveCompleted)) && !(BusState & (1<<TransmitRequested)) && !(BusState & (1<<Transmitting)) && (BusState & (1<<Receiving)) && !(BusState & (1<<BusTimedOut))) {
		if (DataPos>MaxFrameIndex) {
			modbusReset();	
		} else {
			rxbuffer[DataPos]=data;
			if(DataPos+1 < MaxFrameIndex) {
				DataPos++;
			}
		}	    
    } else if (!(BusState & (1<<ReceiveCompleted)) && !(BusState & (1<<TransmitRequested)) && !(BusState & (1<<Transmitting)) && !(BusState & (1<<Receiving)) && (BusState & (1<<BusTimedOut))) { 
		 rxbuffer[0]=data;
		 BusState=((1<<Receiving)|(1<<TimerActive));
		 DataPos=1;
   	}
}

ISR(UART_TRANSMIT_INTERRUPT)
{
	BusState&=~(1<<TransmitRequested);
	BusState|=(1<<Transmitting);
	UART_DATA=rxbuffer[DataPos];
	DataPos++;
	if (DataPos==(PacketTopIndex+1)) {
		UART_CONTROL&=~(1<<UART_UDRIE);
	}
}

ISR(UART_TRANSMIT_COMPLETE_INTERRUPT)
{
	#if PHYSICAL_TYPE == 485
	transceiver_rxen();
	#endif
	modbusReset();
}

/*calculates for 16MHz crystal on attiny441 */
uint16_t bauds[] = {1665, 832, 415, 207, 103, 51, 33, 16};
uint8_t parityModes[] = {0, 2, 3};

void modbusInit(uint8_t baudIdx, uint8_t parityIdx) {
	UBRRH = (unsigned char)(bauds[baudIdx]>>8);
	UBRRL = (unsigned char)bauds[baudIdx];
	UART_CONTROL = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN); // USART receiver and transmitter and receive complete interrupt
	UART_STATUS |= (1<<U2X); //double speed mode. 
	UCSRC = (3<<UCSZ0) | (parityModes[parityIdx] << UPM00); //Frame Size
	
	if(0 == parityIdx) {
		UCSRC |= _BV(USBS0); //if there is no parity, use two stop bits as per Modbus over serial spec v1.02
	}
	
	UART_CONTROL|=(1<<TXCIE); //Transmit Complete Interrupt an
	#if PHYSICAL_TYPE == 485
	transceiver_rxen();
	#endif
	BusState=(1<<TimerActive);
}

/* @brief: Sends a response.
*
*         Arguments: - packtop: Position of the last byte containing data.
*                               modbusSendException is a good usage example.
*/
void modbusSendMessage(unsigned char packtop)
{
	PacketTopIndex=packtop+2;
	crc16(rxbuffer,packtop);
	BusState|=(1<<TransmitRequested);
	DataPos=0;
	#if PHYSICAL_TYPE == 485
	transceiver_txen();
	#endif
	UART_CONTROL|=(1<<UART_UDRIE);
	BusState&=~(1<<ReceiveCompleted);
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


/* @brief:  Returns the amount of requested data objects (coils, discretes, registers)
*
*/
inline static uint16_t modbusRequestedAmount(void) {
	if (rxbuffer[1]==fcPresetSingleRegister) {
		return 1;
	} else {
		return (rxbuffer[5]|(rxbuffer[4]<<8));
	}
}

/* @brief: Returns the address of the first requested data object (coils, discretes, registers)
*
*/
inline static uint16_t modbusRequestedAddress(void)
{
	return (rxbuffer[3]|(rxbuffer[2]<<8));
}

/* @brief: copies a single or multiple bytes from one array of bytes to an array of 16-bit-words
*
*/
void intToModbusRegister(volatile uint16_t *inreg, volatile uint8_t *outreg, uint8_t amount)
{
	for (uint8_t c=0; c<amount; c++)
	{
			*(outreg+c*2) = (uint8_t)(*(inreg+c) >> 8);
			*(outreg+1+c*2) = (uint8_t)(*(inreg+c));
	}
}

/* @brief: copies a single or multiple 16-bit-words from one array of integers to an array of bytes
*
*/
void modbusRegisterToInt(volatile uint8_t *inreg, volatile uint16_t *outreg, uint8_t amount)
{
	for (uint8_t c=0; c<amount; c++)
	{
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
uint8_t modbusExchangeRegisters(volatile uint16_t *ptrToInArray, uint16_t startAddress, uint16_t size)
{
	uint16_t requestedAmount = modbusRequestedAmount();
	uint16_t requestedAdr = modbusRequestedAddress();

	if ((requestedAdr>=startAddress) && ((startAddress+size)>=(requestedAmount+requestedAdr))) {
		if ((rxbuffer[1]==fcReadHoldingRegisters) || (rxbuffer[1]==fcReadInputRegisters) ){
			if ((requestedAmount*2)<=(MaxFrameIndex-4)) {//message buffer big enough?
				rxbuffer[2]=(unsigned char)(requestedAmount*2);
				intToModbusRegister(ptrToInArray+(unsigned char)(requestedAdr-startAddress),rxbuffer+3,rxbuffer[2]);
				modbusSendMessage(2+rxbuffer[2]);
				return 1;
			} else modbusSendException(ecIllegalDataValue);
		}
		else if (rxbuffer[1]==fcPresetMultipleRegisters) {
			if (((rxbuffer[6])>=requestedAmount*2) && ((DataPos-9)>=rxbuffer[6])) { //enough data received?
				modbusRegisterToInt(rxbuffer+7,ptrToInArray+(unsigned char)(requestedAdr-startAddress),(unsigned char)(requestedAmount));
				modbusSendMessage(5);
				return 1;
			} else modbusSendException(ecIllegalDataValue);//too few data bytes received
		}
		else if (rxbuffer[1]==fcPresetSingleRegister) {
			modbusRegisterToInt(rxbuffer+4,ptrToInArray+(unsigned char)(requestedAdr-startAddress),1);
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

/* @brief: Handles single/multiple input/coil reading and single/multiple coil writing.
*
*         Arguments: - ptrToInArray: pointer to the user's data array containing bits
*                    - startAddress: address of the first bit in the supplied array
*                    - size: input array size in the requested format (bits)
*
*/
uint8_t modbusExchangeBits(volatile uint8_t *ptrToInArray, uint16_t startAddress, uint16_t size)
{
	uint16_t requestedAmount = modbusRequestedAmount();
	uint16_t requestedAdr = modbusRequestedAddress();
	if (rxbuffer[1]==fcForceSingleCoil) requestedAmount=1;
	if ((requestedAdr>=startAddress) && ((startAddress+size)>=(requestedAmount+requestedAdr)))
	{
		if ((rxbuffer[1]==fcReadInputStatus) || (rxbuffer[1]==fcReadCoilStatus))
		{
			if (requestedAmount<=((MaxFrameIndex-4)*8)) //message buffer big enough?
			{
				rxbuffer[2]=(requestedAmount/8);
				if (requestedAmount%8>0)
				{
					rxbuffer[(uint8_t)(requestedAmount/8)+3]=0x00; //fill last data byte with zeros
					rxbuffer[2]++;
				}
				for (uint16_t c = 0; c<requestedAmount; c++)
				{
					listBitCopy(ptrToInArray,requestedAdr-startAddress+c,rxbuffer+3,c);
				}
				modbusSendMessage(rxbuffer[2]+2);
				return 1;
			} else modbusSendException(ecIllegalDataValue); //too many bits requested within single request
		}
		else if (rxbuffer[1]==fcForceMultipleCoils)
		{
			if (((rxbuffer[6]*8)>=requestedAmount) && ((DataPos-9)>=rxbuffer[6])) //enough data received?
			{
				for (uint16_t c = 0; c<requestedAmount; c++)
				{
					listBitCopy(rxbuffer+7,c,ptrToInArray,requestedAdr-startAddress+c);
				}
				modbusSendMessage(5);
				return 1;
			} else modbusSendException(ecIllegalDataValue);//exception too few data bytes received
		}
		else if (rxbuffer[1]==fcForceSingleCoil) {
			listBitCopy(rxbuffer+4,0,ptrToInArray,requestedAdr-startAddress);
			modbusSendMessage(5); 
			return 1;
		}
		//modbusSendException(ecSlaveDeviceFailure); //inanpropriate call of modbusExchangeBits
		return 0;
	} else
	{
		modbusSendException(ecIllegalDataValue);
		return 0;
	}
}

uint8_t modbusGetRegisterNumber(void) {
	return rxbuffer[3];
}

uint8_t modbusIsIdle() {
	return BusState == (1<<TimerActive);
}