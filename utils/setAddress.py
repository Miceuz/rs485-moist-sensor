#!/usr/bin/python

"""Looks for sensor on the bus and changes it's address to the one specified on command line""" 

import argparse

from chirp_modbus import SoilMoistureSensor
import serial
import sys
from time import sleep

parser = argparse.ArgumentParser()
parser.add_argument('serialport', metavar='SERIAL', type=str, help='Serial port')
parser.add_argument('newaddress', metavar='NEWADDR', type=int, choices=range(1, 248), help='An address to set')
parser.add_argument('--baudrate', metavar='BAUD', type=int, default=4, choices=range(0, 7), help='Current baudrate index - [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]')
args = parser.parse_args()

baudrates=[1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]

SERIAL_PORT=args.serialport
BAUDRATE = baudrates[args.baudrate]
ADDRESS2 = args.newaddress


found = SoilMoistureSensor.scanBus(serialport=args.serialport, endAddress = 247, findOne=True, verbose=False, serialbaudrate=BAUDRATE, serialstopbits=2)

if found:
	print('Found sensor at address: ' + str(found[0]))
	try:	
		sensor = SoilMoistureSensor(address = found[0], serialport=SERIAL_PORT, serialbaudrate=BAUDRATE)
		print("writing new address: " + str(ADDRESS2))
		sensor.setAddress(ADDRESS2)

		sleep(0.2)
		sensor = SoilMoistureSensor(address = ADDRESS2, serialport=SERIAL_PORT, serialbaudrate=BAUDRATE)
		print("reading address from holding register: ")
		print(sensor.getAddress())
	except Exception as e:
		print (e)
		print ("Could not change the address. Check your connections")
else:
	print('No sensor on the bus found')
