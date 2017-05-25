/**

Stuff to implement:
 X RS485 direction control by using two separate receiver and transmitter control lines
 X Provide moisture in input register
 X Provide temperature in input register 
 X Address control via holding register
 X Baud rate control via holding register
 * Update timeout counters when updating baud 
 X Save config to eeprom and reset
 X Deep sleep mode via holding register
 X Excitation control via holding register
 X Add a timekeeper millisecond timer  
*/

#include <stdbool.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sleep.h>
#include "modbus.h"
#include <avr/eeprom.h>
#include <measurement.h>

#define DRIVER_ENABLE PA4
#define READER_ENABLE PA0

volatile union{
            uint16_t asArray[2];
            struct {
                uint16_t moisture;
                int16_t temperature;
            } asStruct;
        } inputRegisters;

volatile union{
            uint16_t asArray[5];
            struct {
                uint16_t address;
                uint16_t baud;
                uint16_t parity;
                uint16_t measurementIntervalMs;
                uint16_t sleepTimeS;
            } asStruct;
        } holdingRegisters;

uint8_t eeprom_address EEMEM = 1;
uint8_t eeprom_baudIdx EEMEM = 4;
uint8_t eeprom_parityIdx EEMEM = 0;
uint16_t eeprom_measurementIntervalMs EEMEM = 500;

uint8_t temp = 0;

inline static void serialReaderDisable() {
    PORTA |= _BV(READER_ENABLE);
}

inline static void serialDriverDisable() {
    PORTA &= ~_BV(DRIVER_ENABLE);
}

inline static void serialDriverEnable() {
    serialReaderDisable();
    PORTA |= _BV(DRIVER_ENABLE);
}

inline static void serialReaderEnable() {
    serialDriverDisable();
    PORTA &= ~_BV(READER_ENABLE);
}


void transceiver_txen(void) {
    serialDriverEnable();
}

void transceiver_rxen(void) {
    serialReaderEnable();
}

inline static void reset() {
    WDTCSR = _BV(WDE);//reset in 16ms
    while(1);
} 

inline static bool isValidAddress(uint8_t address) {
    return address > 0 && address < 248;
}

inline static bool isValidBaud(uint8_t baudIdx) {
    return baudIdx >=0 && baudIdx < 8;
}

inline static bool isValidParity(uint8_t parityIdx) {
    return parityIdx >=0 && parityIdx < 3;
}

static inline bool isAddressRegisterSet() {
    return 0 == modbusGetRegisterNumber();
}

static inline bool isBaudRegisterSet() {
    return 1 == modbusGetRegisterNumber();
}

static inline bool isParityRegisterSet() {
    return 2 == modbusGetRegisterNumber();
}

void saveByteAndReset(uint8_t *eeprom, uint8_t value) {
    eeprom_write_byte(eeprom, value);                    
    while(!modbusIsIdle()) {
        //wait
    }
    reset();
}

void modbusGet(void) {
    if (modbusIsFrameAvailable()) {
        switch(modbusGetFunctionCode()) {
            case fcReadHoldingRegisters: {
                modbusExchangeRegisters(holdingRegisters.asArray, 5);
            }
            break;
            
            case fcReadInputRegisters: {
                modbusExchangeRegisters(inputRegisters.asArray, 2);
            }
            break;
            
            case fcPresetSingleRegister: {
                modbusExchangeRegisters(holdingRegisters.asArray, 5);

                if(isAddressRegisterSet()){
                    if(isValidAddress(holdingRegisters.asStruct.address)) {
                        saveByteAndReset(&eeprom_address, holdingRegisters.asStruct.address);
                    }
                } else if (isBaudRegisterSet()) {
                    if(isValidBaud(holdingRegisters.asStruct.baud)) {
                        saveByteAndReset(&eeprom_baudIdx, holdingRegisters.asStruct.baud);
                    }
                } else if (isParityRegisterSet()) {
                    if(isValidParity(holdingRegisters.asStruct.parity)) {
                        saveByteAndReset(&eeprom_parityIdx, holdingRegisters.asStruct.parity);
                    }
                }
            }
            break;
            
            case fcPresetMultipleRegisters: {
                modbusExchangeRegisters(holdingRegisters.asArray, 5);
            }
            break;
            
            default: {
                modbusSendException(ecIllegalFunction);
            }
            break;
        }
    }
}

