/**

Stuff to implement:
 X RS485 direction control by using two separate receiver and transmitter control lines
 X Provide moisture in input register
 X Provide temperature in input register 
 X Address control via holding register
 * Baud rate control via holding register
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
#include "yaMBSiavr.h"
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
            uint16_t asArray[6];
            struct {
                uint16_t address;
                uint16_t baud;
                uint16_t parity;
                uint16_t stopBits;
                uint16_t measurementIntervalMs;
                uint16_t sleepTimeS;
            } asStruct;
        } holdingRegisters;

uint8_t eeprom_address EEMEM = 1;
uint8_t eeprom_baudIdx EEMEM = 0;
uint8_t eeprom_parityIdx EEMEM = 0;
uint8_t eeprom_stopBits_dx EEMEM = 0;
uint16_t eeprom_measurementIntervalMs EEMEM = 500;


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


uint8_t temp = 0;

inline static void reset() {
    WDTCSR = _BV(WDE);//reset in 16ms
    while(1);
} 

inline static bool isValidAddress(uint8_t address) {
    return address > 0 && address < 248;
}

void modbusGet(void) {
    if (modbusGetBusState() & (1<<ReceiveCompleted)) {
        switch(rxbuffer[1]) {
            case fcReadHoldingRegisters: {
                modbusExchangeRegisters(holdingRegisters.asArray,0,6);
            }
            break;
            
            case fcReadInputRegisters: {
                modbusExchangeRegisters(inputRegisters.asArray,0,2);
            }
            break;
            
            case fcPresetSingleRegister: {
                modbusExchangeRegisters(holdingRegisters.asArray,0,6);

                if(0 == modbusGetRegisterNumber()){
                    if(isValidAddress(holdingRegisters.asStruct.address)) {
                        eeprom_write_byte(&eeprom_address, holdingRegisters.asStruct.address);                    
                        while(!modbusIsIdle()) {
                            //wait
                        }
                        reset();
                    }
                }
            }
            break;
            
            case fcPresetMultipleRegisters: {
                modbusExchangeRegisters(holdingRegisters.asArray,0,6);
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

void timer0100usStart(void) {
    TCCR0B|=(1<<CS01); //prescaler 8
    TIMSK0|=(1<<TOIE0);
}


ISR(TIMER0_OVF_vect) {
    modbusTickTimer();
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
    holdingRegisters.asStruct.stopBits = eeprom_read_byte(&eeprom_stopBits_dx);
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
    modbusInit();
    adcSetup();    
    sei();    
    timer0100usStart();
    timer1msStart(&(holdingRegisters.asStruct.measurementIntervalMs));
   while(1) {
        processMeasurements(&(inputRegisters.asStruct.moisture), 
                            &(inputRegisters.asStruct.temperature));
        modbusGet();

        if(isSleepTimeSet() && modbusIsIdle()) {
            sleep();
        }
    }
}
