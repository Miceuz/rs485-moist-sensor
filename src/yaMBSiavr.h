#ifndef yaMBIavr_H
#define yaMBIavr_H
#endif
/************************************************************************
Title:    Yet another (small) Modbus (server) implementation for the avr.
Author:   Max Brueggemann
Hardware: any AVR with hardware UART, tested on Atmega 88/168 at 20Mhz
License:  GNU General Public License

LICENSE:
    Copyright (C) 2016 Max Brueggemann

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as 
	published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
************************************************************************/

/** 
 *  @code #include <yaMBSIavr.h> @endcode
 * 
 *  @brief   Interrupt-based Modbus implementation for small avr microcontrollers.
 *           The Modbus implementation guidelines at modbus.org call for response
 *           timeouts in the range of several seconds , hence only timing critical
 *           parts have been implemented within ISRs. The actual handling of the Modbus
 *           frame can easily be done in the main while loop.
 *
 *  @author Max Brueggemann www.maxbrueggemann.de
 */

/*
* Definitions for transceiver enable pin.
*/
#define TRANSCEIVER_ENABLE_PORT PORTA
#define TRANSCEIVER_ENABLE_PIN 4
#define TRANSCEIVER_ENABLE_PORT_DDR DDRA

/**
 * @brief    
 *           At the moment the user has to set the value for Baudrate and
 *           speed mode manually. The values depend on the operating frequency 
 *           of your AVR and can be found in its datasheet.
 */
#if defined(__AVR_ATtiny2313__)
#define UART_TRANSMIT_COMPLETE_INTERRUPT USART_TX_vect
#define UART_RECEIVE_INTERRUPT   USART_RX_vect
#define UART_TRANSMIT_INTERRUPT  USART_UDRE_vect
#define UART_STATUS   UCSRA
#define UART_CONTROL  UCSRB
#define UART_DATA     UDR
#define UART_UDRIE    UDRIE
/*
 * Change this value if you are using a different frequency and/or
 * different baudrate.
*/
#define Baud 25 //38400@16e6Hz

#elif defined(__AVR_ATtiny441__)
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
/*
 * Change this value if you are using a different frequency and/or
 * different baudrate.
*/
#define Baud 103 //19200@16e6Hz

#elif defined(__AVR_ATmega164P__)
#define UART_TRANSMIT_COMPLETE_INTERRUPT USART1_TX_vect
#define UART_RECEIVE_INTERRUPT   USART1_RX_vect
#define UART_TRANSMIT_INTERRUPT  USART1_UDRE_vect
#define UART_STATUS   UCSR1A
#define UART_CONTROL  UCSR1B
#define UART_DATA     UDR1
#define UART_UDRIE    UDRIE1
#define UCSRC UCSR1C
#define RXCIE RXCIE1
#define TXCIE TXCIE1
#define RXEN RXEN1
#define TXEN TXEN1
#define UCSZ0 UCSZ10
#define U2X U2X1
#define UBRRH UBRR1H
#define UBRRL UBRR1L
/*
 * Change this value if you are using a different frequency and/or
 * different baudrate.
*/
#define Baud 64 //38400@20e6Hz

#elif defined(__AVR_ATmega168PA__)|(__AVR_ATmega88PA__)|(__AVR_ATmega328PA__)
#define UART_TRANSMIT_COMPLETE_INTERRUPT USART_TX_vect
#define UART_RECEIVE_INTERRUPT   USART_RX_vect
#define UART_TRANSMIT_INTERRUPT  USART_UDRE_vect
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
/*
 * Change this value if you are using a different frequency and/or
 * different baudrate.
*/
#define Baud 64 //38400@20e6Hz


#elif defined(__AVR_ATmega8__)|| defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega323__)
#define UART_TRANSMIT_COMPLETE_INTERRUPT USART_TXC_vect
#define UART_RECEIVE_INTERRUPT   USART_RXC_vect
#define UART_TRANSMIT_INTERRUPT  USART_UDRE_vect
#define UART_STATUS   UCSRA
#define UART_CONTROL  UCSRB
#define UART_DATA     UDR
#define UART_UDRIE    UDRIE

/*
 * Change this value if you are using a different frequency and/or
 * different baudrate.
*/
#define Baud 25 //38400@8e6Hz double speed mode


#else
#error "no definition available"
#endif

/*
 * Available address modes.
*/
#define MULTIPLE_ADR 2
#define SINGLE_ADR 1

/*
* Use SINGLE_ADR or MULTIPLE_ADR, default: SINGLE_ADR
* This is useful for building gateways, routers or clients that for whatever reason need multiple addresses.
*/
#define ADDRESS_MODE SINGLE_ADR

/*
* Use 485 or 232, default: 485
* Use 232 for testing purposes or very simple applications that do not require RS485 and bus topology.
*/
#define PHYSICAL_TYPE 485 //possible values: 485, 232

/*
#define modbusBaudrate 38400
#define modbusBlocksize 10
#define modbusBlockTime ((float)modbusBlocksize*1000000)/((float) modbusBaudrate) //is 260 für 38400
#define timerISROccurenceTime 102

#define TimeoutStartOfMessage  (uint16_t)(modbusBlockTime*3.5/(float)timerISROccurenceTime)
#define TimeoutEndOfMessage (uint16_t)(modbusBlockTime*4/(float)timerISROccurenceTime)
#define ReceiveMaxGap  (uint16_t)(modbusBlockTime*1.5/(float)timerISROccurenceTime)
*/
#define modbusInterFrameDelayReceiveStart 16
#define modbusInterFrameDelayReceiveEnd 18
#define modbusInterCharTimeout 7

