EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:rs485-moist-sensor-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Digital soil moisture sensor"
Date "Tue 17 Feb 2015"
Rev ""
Comp "Catnip electronics"
Comment1 "by Albertas Mickėnas mic@wemakethings.net"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CRYSTAL X1
U 1 1 54D213EA
P 5700 2800
F 0 "X1" H 5700 2950 50  0000 C CNN
F 1 "16MHz" H 5700 2650 50  0000 C CNN
F 2 "Crystals:Crystal_HC49-SD_SMD" H 5700 2800 60  0001 C CNN
F 3 "" H 5700 2800 60  0000 C CNN
	1    5700 2800
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 54D21881
P 5400 2950
F 0 "C1" H 5450 3050 50  0000 L CNN
F 1 "18p" H 5450 2850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5438 2800 30  0001 C CNN
F 3 "" H 5400 2950 60  0000 C CNN
	1    5400 2950
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 54D218DE
P 6000 2950
F 0 "C2" H 6050 3050 50  0000 L CNN
F 1 "18p" H 6050 2850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6038 2800 30  0001 C CNN
F 3 "" H 6000 2950 60  0000 C CNN
	1    6000 2950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 54D21A04
P 5700 3100
F 0 "#PWR01" H 5700 2850 60  0001 C CNN
F 1 "GND" H 5700 2950 60  0000 C CNN
F 2 "" H 5700 3100 60  0000 C CNN
F 3 "" H 5700 3100 60  0000 C CNN
	1    5700 3100
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR02
U 1 1 54D21DD5
P 1550 1700
F 0 "#PWR02" H 1550 1550 60  0001 C CNN
F 1 "VCC" H 1550 1850 60  0000 C CNN
F 2 "" H 1550 1700 60  0000 C CNN
F 3 "" H 1550 1700 60  0000 C CNN
	1    1550 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 54D21DE7
P 1550 2900
F 0 "#PWR03" H 1550 2650 60  0001 C CNN
F 1 "GND" H 1550 2750 60  0000 C CNN
F 2 "" H 1550 2900 60  0000 C CNN
F 3 "" H 1550 2900 60  0000 C CNN
	1    1550 2900
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 54D21E5C
P 1550 1850
F 0 "C3" H 1600 1950 50  0000 L CNN
F 1 "1u" H 1600 1750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1588 1700 30  0001 C CNN
F 3 "" H 1550 1850 60  0000 C CNN
	1    1550 1850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 54D21F25
P 1550 2000
F 0 "#PWR04" H 1550 1750 60  0001 C CNN
F 1 "GND" H 1550 1850 60  0000 C CNN
F 2 "" H 1550 2000 60  0000 C CNN
F 3 "" H 1550 2000 60  0000 C CNN
	1    1550 2000
	1    0    0    -1  
$EndComp
$Comp
L DIODESCH D1
U 1 1 54D21FCD
P 9100 2350
F 0 "D1" H 9100 2450 50  0000 C CNN
F 1 "BAT54" H 9100 2250 50  0000 C CNN
F 2 "SMD_Packages:SOD-523" H 9100 2350 60  0001 C CNN
F 3 "" H 9100 2350 60  0000 C CNN
	1    9100 2350
	-1   0    0    1   
$EndComp
$Comp
L DIODESCH D2
U 1 1 54D22068
P 9100 3800
F 0 "D2" H 9100 3900 50  0000 C CNN
F 1 "BAT54" H 9100 3700 50  0000 C CNN
F 2 "SMD_Packages:SOD-523" H 9100 3800 60  0001 C CNN
F 3 "" H 9100 3800 60  0000 C CNN
	1    9100 3800
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 54D2209D
P 8350 3050
F 0 "R4" V 8430 3050 50  0000 C CNN
F 1 "330" V 8357 3051 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8280 3050 30  0001 C CNN
F 3 "" H 8350 3050 30  0000 C CNN
	1    8350 3050
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR05
U 1 1 54D2215E
P 9300 2050
F 0 "#PWR05" H 9300 1900 60  0001 C CNN
F 1 "VCC" H 9300 2200 60  0000 C CNN
F 2 "" H 9300 2050 60  0000 C CNN
F 3 "" H 9300 2050 60  0000 C CNN
	1    9300 2050
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 54D22174
P 9300 2200
F 0 "R5" V 9380 2200 50  0000 C CNN
F 1 "1M" V 9307 2201 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9230 2200 30  0001 C CNN
F 3 "" H 9300 2200 30  0000 C CNN
	1    9300 2200
	-1   0    0    1   
