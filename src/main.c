#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "thermistor.h"


#define DRIVER_ENABLE PA0

inline static void serialSetup() {
    #define BAUD 9600
    #include <util/setbaud.h>
    DDRA |= _BV(DRIVER_ENABLE);
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

inline static void serialDriverEnable() {
    PORTA |= _BV(DRIVER_ENABLE);
}

inline static void serialDriverDisable() {
    PORTA &= ~_BV(DRIVER_ENABLE);
}

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

static inline void adcSetup() {
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);
    ADMUXB = 0;
}

#define CHANNEL_THERMISTOR 3
#define CHANNEL_CAPACITANCE_HIGH 5
#define CHANNEL_CAPACITANCE_LOW 7
#define CHANNEL_CHIP_TEMP 0b00001100

uint16_t adcReadChannel(uint8_t channel) {
    ADMUXA = channel;
    ADCSRA |= _BV(ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    uint16_t ret = ADC;
    return ret;
}

char buff[10];

int main (void) {
    stdout = &uart_output;
    serialSetup();
    serialDriverEnable();

    adcSetup();    
    
    while(1) {
        _delay_ms(100);
        uint16_t thermistor = adcReadChannel(CHANNEL_THERMISTOR);

        fputs("Thermistor = ", stdout);
        
        itoa(thermistor, buff, 10);
        fputs(buff, stdout);
        fputs(" -> ", stdout);
        
        int tt = thermistorLsbToTemperature(thermistor);
        itoa((int)tt, buff, 10);
        fputs(buff, stdout);
        
        uint16_t caph = adcReadChannel(CHANNEL_CAPACITANCE_HIGH);
        uint16_t capl = adcReadChannel(CHANNEL_CAPACITANCE_LOW);

        fputs(" Capacitance = ", stdout);
        itoa(caph, buff, 10);
        fputs(buff, stdout);
        fputs(" - ", stdout);

        itoa(capl, buff, 10);
        fputs(buff, stdout);
        fputs(" = ", stdout);

        itoa(caph - capl, buff, 10);
        fputs(buff, stdout);
        
        uint16_t chipTemp = adcReadChannel(CHANNEL_CHIP_TEMP);
        fputs(" @ temp = ", stdout);
        itoa(chipTemp, buff, 10);
        fputs(buff, stdout);
        puts(".");

        _delay_ms(100);
    }
}
