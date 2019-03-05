/**
Version history:

0x1000 -- initial
0x1001 -- bugfix, allow address change
0x1100 -- new feature - watchdog timer always on, fix sleep times, add firmware version input register 

Versioning number structure:
0x[major version, 4bit][feature update, 4bit][bugfix 8bit]
*/
#define FIRMWARE_VERSION 0x1101

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
                uint16_t sleepTimeSec;
            } asStruct;
        } holdingRegisters;

uint16_t eeprom_address EEMEM = 1;
uint16_t eeprom_baudIdx EEMEM = 4;
uint16_t eeprom_parityIdx EEMEM = 0;
uint16_t eeprom_measurementIntervalMs EEMEM = 500;

uint16_t eeAddr = 0; //renamed from temp since temp is used in measurement.c Made it seem as though temperature was being loaded from eeprom.  Made u16 to support loading measurement interval.

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

inline static void reset() {  // WDE is a write protected register. CCP disables interrupts by default CLI,SEI uneeded. Configuration Change Protection signature needs to be set to 0xD8 for this to be written to.  Reset function call was likely non-functional.
	CCP = 0xD8;
	WDTCSR |= (1<<WDE);
	while(1);
} 

inline static bool isValidAddress(uint16_t address) {
    return address > 0 && address < 248;
}

inline static bool isValidBaud(uint16_t baudIdx) {
    return baudIdx >=0 && baudIdx < 8;
}

inline static bool isValidParity(uint16_t parityIdx) {
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

void saveWordAndReset(uint16_t *eeprom, uint16_t value) {
	cli();
	eeprom_write_word(eeprom, value);
	while(!modbusIsIdle()) {
		//wait
	}
	sei();
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
                        saveWordAndReset(&eeprom_address, holdingRegisters.asStruct.address);
                    }
                } else if (isBaudRegisterSet()) {
                    if(isValidBaud(holdingRegisters.asStruct.baud)) {
                        saveWordAndReset(&eeprom_baudIdx, holdingRegisters.asStruct.baud);
                    }
                } else if (isParityRegisterSet()) {
                    if(isValidParity(holdingRegisters.asStruct.parity)) {
                        saveWordAndReset(&eeprom_parityIdx, holdingRegisters.asStruct.parity);
                    }
                } else if(isMeasurementIntervalRegisterSet()) {
                    saveWordAndReset(&eeprom_measurementIntervalMs, holdingRegisters.asStruct.measurementIntervalMs);
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
	CCP = 0xD8;
    timeout |= _BV(WDE);
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

    secondsToSleep = holdingRegisters.asStruct.sleepTimeSec;
    holdingRegisters.asStruct.sleepTimeSec = 0;

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
                            (int16_t*) &(inputRegisters.asStruct.temperature));

    UCSR0B |= _BV(TXEN0);
    serialReaderEnable();
}

inline static bool isSleepTimeSet() {
    return 0 != holdingRegisters.asStruct.sleepTimeSec;
}

inline static void saveConfig() {
	cli();
	eeprom_update_word(&eeprom_address, holdingRegisters.asStruct.address);
	eeprom_update_word(&eeprom_baudIdx, holdingRegisters.asStruct.baud);
	eeprom_update_word(&eeprom_parityIdx, holdingRegisters.asStruct.parity);
	eeprom_update_word(&eeprom_measurementIntervalMs, holdingRegisters.asStruct.measurementIntervalMs);
	sei();
}

inline static void loadConfig() {
	inputRegisters.asStruct.fwVersion = FIRMWARE_VERSION;
	holdingRegisters.asStruct.sleepTimeSec = 0;

	eeAddr = eeprom_read_word(&eeprom_address);
	
	if(isValidAddress(eeAddr)) { //set to eeprom stored address or set to one if false returned
		holdingRegisters.asStruct.address = eeAddr;
		} else {
		holdingRegisters.asStruct.address = 1;  // load default value if it's totally borked.
	}

	eeAddr = eeprom_read_word(&eeprom_baudIdx);
	if(eeAddr >= 0 && eeAddr < 8) { //set to eeprom stored baud rate between 0 and 7 or set to 4 if out of bounds
		holdingRegisters.asStruct.baud = eeAddr;
		} else {
		holdingRegisters.asStruct.baud = 4;  // load default value if it's totally borked.
	}

	eeAddr = eeprom_read_word(&eeprom_parityIdx);
	if(eeAddr >=0 && eeAddr < 3) { //set to eeprom stored parity between 0 and 2 or set to 0 if out of bounds
		holdingRegisters.asStruct.parity = eeAddr;
		} else {
		holdingRegisters.asStruct.parity = 0;  // load default value if it's totally borked.
	}

	eeAddr = eeprom_read_word(&eeprom_measurementIntervalMs);
	if (eeAddr >=0 && eeAddr <= 65535) {	// if less than or equal to 655355 and greater or equal to zero, load value from eeprom
		holdingRegisters.asStruct.measurementIntervalMs = eeAddr;
		} else {
		holdingRegisters.asStruct.measurementIntervalMs = 500;	// load default value if it's totally borked.
	}
}

int main (void) {
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
                            (int16_t*) &(inputRegisters.asStruct.temperature));

    serialReaderEnable();

    while(1) {
        processMeasurements((uint16_t*) &(inputRegisters.asStruct.moisture), 
                            (int16_t*) &(inputRegisters.asStruct.temperature));
        modbusGet();

        if(isSleepTimeSet() && modbusIsIdle() && modbusIsTXComplete()) {
            sleep();
        }
        asm("WDR");
    }
}