$EndComp
$Comp
L R R8
U 1 1 54D221C8
P 9600 3950
F 0 "R8" V 9680 3950 50  0000 C CNN
F 1 "1M" V 9607 3951 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9530 3950 30  0001 C CNN
F 3 "" H 9600 3950 30  0000 C CNN
	1    9600 3950
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR06
U 1 1 54D2221D
P 9300 4100
F 0 "#PWR06" H 9300 3850 60  0001 C CNN
F 1 "GND" H 9300 3950 60  0000 C CNN
F 2 "" H 9300 4100 60  0000 C CNN
F 3 "" H 9300 4100 60  0000 C CNN
	1    9300 4100
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 54D2222A
P 9300 2500
F 0 "C4" H 9350 2600 50  0000 L CNN
F 1 "1n" H 9350 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9338 2350 30  0001 C CNN
F 3 "" H 9300 2500 60  0000 C CNN
	1    9300 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 54D222DF
P 9300 2650
F 0 "#PWR07" H 9300 2400 60  0001 C CNN
F 1 "GND" H 9300 2500 60  0000 C CNN
F 2 "" H 9300 2650 60  0000 C CNN
F 3 "" H 9300 2650 60  0000 C CNN
	1    9300 2650
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 54D222F8
P 9600 2500
F 0 "R7" V 9680 2500 50  0000 C CNN
F 1 "1M" V 9607 2501 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9530 2500 30  0001 C CNN
F 3 "" H 9600 2500 30  0000 C CNN
	1    9600 2500
	-1   0    0    1   
$EndComp
Text Label 9900 2350 0    60   ~ 0
SENSE_LOW
$Comp
L C C5
U 1 1 54D22484
P 9300 3950
F 0 "C5" H 9350 4050 50  0000 L CNN
F 1 "1n" H 9350 3850 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 9338 3800 30  0001 C CNN
F 3 "" H 9300 3950 60  0000 C CNN
	1    9300 3950
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 54D2252C
P 9300 3650
F 0 "R6" V 9380 3650 50  0000 C CNN
F 1 "1M" V 9307 3651 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 9230 3650 30  0001 C CNN
F 3 "" H 9300 3650 30  0000 C CNN
	1    9300 3650
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR08
U 1 1 54D226D9
P 9300 3500
F 0 "#PWR08" H 9300 3350 60  0001 C CNN
F 1 "VCC" H 9300 3650 60  0000 C CNN
F 2 "" H 9300 3500 60  0000 C CNN
F 3 "" H 9300 3500 60  0000 C CNN
	1    9300 3500
	1    0    0    -1  
$EndComp
Text Label 9900 3800 0    60   ~ 0
SENSE_HIGH/MOSI
Text Label 8500 3450 2    60   ~ 0
SENSOR_TRACK
$Comp
L GND #PWR09
U 1 1 54D22F14
P 4750 5750
F 0 "#PWR09" H 4750 5500 60  0001 C CNN
F 1 "GND" H 4750 5600 60  0000 C CNN
F 2 "" H 4750 5750 60  0000 C CNN
F 3 "" H 4750 5750 60  0000 C CNN
	1    4750 5750
	1    0    0    -1  
