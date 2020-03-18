#!/usr/bin/python

"""Looks for sensor on the bus and changes it's address to the one specified on command line""" 

import argparse
import serial
from time import sleep

from chirp_modbus import SoilMoistureSensor
baudrates=[1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]


parser = argparse.ArgumentParser()
parser.add_argument('serialport', metavar='SERIAL', type=str, help='Serial port')
parser.add_argument('baudrate', metavar='BAUD', type=int, choices=range(0, 7), help='Current baudrate index - [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]')
parser.add_argument('newbaudrate', metavar='NEWBAUD', type=int, choices=range(0, 7), help='Baudrate index to set - [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]')
args = parser.parse_args()

ADDRESS1 = 1
BAUDRATE1 = baudrates[args.baudrate]
BAUDRATE2 = args.newbaudrate

found = SoilMoistureSensor.scanBus(serialport=args.serialport, endAddress = 247, findOne=True, verbose=False, serialbaudrate=BAUDRATE1, serialstopbits=2)
if found:
	print('Found sensor at address: ' + str(found[0]))
	try:	
		sensor = SoilMoistureSensor(address = found[0], serialport=args.serialport, serialbaudrate=BAUDRATE1)
		print("Setting new baudrate: " + str(baudrates[BAUDRATE2]))
		sensor.setBaudrate(BAUDRATE2)

		sleep(0.2)
		sensor = SoilMoistureSensor(address = found[0], serialport=args.serialport, serialbaudrate=baudrates[BAUDRATE2])
		print("reading address from holding register via new baudrate: ")
		print(sensor.getMoisture())
	except Exception as e:
		print e
		print "Could not change the address. Check your connections"
else:
	print('No sensor found on the bus')