uint16_t sleepTimes = 0;

void sleep() {
    measurementReset();
    PORTA = 0;

    serialDriverDisable();
    serialReaderDisable();
    UCSR0B &= ~_BV(TXEN0);

    if(holdingRegisters.asStruct.sleepTimeS >=8) {
        WDTCSR = 0b00100001;
        sleepTimes = holdingRegisters.asStruct.sleepTimeS / 8;
    } else if(holdingRegisters.asStruct.sleepTimeS >=4) {
        WDTCSR = 0b00100000;
        sleepTimes = holdingRegisters.asStruct.sleepTimeS / 4;
    } else if(holdingRegisters.asStruct.sleepTimeS >=2) {
        WDTCSR = 0b00000111;
        sleepTimes = holdingRegisters.asStruct.sleepTimeS / 2;
    } else if(holdingRegisters.asStruct.sleepTimeS >=1) {
        WDTCSR = 0b00000110;
        sleepTimes = 1;
    }

    holdingRegisters.asStruct.sleepTimeS = 0;
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    WDTCSR |= _BV(WDIE);
    while(sleepTimes > 0) {
        sleep_mode();
    }

    temp = WDTCSR & ~_BV(WDE) & ~_BV(WDIE);
    CCP = 0xD8;
    WDTCSR = temp;

    UCSR0B |= _BV(TXEN0);
    serialReaderEnable();
}

ISR(WDT_vect) {
    WDTCSR |= _BV(WDIE);
    sleepTimes--;
}

inline static bool isSleepTimeSet() {
    return 0 != holdingRegisters.asStruct.sleepTimeS;
}

inline static void saveConfig() {

}

inline static void loadConfig() {
    temp = eeprom_read_byte(&eeprom_address);
    if(isValidAddress(temp)) {
        holdingRegisters.asStruct.address = temp;
    } else {
        holdingRegisters.asStruct.address = 1;
    }

    holdingRegisters.asStruct.baud = eeprom_read_byte(&eeprom_baudIdx);
    holdingRegisters.asStruct.parity = eeprom_read_byte(&eeprom_parityIdx);
    holdingRegisters.asStruct.measurementIntervalMs = eeprom_read_word(&eeprom_measurementIntervalMs);
    holdingRegisters.asStruct.sleepTimeS = 0;
}

inline static void  wdtDisable(){
    MCUSR = 0;
    temp = WDTCSR & ~_BV(WDE) & ~_BV(WDIE);
    CCP = 0xD8;
    WDTCSR = temp;
}

void main (void) {
    wdtDisable();

    DDRA |= _BV(DRIVER_ENABLE);
    DDRA |= _BV(READER_ENABLE);

    // DDRA |= _BV(PA3);
    // PINA |= _BV(PA3);
    // _delay_ms(100);
    // PINA |= _BV(PA3);

    loadConfig();

    serialDriverEnable();
    
    modbusSetAddress(holdingRegisters.asStruct.address);
    modbusInit(holdingRegisters.asStruct.baud, holdingRegisters.asStruct.parity);
    adcSetup();    
    sei();    
    timer1msStart(&(holdingRegisters.asStruct.measurementIntervalMs));
   while(1) {
        processMeasurements((uint16_t*) &(inputRegisters.asStruct.moisture), 
                            (uint16_t*) &(inputRegisters.asStruct.temperature));
        modbusGet();

        if(isSleepTimeSet() && modbusIsIdle()) {
            sleep();
        }
    }
}