$EndComp
$Comp
L ATTINY441-MU IC1
U 1 1 54DF0163
P 2600 2300
F 0 "IC1" H 1750 3050 40  0000 C CNN
F 1 "ATTINY441-MU" H 3300 1550 40  0000 C CNN
F 2 "Housings_DFN_QFN:QFN-20-1EP_4x4mm_Pitch0.5mm" H 2600 2100 35  0001 C CIN
F 3 "" H 2600 2300 60  0000 C CNN
	1    2600 2300
	1    0    0    -1  
$EndComp
$Comp
L SP3481EN U1
U 1 1 54DF0ADA
P 6400 2000
F 0 "U1" H 6100 2350 50  0000 L CNN
F 1 "SP3481EN" H 6500 2350 50  0000 L CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 6400 2000 50  0001 C CIN
F 3 "" H 6400 2000 60  0000 C CNN
	1    6400 2000
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 54DF0C76
P 6650 1100
F 0 "C7" H 6700 1200 50  0000 L CNN
F 1 "1u" H 6700 1000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6688 950 30  0001 C CNN
F 3 "" H 6650 1100 60  0000 C CNN
	1    6650 1100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 54DF0CCD
P 6650 1250
F 0 "#PWR010" H 6650 1000 60  0001 C CNN
F 1 "GND" H 6650 1100 60  0000 C CNN
F 2 "" H 6650 1250 60  0000 C CNN
F 3 "" H 6650 1250 60  0000 C CNN
	1    6650 1250
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR011
U 1 1 54DF0D5D
P 6650 950
F 0 "#PWR011" H 6650 800 60  0001 C CNN
F 1 "VCC" H 6650 1100 60  0000 C CNN
F 2 "" H 6650 950 60  0000 C CNN
F 3 "" H 6650 950 60  0000 C CNN
	1    6650 950 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 54DF15E6
P 6400 2400
F 0 "#PWR012" H 6400 2150 60  0001 C CNN
F 1 "GND" H 6400 2250 60  0000 C CNN
F 2 "" H 6400 2400 60  0000 C CNN
F 3 "" H 6400 2400 60  0000 C CNN
	1    6400 2400
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 54DF19BF
P 4650 2900
F 0 "R1" V 4730 2900 50  0000 C CNN
F 1 "10k" V 4657 2901 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 4580 2900 30  0001 C CNN
F 3 "" H 4650 2900 30  0000 C CNN
	1    4650 2900
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR013
U 1 1 54DF1CF6
P 4800 2900
F 0 "#PWR013" H 4800 2750 60  0001 C CNN
F 1 "VCC" H 4800 3050 60  0000 C CNN
F 2 "" H 4800 2900 60  0000 C CNN
F 3 "" H 4800 2900 60  0000 C CNN
	1    4800 2900
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 54DF1D15
P 4500 3050
F 0 "C6" H 4550 3150 50  0000 L CNN
F 1 "1u" H 4550 2950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 4538 2900 30  0001 C CNN
F 3 "" H 4500 3050 60  0000 C CNN
	1    4500 3050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 54DF1D84
P 4500 3200
F 0 "#PWR014" H 4500 2950 60  0001 C CNN
F 1 "GND" H 4500 3050 60  0000 C CNN
F 2 "" H 4500 3200 60  0000 C CNN
F 3 "" H 4500 3200 60  0000 C CNN
	1    4500 3200
	1    0    0    -1  
$EndComp
Text Label 4550 2200 2    60   ~ 0
MISO
Text Label 4550 2100 2    60   ~ 0
SCK
Text Label 4200 2900 2    60   ~ 0
RESET
Text Label 4200 2800 2    60   ~ 0
EXCITATION
Text Label 7400 3050 0    60   ~ 0
EXCITATION
Text Label 4550 2300 2    60   ~ 0
SENSE_HIGH/MOSI
Text Label 4550 2400 2    60   ~ 0
SENSE_LOW
Text Notes 2450 4900 0    60   ~ 0
Gerai patikrink kojas datasheete:\n* ar geras pinas EXCITATION signalui?\n* ar yra kokių skirtumų tarp USART1 ir USART2?\n
$Comp
L THERMISTOR TH1
U 1 1 54E05F89
P 7250 4550
F 0 "TH1" V 7350 4600 50  0000 C CNN
F 1 "NCP18XH103F03RB" V 7150 4550 50  0000 C BNN
F 2 "Resistors_SMD:R_0603" H 7250 4550 60  0001 C CNN
F 3 "" H 7250 4550 60  0000 C CNN
	1    7250 4550
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 54E06028
P 7250 4150
F 0 "R2" V 7330 4150 50  0000 C CNN
F 1 "10k 0.1%" V 7257 4151 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7180 4150 30  0001 C CNN
F 3 "" H 7250 4150 30  0000 C CNN
	1    7250 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 54E0608E
