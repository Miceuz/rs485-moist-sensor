EESchema Schematic File Version 5
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 "Albertas Mickenas"
Comment2 "albertas@technarium.lt"
Comment3 ""
Comment4 ""
Comment5 ""
Comment6 ""
Comment7 ""
Comment8 ""
Comment9 ""
$EndDescr
Connection ~ 4175 4400
Connection ~ 4525 4600
Connection ~ 4625 3975
Connection ~ 4625 4300
Wire Wire Line
	2750 4100 3775 4100
Wire Wire Line
	2750 4200 3775 4200
Wire Wire Line
	2750 4700 5275 4700
Wire Wire Line
	2750 4800 5375 4800
Wire Wire Line
	3775 4100 3775 3975
Wire Wire Line
	3975 3075 5200 3075
Wire Wire Line
	4175 3975 4625 3975
Wire Wire Line
	4175 4200 4175 4400
Wire Wire Line
	4175 4400 2750 4400
Wire Wire Line
	4525 4400 4175 4400
Wire Wire Line
	4525 4400 4525 4600
Wire Wire Line
	4525 4600 2750 4600
Wire Wire Line
	4525 4600 4525 4875
Wire Wire Line
	4625 3975 4625 4300
Wire Wire Line
	4625 4300 2750 4300
Wire Wire Line
	4625 4300 4625 4500
Wire Wire Line
	4625 4300 5050 4300
Wire Wire Line
	4625 4500 2750 4500
Wire Wire Line
	5050 4300 5050 3800
Wire Wire Line
	5200 3075 5200 3550
Wire Wire Line
	5200 4050 5750 4050
Wire Wire Line
	5275 4350 5850 4350
Wire Wire Line
	5275 4700 5275 4350
Wire Wire Line
	5375 4450 5850 4450
Wire Wire Line
	5375 4800 5375 4450
Wire Wire Line
	5650 4250 5650 4550
Wire Wire Line
	5750 4050 5750 4550
Wire Wire Line
	5750 4550 5850 4550
Wire Wire Line
	5850 4250 5650 4250
Text Notes 2925 4300 2    50   ~ 0
O
Text Notes 2925 4400 2    50   ~ 0
BW
Text Notes 2925 4500 2    50   ~ 0
B
Text Notes 2925 4600 2    50   ~ 0
OW
Text Notes 2925 4700 2    50   ~ 0
G
Text Notes 2950 4100 2    50   ~ 0
BR
Text Notes 2950 4200 2    50   ~ 0
BRW
Text Notes 2950 4800 2    50   ~ 0
GW
Text Label 3125 4300 2    50   ~ 0
VCC
Text Label 3125 4400 2    50   ~ 0
GND
Text Label 3125 4500 2    50   ~ 0
VCC
Text Label 3125 4600 2    50   ~ 0
GND
Text Label 3125 4700 2    50   ~ 0
B
Text Label 3125 4800 2    50   ~ 0
A
Text Label 3250 4100 2    50   ~ 0
B{slash}VCC
Text Label 3250 4200 2    50   ~ 0
A{slash}GND
Text Label 5550 4350 2    50   ~ 0
B
Text Label 5550 4450 2    50   ~ 0
A
$Comp
L power:VCC #PWR?
U 1 1 5E309DAF
P 4625 3975
F 0 "#PWR?" H 4625 3825 50  0001 C CNN
F 1 "VCC" H 4640 4148 50  0000 C CNN
F 2 "" H 4625 3975 50  0001 C CNN
F 3 "" H 4625 3975 50  0001 C CNN
	1    4625 3975
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E310096
P 3975 3175
F 0 "#PWR?" H 3975 2925 50  0001 C CNN
F 1 "GND" H 3980 3002 50  0000 C CNN
F 2 "" H 3975 3175 50  0001 C CNN
F 3 "" H 3975 3175 50  0001 C CNN
	1    3975 3175
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E30A136
P 4525 4875
F 0 "#PWR?" H 4525 4625 50  0001 C CNN
F 1 "GND" H 4530 4702 50  0000 C CNN
F 2 "" H 4525 4875 50  0001 C CNN
F 3 "" H 4525 4875 50  0001 C CNN
	1    4525 4875
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E30BC3A
P 5650 4550
F 0 "#PWR?" H 5650 4300 50  0001 C CNN
F 1 "GND" H 5655 4377 50  0000 C CNN
F 2 "" H 5650 4550 50  0001 C CNN
F 3 "" H 5650 4550 50  0001 C CNN
	1    5650 4550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5E30CE36
P 3775 3075
F 0 "J3" H 3883 3254 50  0000 C CNN
F 1 "Conn_01x02_Male" H 3883 3163 50  0000 C CNN
F 2 "TerminalBlock_RND:TerminalBlock_RND_205-00287_Pitch5.08mm" H 3775 3075 50  0001 C CNN
F 3 "~" H 3775 3075 50  0001 C CNN
	1    3775 3075
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP1
U 1 1 5E3170CB
P 3975 3975
F 0 "JP1" H 3975 4100 50  0000 C CNN
F 1 "Jumper_2_Open" H 3975 4118 50  0001 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 3975 3975 50  0001 C CNN
F 3 "~" H 3975 3975 50  0001 C CNN
	1    3975 3975
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP2
U 1 1 5E319375
P 3975 4200
F 0 "JP2" H 3975 4325 50  0000 C CNN
F 1 "Jumper_2_Open" H 3975 4343 50  0001 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 3975 4200 50  0001 C CNN
F 3 "~" H 3975 4200 50  0001 C CNN
	1    3975 4200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 5E30A589
P 6050 4450
F 0 "J1" H 6022 4333 50  0000 R CNN
F 1 "Conn_01x04_Male" H 6022 4423 50  0000 R CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04_Pitch2.54mm" H 6050 4450 50  0001 C CNN
F 3 "~" H 6050 4450 50  0001 C CNN
	1    6050 4450
	-1   0    0    1   
$EndComp
$Comp
L Jumper:Jumper_3_Bridged12 JP3
U 1 1 5E312537
P 5200 3800
F 0 "JP3" V 5200 3925 50  0000 C CNN
F 1 "Jumper_3_Bridged12" H 5200 3912 50  0001 C CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged12_RoundedPad1.0x1.5mm_NumberLabels" H 5200 3800 50  0001 C CNN
F 3 "~" H 5200 3800 50  0001 C CNN
	1    5200 3800
	0    1    -1   0   
$EndComp
$Comp
L Connector:RJ45 J2
U 1 1 5E263AF8
P 2350 4500
F 0 "J2" H 2407 5166 50  0000 C CNN
F 1 "RJ45" H 2407 5075 50  0000 C CNN
F 2 "Connector_RJ:RJ45_Amphenol_54602-x08_Horizontal" V 2350 4525 50  0001 C CNN
F 3 "~" V 2350 4525 50  0001 C CNN
	1    2350 4500
	1    0    0    -1  
$EndComp
$EndSCHEMATC
