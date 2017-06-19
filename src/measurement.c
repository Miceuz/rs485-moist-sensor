#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "thermistor.h"

#define CHANNEL_THERMISTOR 3
#define CHANNEL_CAPACITANCE_HIGH 5
#define CHANNEL_CAPACITANCE_LOW 7
#define POWER_TO_DIVIDERS PA6

#define STATE_MEASUREMENT_OFF 0
#define STATE_MEASUREMENT_STABILIZE 1
#define STATE_MEASUREMENT_IN_PROGRESS 2
uint8_t measurementState = STATE_MEASUREMENT_OFF;

volatile uint16_t milliseconds = 0;
volatile uint16_t *measurementTimeoutMs;
uint8_t temp;

static inline void powerToDividersEnable() {
    PORTA |= _BV(POWER_TO_DIVIDERS);
}

static inline void powerToDividersDisable() {
    PORTA &= ~_BV(POWER_TO_DIVIDERS);
}

inline static void adcEnable() {
    ADCSRA |= _BV(ADEN);
}

inline static void adcDisable() {
    ADCSRA &= ~_BV(ADEN);
}

void adcSetup() {
    DDRA |= _BV(POWER_TO_DIVIDERS);

    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);
    ADMUXB = 0;
    DIDR0 |= _BV(ADC7D) | _BV(ADC5D) | _BV(ADC3D);// | _BV(ADC0D) | _BV(ADC1D) | _BV(ADC2D) | _BV(ADC4D) | _BV(ADC6D) | _BV(ADC8D);

    ACSR0A = _BV(ACD0); //disable comparators
    ACSR1A = _BV(ACD1);

    adcDisable();
}

uint16_t adcReadChannel(uint8_t channel) {
    ADMUXA = channel;
    ADCSRA |= _BV(ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    uint16_t ret = ADC;
    return ret;
}

inline static bool isTimeToMeasure() {
    return milliseconds > *measurementTimeoutMs;
}

inline static bool isTimeToStabilizeOver() {
    return milliseconds > *measurementTimeoutMs + 2;
}

inline static void startMeasurementTimer() {
    TIMSK1 &= ~_BV(OCIE1A);
    milliseconds = 0;
    TCNT1 = 0;
    TIMSK1 |= _BV(OCIE1A);
}

void timer1msStart(volatile uint16_t *ptrToTimeout) {
    measurementTimeoutMs = ptrToTimeout;
    OCR1A = (uint16_t) 16000L;
    TIMSK1 |= _BV(OCIE1A);
    TCCR1B = _BV(CS10) | _BV(WGM12);
}

ISR(TIMER1_COMPA_vect) {
    milliseconds++;
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
    DDRB |= _BV(PB2);
    PORTB &= ~_BV(PB2);
}

void measurementReset() {
    powerToDividersDisable();
    adcDisable();
    excitationDisable();
    startMeasurementTimer();
    measurementState = STATE_MEASUREMENT_OFF;
}

bool isMeasurementInProgress() {
    return measurementState != STATE_MEASUREMENT_OFF;
}

inline static void measurementPeripheryOn() {
    powerToDividersEnable();
    adcEnable();
    excitationEnable();
}

void processMeasurements(uint16_t *ptrMoisture, int16_t *ptrTemperature) {
    switch(measurementState) {
        case STATE_MEASUREMENT_OFF:
            if(isTimeToMeasure()) {
                measurementPeripheryOn();
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
            *ptrTemperature = thermistorLsbToTemperature(thermistor);
            
            uint16_t caph = adcReadChannel(CHANNEL_CAPACITANCE_HIGH);
            uint16_t capl = adcReadChannel(CHANNEL_CAPACITANCE_LOW);
            *ptrMoisture = 1023 - (caph - capl);
            measurementReset();
            }
            break;
    }
}

inline static void forceStartMeasurement() {
    cli();
    milliseconds = *measurementTimeoutMs+1;
    sei();
    measurementPeripheryOn();
    measurementState = STATE_MEASUREMENT_STABILIZE;
}

void performMeasurement(uint16_t *ptrMoisture, int16_t *ptrTemperature) {
    forceStartMeasurement();
    while(isMeasurementInProgress()) {
        processMeasurements(ptrMoisture, ptrTemperature);
    }
}