P 7250 4800
F 0 "#PWR015" H 7250 4550 60  0001 C CNN
F 1 "GND" H 7250 4650 60  0000 C CNN
F 2 "" H 7250 4800 60  0000 C CNN
F 3 "" H 7250 4800 60  0000 C CNN
	1    7250 4800
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR016
U 1 1 54E060B0
P 7250 4000
F 0 "#PWR016" H 7250 3850 60  0001 C CNN
F 1 "VCC" H 7250 4150 60  0000 C CNN
F 2 "" H 7250 4000 60  0000 C CNN
F 3 "" H 7250 4000 60  0000 C CNN
	1    7250 4000
	1    0    0    -1  
$EndComp
Text Label 6250 4300 0    60   ~ 0
THERMISTOR
Text Label 4550 2000 2    60   ~ 0
THERMISTOR
$Comp
L LED D3
U 1 1 54E06E01
P 5100 2300
F 0 "D3" H 5100 2400 50  0000 C CNN
F 1 "LED" H 5100 2200 50  0000 C CNN
F 2 "LEDs:LED-0603" H 5100 2300 60  0001 C CNN
F 3 "" H 5100 2300 60  0000 C CNN
	1    5100 2300
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 54E06FCA
P 5450 2300
F 0 "R9" V 5530 2300 50  0000 C CNN
F 1 "330" V 5457 2301 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5380 2300 30  0001 C CNN
F 3 "" H 5450 2300 30  0000 C CNN
	1    5450 2300
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR017
U 1 1 54E0711E
P 5600 2300
F 0 "#PWR017" H 5600 2050 60  0001 C CNN
F 1 "GND" H 5600 2150 60  0000 C CNN
F 2 "" H 5600 2300 60  0000 C CNN
F 3 "" H 5600 2300 60  0000 C CNN
	1    5600 2300
	1    0    0    -1  
$EndComp
Text Label 7200 1900 2    60   ~ 0
RS485A
Text Label 7200 2100 2    60   ~ 0
RS485B
$Comp
L R R10
U 1 1 54E0768F
P 7350 1900
F 0 "R10" V 7430 1900 50  0000 C CNN
F 1 "120" V 7357 1901 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 7280 1900 30  0001 C CNN
F 3 "" H 7350 1900 30  0000 C CNN
	1    7350 1900
	0    1    1    0   
$EndComp
$Comp
L JUMPER JP1
U 1 1 54E0770D
P 7500 2100
F 0 "JP1" H 7500 2250 50  0000 C CNN
F 1 "JUMPER" H 7500 2020 50  0000 C CNN
F 2 "SMD_Packages:SMD-0603_r" H 7500 2100 60  0001 C CNN
F 3 "" H 7500 2100 60  0000 C CNN
	1    7500 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 2700 5400 2700
Wire Wire Line
	5400 3100 6000 3100
Connection ~ 5700 3100
Wire Wire Line
	9300 2350 10500 2350
Wire Wire Line
	9600 2650 9300 2650
Connection ~ 9600 2350
Wire Wire Line
	9300 4100 9600 4100
Wire Wire Line
	9300 3800 10500 3800
Connection ~ 9600 3800
Wire Wire Line
	8900 2350 8900 3800
Wire Wire Line
	8500 3050 8900 3050
