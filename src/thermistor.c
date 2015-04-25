#include <stdio.h>

#define POINTS_COUNT 22

typedef	struct {
	long temp;
	unsigned long microvolts;
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

static inline long interpolate(long val, long rangeStart, long rangeEnd, long valStart, long valEnd) {
//    printf("%ld + (%ld - %ld) * (%ld - %ld) / (%ld - %ld)\n", rangeStart, rangeEnd, rangeStart, val, valStart, valEnd, valStart);
    return (rangeEnd - rangeStart) * (val - valStart) / (valEnd - valStart) + rangeStart;
}

static inline long interpolateVoltage(unsigned long temp, unsigned char i){
    return interpolate(temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp);
}

static inline long interpolateTemperature(unsigned long microvolts, unsigned char i){
//	printf("%d [%d -- %d] // [%d -- %d]\n", microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts);
    return interpolate(microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts);
}

/**
 * Returns the index of the first point whose temperature value is greater than argument
 **/
static inline unsigned char searchTemp(unsigned long temp) {
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
static inline unsigned char searchMicrovolts(unsigned long microvolts) {
	unsigned char i;
	for(i = 0; i < POINTS_COUNT; i++) {
		if(thermistorPoints[i].microvolts > microvolts) {
			return i;
		}
	}
	return POINTS_COUNT-1;
}

long thermistorLsbToTemperature(unsigned long lsb) {
	return interpolateTemperature(lsb, searchMicrovolts(lsb));
}

void main() {
	int i;
	for(i = 128; i < 893; i++) {
		printf("%d = %ld\n", i, thermistorLsbToTemperature(i));
	}
}
