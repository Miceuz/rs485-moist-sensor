#!/usr/bin/python -u
from chirp_modbus import SoilMoistureSensor
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('serialport', metavar='SERIAL', type=str, help='Serial port')
parser.add_argument('--baudrate', metavar='BAUD', type=int, default=4, choices=range(0, 7), help='Current baudrate index - [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]')
args = parser.parse_args()

baudrates=[1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]

SERIAL_PORT = args.serialport
BAUDRATE = baudrates[args.baudrate]

print("Scanning bus on port " + SERIAL_PORT + " at baud rate " + str(BAUDRATE))

while True:
	found = SoilMoistureSensor.scanBus(serialport=SERIAL_PORT, endAddress = 80, findOne=False, verbose=False, serialbaudrate=BAUDRATE)
	if found:
		print("Found " + str(len(found)) + " sensors: " + str(found))

	# while True:
	# 	for a in found:
	# 		s= SoilMoistureSensor(address = a, serialport = SERIAL_PORT)
	# 		print (str(a) + ": " + str(s.getMoisture()) + " " + str(s.getTemperature()))
