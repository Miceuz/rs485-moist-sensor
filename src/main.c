/**
Version history:

0x1000 -- initial
0x1001 -- bugfix, allow address change
0x1100 -- new feature - watchdog timer always on, fix sleep times, add firmware version input register 

Versioning number structure:
0x[major version, 4bit][feature update, 4bit][bugfix 8bit]
*/
#define FIRMWARE_VERSION 0x1100

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
            uint16_t asArray[3];
            struct {
                uint16_t moisture;
                int16_t temperature;
                uint16_t fwVersion;
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

static inline bool isMeasurementIntervalRegisterSet() {
    return 3 == modbusGetRegisterNumber();
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
                modbusExchangeRegisters(inputRegisters.asArray, 3);
            }
            break;
            
//            case fcPresetMultipleRegisters:
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
                } else if(isMeasurementIntervalRegisterSet()) {
                    eeprom_write_word(&eeprom_measurementIntervalMs, holdingRegisters.asStruct.measurementIntervalMs);
                }
            }
            break;
            
            default: {
                modbusSendException(ecIllegalFunction);
            }
            break;
        }
    }
}

volatile uint16_t sleepTimes = 0;
volatile uint16_t secondsToSleep = 0;

#define WDT_TIMEOUT_8S      0b00100001
#define WDT_TIMEOUT_4S      0b00100000
#define WDT_TIMEOUT_2S      0b00000111
#define WDT_TIMEOUT_1S      0b00000110
#define WDT_TIMEOUT_05S     0b00000101
#define WDT_TIMEOUT_025S    0b00000100
#define WDT_TIMEOUT_0125S   0b00000011
#define WDT_TIMEOUT_64MS    0b00000010
#define WDT_TIMEOUT_32MS    0b00000001
#define WDT_TIMEOUT_16MS    0b00000000
#define WDT_TIMEOUT_DEFAULT WDT_TIMEOUT_0125S

void wdtSetTimeout(uint8_t timeout) {
    timeout |= _BV(WDE);
    CCP = 0xD8;
    WDTCSR = timeout;
}

inline static void wdtInterruptEnable(){
    WDTCSR |= _BV(WDIE);
}

inline static void wdtInterruptDisable(){
    WDTCSR &= ~_BV(WDIE);
}

ISR(WDT_vect) {
   //Just wakeup. For some reason enabling WDT interrupt inside of ISR does not work, so we do it in sleep loop 
}

static inline void sleepSetup() {
    if(secondsToSleep >= 8) {
        wdtSetTimeout(WDT_TIMEOUT_8S);
        sleepTimes = secondsToSleep / 8;
        secondsToSleep = secondsToSleep - sleepTimes * 8;

    } else if(secondsToSleep >= 4) {
        wdtSetTimeout(WDT_TIMEOUT_4S);
        sleepTimes = 1;
        secondsToSleep = secondsToSleep - 4;
    
    } else if(secondsToSleep >= 2) {
        wdtSetTimeout(WDT_TIMEOUT_2S);
        sleepTimes = 1;
        secondsToSleep = 1;//secondsToSleep - 2;
    
    } else if(secondsToSleep == 1) {
        wdtSetTimeout(WDT_TIMEOUT_1S);
        sleepTimes = 1;
        secondsToSleep = secondsToSleep - 1;
    }
}

void sleep() {
    measurementReset();
    PORTA = 0;

    serialDriverDisable();
    serialReaderDisable();
    UCSR0B &= ~_BV(TXEN0);

    secondsToSleep = holdingRegisters.asStruct.sleepTimeS;
    holdingRegisters.asStruct.sleepTimeS = 0;

    sleepSetup();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    asm("WDR");
    while((sleepTimes > 0) || (secondsToSleep > 0)) {
        while(sleepTimes > 0) {
            wdtInterruptEnable();
            sleep_mode();
            sleepTimes--;
        }
        sleepSetup();
    }

    wdtInterruptDisable();
    wdtSetTimeout(WDT_TIMEOUT_DEFAULT);

    performMeasurement((uint16_t*) &(inputRegisters.asStruct.moisture), 
                            (uint16_t*) &(inputRegisters.asStruct.temperature));

    UCSR0B |= _BV(TXEN0);
    serialReaderEnable();
}

inline static bool isSleepTimeSet() {
    return 0 != holdingRegisters.asStruct.sleepTimeS;
}

inline static void saveConfig() {
    eeprom_write_byte(&eeprom_address, holdingRegisters.asStruct.address);
    eeprom_write_byte(&eeprom_baudIdx, holdingRegisters.asStruct.baud);
    eeprom_write_byte(&eeprom_parityIdx, holdingRegisters.asStruct.parity);
    eeprom_write_word(&eeprom_measurementIntervalMs, holdingRegisters.asStruct.measurementIntervalMs);
}

inline static void loadConfig() {
    inputRegisters.asStruct.fwVersion = FIRMWARE_VERSION;
    holdingRegisters.asStruct.sleepTimeS = 0;

    temp = eeprom_read_byte(&eeprom_address);
    
    if(isValidAddress(temp)) {
        holdingRegisters.asStruct.address = temp;
    } else {
        holdingRegisters.asStruct.address = 1;
    }

    temp = eeprom_read_byte(&eeprom_baudIdx);
    if(temp >= 0 && temp < 8) {
        holdingRegisters.asStruct.baud = temp;
    } else {
        holdingRegisters.asStruct.baud = 4;
    }

    temp = eeprom_read_byte(&eeprom_parityIdx);
    if(temp >=0 && temp < 3) {
        holdingRegisters.asStruct.parity = temp;
    } else {
        holdingRegisters.asStruct.parity = 0;
    }

    holdingRegisters.asStruct.measurementIntervalMs = eeprom_read_word(&eeprom_measurementIntervalMs);
    if(65535 == holdingRegisters.asStruct.measurementIntervalMs) {
        holdingRegisters.asStruct.measurementIntervalMs = 500;
    }
}

void main (void) {
    wdtSetTimeout(WDT_TIMEOUT_DEFAULT);

    DDRA |= _BV(DRIVER_ENABLE);
    DDRA |= _BV(READER_ENABLE);

    // DDRA |= _BV(PA3);
    // PINA |= _BV(PA3);
    // _delay_ms(100);
    // PINA |= _BV(PA3);

    loadConfig();

    modbusSetAddress(holdingRegisters.asStruct.address);
    modbusInit(holdingRegisters.asStruct.baud, holdingRegisters.asStruct.parity);

    adcSetup();    
    sei();    
    timer1msStart(&(holdingRegisters.asStruct.measurementIntervalMs));

    performMeasurement((uint16_t*) &(inputRegisters.asStruct.moisture), 
                            (uint16_t*) &(inputRegisters.asStruct.temperature));

    serialReaderEnable();

    while(1) {
        processMeasurements((uint16_t*) &(inputRegisters.asStruct.moisture), 
                            (uint16_t*) &(inputRegisters.asStruct.temperature));
        modbusGet();

        if(isSleepTimeSet() && modbusIsIdle() && modbusIsTXComplete()) {
            sleep();
        }
        asm("WDR");
    }
}