Connection ~ 8900 3050
Wire Wire Line
	8500 3050 8500 3450
Wire Wire Line
	6400 1600 6400 950 
Wire Wire Line
	6400 950  6650 950 
Wire Wire Line
	3650 2600 6000 2600
Wire Wire Line
	6000 2600 6000 2800
Wire Wire Line
	5400 2700 5400 2800
Wire Wire Line
	3650 2900 4500 2900
Wire Wire Line
	3650 2800 4200 2800
Wire Wire Line
	7400 3050 8200 3050
Wire Wire Line
	3650 2300 4550 2300
Wire Wire Line
	3650 2400 4550 2400
Wire Wire Line
	6800 1900 7200 1900
Wire Wire Line
	6800 2100 7200 2100
Wire Wire Line
	7250 4300 6250 4300
Wire Wire Line
	7800 1900 7800 2100
Wire Wire Line
	3650 1900 6000 1900
Wire Wire Line
	3650 1800 6000 1800
Wire Wire Line
	3650 2200 4900 2200
Connection ~ 6000 2100
Wire Wire Line
	6000 2100 6000 2200
Wire Wire Line
	6000 2100 5300 2100
Wire Wire Line
	5300 2100 5300 1700
Wire Wire Line
	5300 1700 3650 1700
Wire Wire Line
	4550 2100 3650 2100
Wire Wire Line
	4900 2200 4900 2300
Wire Wire Line
	3650 2000 4550 2000
$Comp
L CONN_02X03 P1
U 1 1 54E0D5F3
P 4500 5650
F 0 "P1" H 4500 5850 50  0000 C CNN
F 1 "CONN_02X03" H 4500 5450 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03" H 4500 4450 60  0001 C CNN
F 3 "" H 4500 4450 60  0000 C CNN
	1    4500 5650
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR018
U 1 1 54E0D755
P 4750 5550
F 0 "#PWR018" H 4750 5400 60  0001 C CNN
F 1 "VCC" H 4750 5700 60  0000 C CNN
F 2 "" H 4750 5550 60  0000 C CNN
F 3 "" H 4750 5550 60  0000 C CNN
	1    4750 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 5650 5650 5650
Text Label 5650 5650 2    60   ~ 0
SENSE_HIGH/MOSI
Wire Wire Line
	4250 5550 3850 5550
Text Label 3850 5550 0    60   ~ 0
MISO
Wire Wire Line
	4250 5650 3850 5650
Text Label 3850 5650 0    60   ~ 0
SCK
Wire Wire Line
	4250 5750 3850 5750
Text Label 3850 5750 0    60   ~ 0
RESET
$Comp
L CONN_01X04 P2
U 1 1 54E1FE0E
P 2900 5700
F 0 "P2" H 2900 5950 50  0000 C CNN
F 1 "CONN_01X04" V 3000 5700 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04" H 2900 5700 60  0001 C CNN
F 3 "" H 2900 5700 60  0000 C CNN
	1    2900 5700
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR019
U 1 1 54E1FEA9
P 2700 5550
F 0 "#PWR019" H 2700 5400 60  0001 C CNN
F 1 "VCC" H 2700 5700 60  0000 C CNN
F 2 "" H 2700 5550 60  0000 C CNN
F 3 "" H 2700 5550 60  0000 C CNN
	1    2700 5550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 54E1FECE
P 2700 5850
F 0 "#PWR020" H 2700 5600 60  0001 C CNN
F 1 "GND" H 2700 5700 60  0000 C CNN
F 2 "" H 2700 5850 60  0000 C CNN
F 3 "" H 2700 5850 60  0000 C CNN
	1    2700 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 5750 2000 5750
Text Label 2000 5750 0    60   ~ 0
RS485A
Wire Wire Line
	2700 5650 2000 5650
Text Label 2000 5650 0    60   ~ 0
RS485B
Wire Wire Line
	7500 1900 7800 1900
Connection ~ 9300 2350
$EndSCHEMATC
