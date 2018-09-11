#!/usr/bin/python

"""Looks for sensor on the bus and changes it's address to the one specified on command line""" 

import argparse

import minimalmodbus
import serial
from time import sleep

baudrates=[1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]


parser = argparse.ArgumentParser()
parser.add_argument('baudrate', metavar='BAUD', type=int, choices=range(0, 7), help='Current baudrate')
parser.add_argument('newbaudrate', metavar='NEWBAUD', type=int, choices=range(0, 7), help='Baudrate index to set')
args = parser.parse_args()

ADDRESS1 = 1
BAUDRATE2 = args.newbaudrate

minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
minimalmodbus.PARITY=serial.PARITY_NONE
minimalmodbus.STOPBITS = 2
minimalmodbus.BAUDRATE=baudrates[args.baudrate]

minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True


def scanModbus():
	for i in range(1, 248):
		try:
			print('Trying address: ' + str(i))
			sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=i)
			addressRead = sensor.read_register(0, functioncode=3)
			if(i == addressRead):
				print('FOUND!')
				return (True, i)
		except (IOError):
			print("nope...")
			pass
	return (False, 0)
# sensor.debug=True

(found, i) = scanModbus()
if found:
	print('Found sensor at address: ' + str(i))
	try:	
		sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=i)
		print("Setting new baudrate: " + str(baudrates[BAUDRATE2]))
		sensor.write_register(1, value=BAUDRATE2, functioncode=6)
		minimalmodbus._SERIALPORTS={}
		minimalmodbus.BAUDRATE=baudrates[BAUDRATE2]

		sleep(0.2)
		sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=i)
		print("reading address from holding register: ")
		print(sensor.read_register(0, functioncode=3))
	except:
		print "Could not change the address. Check your connections"
else:
	print('No sensor on the bus found')