#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
//#include "thermistor.h"


#define POINTS_COUNT 20

typedef struct {
    int temp;
    int microvolts;
} temp_point;

static temp_point thermistorPoints[] = {
    { 850 , 129 },
    { 800 , 146 },
    { 750 , 165 },  
    { 700 , 186 },  
    { 650 , 210 },  
    { 600 , 236 },  
    { 550 , 267 },  
    { 500 , 300 },  
    { 450 , 337 },
    { 400 , 376 },  
    { 350 , 419 },  
    { 300 , 464 },  
    { 200 , 559 },  
    { 250 , 511 },   
    { 150 , 608 },  
    { 100 , 656 },
    { 50  , 703 },  
    { 0    , 748 },  
    { -50 , 789 },  
    { -100, 828 },  
    { -150, 862 },  
    { -200, 892 }
};

static inline int interpolate(int val, int rangeStart, int rangeEnd, int valStart, int valEnd) {
    return (rangeEnd - rangeStart) * (val - valStart) / (valEnd - valStart) + rangeStart;
}

static inline int interpolateVoltage(int temp, unsigned char i){
    return interpolate(temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp);
}

static inline int interpolateTemperature(int microvolts, unsigned char i){
//    printf("%d [%d -- %d] // [%d -- %d]\n", microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts);
    return interpolate(microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts);
}

/**
 * Returns the index of the first point whose temperature value is greater than argument
 **/
static inline unsigned char searchTemp(int temp) {
    unsigned char i;
    for(i = 0; i < POINTS_COUNT; i++) {
        if(thermistorPoints[i].temp > temp) {
            return i;
        }
    }
    return POINTS_COUNT-1;
}

/**
 * Returns the index of the first point whose microvolts value is greater than argument
 **/
static inline unsigned char searchMicrovolts(int microvolts) {
    unsigned char i;
    for(i = 0; i < POINTS_COUNT; i++) {
        if(thermistorPoints[i].microvolts > microvolts) {
            return i;
        }
    }
    return POINTS_COUNT-1;
}

long thermistorLsbToTemperature(unsigned int lsb) {
    return interpolateTemperature(lsb, searchMicrovolts(lsb));
}


inline static void ledOn() {
    PORTA |= _BV(PA5);
}

inline static void ledOff() {
    PORTA &= ~_BV(PA5);
};

inline static void ledSetup(){
    DDRA |= _BV(PA5);
}

#define DRIVER_ENABLE PA0
//#define UBRRVAL F_CPU/(16 * BAUD) + 1

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
#define CHANNEL_CAPACITANCE_HIGH 6
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

// static int64_t toTemperature(int64_t x) {
//     int64_t res = x*x*x*-25119L/100000000000L + x*x*4336623L/10000000000L + x * -346093150L/1000000000L + 122L;
//     return  res;
// }

int main (void) {
    ledSetup();
    stdout = &uart_output;
    serialSetup();
    serialDriverEnable();

    adcSetup();    
    
    while(1) {
        ledOn();
        _delay_ms(100);
        uint16_t thermistor = adcReadChannel(CHANNEL_THERMISTOR);

        fputs("Thermistor = ", stdout);
        
        itoa(thermistor, buff, 10);
        fputs(buff, stdout);
        fputs(" -> ", stdout);
        
        int tt = thermistorLsbToTemperature(thermistor);
//        fprintf("%d\n", tt, stdout);
        itoa((int)tt, buff, 10);
        fputs(buff, stdout);
        
        // uint16_t caph = adcReadChannel(CHANNEL_CAPACITANCE_HIGH);
        // uint16_t capl = adcReadChannel(CHANNEL_CAPACITANCE_LOW);

        // fputs("Capacitance = ", stdout);
        // itoa(caph, buff, 10);
        // fputs(buff, stdout);
        // fputs(" - ", stdout);

        // itoa(capl, buff, 10);
        // fputs(buff, stdout);
        // fputs(" = ", stdout);

        // itoa(caph - capl, buff, 10);
        // fputs(buff, stdout);
        
        // uint16_t chipTemp = adcReadChannel(CHANNEL_CHIP_TEMP);
        // fputs(" @ temp = ", stdout);
        // itoa(chipTemp, buff, 10);
        // fputs(buff, stdout);
        puts(".");

        ledOff();
        _delay_ms(100);
    }
}
