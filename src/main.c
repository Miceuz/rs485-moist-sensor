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
#include "thermistor.h"
#include "yaMBSiavr.h"
#include <avr/eeprom.h>


#define DRIVER_ENABLE PA4
#define READER_ENABLE PA0

#define CHANNEL_THERMISTOR 3
#define CHANNEL_CAPACITANCE_HIGH 5
#define CHANNEL_CAPACITANCE_LOW 7
#define CHANNEL_CHIP_TEMP 0b00001100

#define POWER_TO_DIVIDERS PA6

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

volatile uint16_t milliseconds = 0;

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

void powerToDividersEnable() {
    PORTA |= _BV(POWER_TO_DIVIDERS);
}

void powerToDividersDisable() {
    PORTA &= ~_BV(POWER_TO_DIVIDERS);
}

static inline void adcSetup() {
    DDRA |= _BV(POWER_TO_DIVIDERS);

    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);
    ADMUXB = 0;
    DIDR0 |= _BV(ADC7D) | _BV(ADC5D) | _BV(ADC3D);// | _BV(ADC0D) | _BV(ADC1D) | _BV(ADC2D) | _BV(ADC4D) | _BV(ADC6D) | _BV(ADC8D);
}

uint16_t adcReadChannel(uint8_t channel) {
    ADMUXA = channel;
    ADCSRA |= _BV(ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    uint16_t ret = ADC;
    return ret;
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


#define STATE_MEASUREMENT_OFF 0
#define STATE_MEASUREMENT_STABILIZE 1
#define STATE_MEASUREMENT_IN_PROGRESS 2
uint8_t measurementState = STATE_MEASUREMENT_OFF;

inline static bool isTimeToMeasure() {
    return milliseconds > holdingRegisters.asStruct.measurementIntervalMs;
}

inline static bool isTimeToStabilizeOver() {
    return milliseconds > holdingRegisters.asStruct.measurementIntervalMs + 2;
}

inline static void adcEnable() {
    ADCSRA |= _BV(ADEN);
}

inline static void adcDisable() {
    ADCSRA &= ~_BV(ADEN);
}

inline static void excitationEnable() {
    temp = CLKCR & ~_BV(CKOUTC);
    CCP = 0xD8;
    CLKCR = temp;
}

inline static void excitationDisable() {
    temp = CLKCR | _BV(CKOUTC);
    CCP = 0xD8;
    CLKCR = temp;
}

uint16_t sleepTimes = 0;

void sleep() {
    excitationDisable();
    PORTA = 0;

    DDRB |= _BV(PB2);
    PORTB &= ~_BV(PB2);

    serialDriverDisable();
    serialReaderDisable();
    UCSR0B &= ~_BV(TXEN0);

    adcDisable();

    ACSR0A = _BV(ACD0); //disable comparators
    ACSR1A = _BV(ACD1);

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

inline static void startMeasurementTimer() {
    TIMSK1 &= ~_BV(OCIE1A);
    milliseconds = 0;
    TCNT1 = 0;
    TIMSK1 |= _BV(OCIE1A);
}

inline static void processMeasurements() {
    switch(measurementState) {
        case STATE_MEASUREMENT_OFF:
            if(isTimeToMeasure()) {
                powerToDividersEnable();
                adcEnable();
                excitationEnable();
                measurementState = STATE_MEASUREMENT_STABILIZE;
            }
            break;

        case STATE_MEASUREMENT_STABILIZE:
            if(isTimeToStabilizeOver()) {
                measurementState = STATE_MEASUREMENT_IN_PROGRESS;
            }
            break;

        case STATE_MEASUREMENT_IN_PROGRESS: {      
            uint16_t thermistor = adcReadChannel(CHANNEL_THERMISTOR);
            inputRegisters.asStruct.temperature = thermistorLsbToTemperature(thermistor);
            
            uint16_t caph = adcReadChannel(CHANNEL_CAPACITANCE_HIGH);
            uint16_t capl = adcReadChannel(CHANNEL_CAPACITANCE_LOW);
            inputRegisters.asStruct.moisture = 1023 - (caph - capl);

            powerToDividersDisable();
            adcDisable();
            excitationDisable();
            startMeasurementTimer();
            measurementState = STATE_MEASUREMENT_OFF;
        }
            break;
    }
}

inline static bool isSleepTimeSet() {
    return 0 != holdingRegisters.asStruct.sleepTimeS;
}

void timer0100usStart(void) {
    TCCR0B|=(1<<CS01); //prescaler 8
    TIMSK0|=(1<<TOIE0);
}

void timer1msStart() {
    OCR1A = (uint16_t) 16000L;
    TIMSK1 |= _BV(OCIE1A);
    TCCR1B = _BV(CS10) | _BV(WGM12);
}

ISR(TIMER1_COMPA_vect) {
    milliseconds++;
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
    timer1msStart();
   while(1) {
        processMeasurements();
        modbusGet();

        if(modbusIsIdle() && isSleepTimeSet()) {
            sleep();
        }
    }
}
