#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

void adcSetup();
void processMeasurements(uint16_t *ptrMoisture, int16_t *ptrTemperature);
void timer1msStart(volatile uint16_t *ptrToTimeout);
void measurementReset();
#endif