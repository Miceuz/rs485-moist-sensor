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

/**
 * @brief    Modbus Exception Codes
 *           Refer to modbus.org for further information.
 *           It's good practice to return exception code 01 in case you receive a function code
 *           that you haven't implemented in your application.
 */
#ifndef __MODBUS_H
#define __MODBUS_H

#include <stdbool.h>

#define ecIllegalFunction 1 
#define ecIllegalDataAddress 2 
#define ecIllegalDataValue 3
#define ecSlaveDeviceFailure 4
#define ecAcknowledge 5
#define ecSlaveDeviceBusy 6
#define ecNegativeAcknowledge 7
#define ecMemoryParityError 8

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

void modbusInit(uint8_t baudIdx, uint8_t parityIdx);
bool modbusIsFrameAvailable();
uint8_t modbusGetRegisterNumber(void);
uint8_t modbusGetFunctionCode(void);
void modbusIsBroadcastFrame();
bool modbusIsIdle();
uint8_t modbusExchangeRegisters(volatile uint16_t *ptrToInArray, uint16_t size);
void modbusSendException(unsigned char exceptionCode);
void modbusSetAddress(uint8_t address);

#define BAUD_1200 	0
#define BAUD_2400	1
#define BAUD_4800 	2
#define BAUD_9600	3
#define BAUD_19200	4
#define BAUD_38400	5
#define BAUD_57600	6
#define BAUD_115200	7

#define PARITY_NONE 0
#define PARITY_EVEN 1
#define PARITY_ODD  2

extern void transceiver_txen(void);
extern void transceiver_rxen(void);

#endif