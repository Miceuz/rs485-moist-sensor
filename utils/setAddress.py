#!/usr/bin/python

"""Looks for sensor on the bus and changes it's address to the one specified on command line""" 

import argparse

import minimalmodbus
import serial
import sys
from time import sleep


parser = argparse.ArgumentParser()
parser.add_argument('address', metavar='ADDR', type=int, choices=range(1, 248), help='An address to set')
args = parser.parse_args()

SERIAL_PORT='/dev/ttyUSB5'

ADDRESS1 = 1
ADDRESS2 = args.address

minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
minimalmodbus.PARITY=serial.PARITY_NONE
minimalmodbus.STOPBITS = 2
minimalmodbus.BAUDRATE=19200

minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True

def scanModbus():
	for i in range(1, 248):
		try:
			sys.stdout.write('Trying address: %d \r' % i)
			sys.stdout.flush()
			sensor = minimalmodbus.Instrument(SERIAL_PORT, slaveaddress=i)
			sensor.serial.timeout = 0.013
			addressRead = sensor.read_register(0, functioncode=3)
			if(i == addressRead):
				return (True, i)
		except (IOError):
			# print("nope...")
			pass
	return (False, 0)
# sensor.debug=True

(found, i) = scanModbus()
if found:
	print('Found sensor at address: ' + str(i))
	try:	
		sensor = minimalmodbus.Instrument(SERIAL_PORT, slaveaddress=i)
		print("writing new address: " + str(ADDRESS2))
		sensor.write_register(0, value=ADDRESS2, functioncode=6)

		sleep(0.2)
		sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS2)
		print("reading address from holding register: ")
		print(sensor.read_register(0, functioncode=3))
	except:
		print "Could not change the address. Check your connections"
else:
	print('No sensor on the bus found')
