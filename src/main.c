/**

Stuff to implement:
 X RS485 direction control by using two separate receiver and transmitter control lines
 X Provide moisture in input register
 X Provide temperature in input register 
 * Address control via holding register
 * Baud rate control via holding register
 X Deep sleep mode via holding register
 X Excitation control via holding register
 * Add a timekeeper millisecond timer  
*/

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/sleep.h>
#include "thermistor.h"
#include "yaMBSiavr.h"


#define DRIVER_ENABLE PA4
#define READER_ENABLE PA0

#define CHANNEL_THERMISTOR 3
#define CHANNEL_CAPACITANCE_HIGH 5
#define CHANNEL_CAPACITANCE_LOW 7
#define CHANNEL_CHIP_TEMP 0b00001100

#define POWER_TO_DIVIDERS PA6


volatile uint8_t instate = 0;
volatile uint8_t outstate = 0;
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

inline static void serialSetup() {
    #define BAUD 9600
    #include <util/setbaud.h>
    DDRA |= _BV(DRIVER_ENABLE);
    DDRA |= _BV(READER_ENABLE);
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

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


void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

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

void modbusGet(void) {
    if (modbusGetBusState() & (1<<ReceiveCompleted)) {
        switch(rxbuffer[1]) {
            case fcReadHoldingRegisters: {
                modbusExchangeRegisters(holdingRegisters.asArray,0,4);
            }
            break;
            
            case fcReadInputRegisters: {
                modbusExchangeRegisters(inputRegisters.asArray,0,4);
            }
            break;
            
            case fcPresetSingleRegister: {
                modbusExchangeRegisters(holdingRegisters.asArray,0,4);
            }
            break;
            
            case fcPresetMultipleRegisters: {
                modbusExchangeRegisters(holdingRegisters.asArray,0,4);
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

inline static uint8_t isTimeToMeasure() {
    return 0;
}

inline static uint8_t isTimeToStabilizeOver() {
    return 0;
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

    uint16_t sleepTimes = 0;
    if(holdingRegisters.asStruct.sleepTimeS >=8) {
        WDTCSR = 0b00001001;
        sleepTimes = holdingRegisters.asStruct.sleepTimeS / 8;
    } else if(holdingRegisters.asStruct.sleepTimeS >=4) {
        WDTCSR = 0b00001000;
        sleepTimes = holdingRegisters.asStruct.sleepTimeS / 4;
    } else if(holdingRegisters.asStruct.sleepTimeS >=2) {
        WDTCSR = 0b00000111;
        sleepTimes = holdingRegisters.asStruct.sleepTimeS / 2;
    } else if(holdingRegisters.asStruct.sleepTimeS >=1) {
        WDTCSR = 0b00000110;
        sleepTimes = 1;
    }

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    WDTCSR |= _BV(WDIE);
    while(sleepTimes-- > 0) {
        WDTCSR |= _BV(WDIE);
        sleep_mode();
    }

    temp = WDTCSR & ~_BV(WDE);
    CCP = 0xD8;
    WDTCSR = temp;

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
            inputRegisters.asStruct.moisture = caph - capl;

            powerToDividersDisable();
            adcDisable();
            excitationDisable();
            measurementState = STATE_MEASUREMENT_OFF;
        }
            break;
    }
}

inline static isSleepTimeSet() {
    return 0 != holdingRegisters.asStruct.sleepTimeS;
}

int main (void) {

    serialSetup();
    
    serialDriverEnable();
    serialReaderDisable();
    
    modbusInit();
    
    adcSetup();    

    sleep();
    
    while(1) {
        processMeasurements();
        modbusGet();
        // _delay_ms(100);

        // powerToDividersEnable();
        // _delay_ms(2);
        // uint16_t thermistor = adcReadChannel(CHANNEL_THERMISTOR);
        // int tt = thermistorLsbToTemperature(thermistor);
        
        // uint16_t caph = adcReadChannel(CHANNEL_CAPACITANCE_HIGH);
        // uint16_t capl = adcReadChannel(CHANNEL_CAPACITANCE_LOW);

        // powerToDividersDisable();

        // _delay_ms(100);
        if(isSleepTimeSet()) {
            sleep();
        }
    }
}