/**
 * @brief    Defines the maximum Modbus frame size accepted by the device. 255 is the default
 *           and also the maximum value. However, it might be useful to set this to lower
 *           values, with 8 being the lowest possible value, in order to save on ram space.
 */
#define MaxFrameIndex 255

/**
 * @brief    Modbus Function Codes
 *           Refer to modbus.org for further information.
 *           It's good practice to return exception code 01 in case you receive a function code
 *           that you haven't implemented in your application.
 */
#define fcReadCoilStatus 1 //read single/multiple coils
#define fcReadInputStatus 2 //read single/multiple inputs
#define fcReadHoldingRegisters 3 //read analog output registers
#define fcReadInputRegisters 4 //read analog input registers (2 Bytes per register)
#define fcForceSingleCoil 5 //write single bit
#define fcPresetSingleRegister 6 //write analog output register (2 Bytes)
#define fcForceMultipleCoils 15 //write multiple bits
#define fcPresetMultipleRegisters 16 //write multiple analog output registers (2 Bytes each)
#define fcReportSlaveID 17 //read device description, run status and other device specific information

/**
 * @brief    Modbus Exception Codes
 *           Refer to modbus.org for further information.
 *           It's good practice to return exception code 01 in case you receive a function code
 *           that you haven't implemented in your application.
 */
#define ecIllegalFunction 1 
#define ecIllegalDataAddress 2 
#define ecIllegalDataValue 3
#define ecSlaveDeviceFailure 4
#define ecAcknowledge 5
#define ecSlaveDeviceBusy 6
#define ecNegativeAcknowledge 7
#define ecMemoryParityError 8

/**
 * @brief    Internal bit definitions
 */
#define BusTimedOut 0
#define Receiving 1
#define Transmitting 2
#define ReceiveCompleted 3
#define TransmitRequested 4
#define TimerActive 5
#define GapDetected 6

/**
* @brief    Configures the UART. Call this function only once.
*/
extern void modbusInit(void);

extern void transceiver_txen(void);
extern void transceiver_rxen(void);

/**
* @brief    receive/transmit data array
*/
volatile unsigned char rxbuffer[MaxFrameIndex+1];

/**
* @brief    Current receive/transmit position
*/
volatile uint16_t DataPos;

/**
 * This only applies to single address mode.
 */
#if ADDRESS_MODE == SINGLE_ADR
	/**
	* @brief: Read the device address   
	*/
	extern uint8_t modbusGetAddress(void);

	/**
	* @brief: Set the device address
	*         Arguments: - newadr: the new device address
	*/
	extern void modbusSetAddress(unsigned char newadr);
#endif

/* @brief: Sends a response.
*
*         Arguments: - packtop, index of the last byte in rxbuffer
*                      that contains payload. Maximum value is
*				       MaxFrameIndex-2.
*/
extern void modbusSendMessage(unsigned char packtop);

/* @brief: Sends a Modbus exception.
*
*         Arguments: - exceptionCode
*/
extern void modbusSendException(unsigned char exceptionCode);

/* @brief: Discards the current transaction. For MULTIPLE_ADR-mode and general
*		   testing purposes. Call this function if you don't want to reply at all.
*/
void modbusReset(void);

/**
 * @brief    Call this function whenever possible and check if its return value has the ReceiveCompleted Bit set.
 *           Preferably do this in the main while. I do not recommend calling this function within ISRs.
 * @example  if (modbusGetBusState() & (1<<ReceiveCompleted)) {
 *           modbusSendExcepton(ecIllegalFunction);
 *           }
 */
extern uint8_t modbusGetBusState(void);

/**
 * @brief    Call every 100µs using a timer ISR.
 */
extern void modbusTickTimer(void);

/**
 * @brief    Returns amount of bits/registers requested.
 */
//extern uint16_t modbusRequestedAmount(void);

/**
 * @brief    Returns the address of the first requested bit/register.
 */
//extern uint16_t modbusRequestedAddress(void);

/* A fairly simple and hopefully Modbus compliant 16 Bit CRC algorithm.
*  Returns 1 if the crc check is positive, returns 0 if it fails.
*  Appends two crc bytes to the array.
*/
extern uint8_t crc16(volatile uint8_t *ptrToArray,uint8_t inputSize);

/* @brief: Handles single/multiple input/coil reading and single/multiple coil writing.
*
*         Arguments: - ptrToInArray: pointer to the user's data array containing bits
*                    - startAddress: address of the first bit in the supplied array
*                    - size: input array size in the requested format (bits)
*
*/
extern uint8_t modbusExchangeBits(volatile uint8_t *ptrToInArray, uint16_t startAddress, uint16_t size);

/* @brief: Handles single/multiple register reading and single/multiple register writing.
*
*         Arguments: - ptrToInArray: pointer to the user's data array containing registers
*                    - startAddress: address of the first register in the supplied array
*                    - size: input array size in the requested format (16bit-registers)
*
*/
extern uint8_t modbusExchangeRegisters(volatile uint16_t *ptrToInArray, uint16_t startAddress, uint16_t size);
