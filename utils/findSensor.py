#!/usr/bin/python -u
from chirp_modbus import SoilMoistureSensor
import serial
import minimalmodbus

import argparse

parser = argparse.ArgumentParser()
parser.add_argument('serialport', metavar='SERIAL', type=str, help='Serial port')
parser.add_argument('--baudrate', metavar='BAUD', type=int, default=4, choices=range(0, 7), help='Current baudrate index - [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]')
args = parser.parse_args()

SERIAL_PORT = args.serialport

baudrates=[19200, 9600, 1200, 2400, 4800, 38400, 57600, 115200]

print("Scanning bus on port " + SERIAL_PORT)

for baudrate in baudrates:
	print("Trying baudrate " + str(baudrate))

	found = SoilMoistureSensor.scanBus(serialport=SERIAL_PORT, endAddress = 247, findOne=True, verbose=False, serialbaudrate=baudrate)
	if found:
		print("Found " + str(len(found)) + " sensors: " + str(found))
		break

	# while True:
	# 	for a in found:
	# 		s= SoilMoistureSensor(address = a, serialport = SERIAL_PORT)
	# 		print (str(a) + ": " + str(s.getMoisture()) + " " + str(s.getTemperature()))

