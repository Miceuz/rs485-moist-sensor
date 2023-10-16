#!/usr/bin/python -u

"""Address setting utility for when you want to set up a lot of sensors"""

from time import sleep
from chirp_modbus import SoilMoistureSensor
import sys

SERIAL_PORT = '/dev/ttyUSB1'

found = False
while not found:
	print("Scanning bus...")
	found = SoilMoistureSensor.scanBus(serialport=SERIAL_PORT, findOne=True)
	if not found:
		print("No sensors found on bus.")

print("Found sensor at address " + str(found[0]))

program_address = 1

while True:
	print("Set address to %d? [Y]es / [n]o / [r]escan bus)" % program_address)
	c = input()

	if c == 'y' or c == '' or 'r' == c:
		found = False
		while not found:
			print("Scanning bus...")
			found = SoilMoistureSensor.scanBus(serialport=SERIAL_PORT, findOne=True)
			if not found:
				print("No sensors found on bus.")

	print("Found sensor at address " + str(found[0]))

	if 'y' == c or c == '':
		print("Setting address %d" % program_address)
		sensor = SoilMoistureSensor(found[0], SERIAL_PORT)
		try:
			sensor.setAddress(program_address)
		except:
			pass 

	if 'y' == c or c == '' or 'n' == c:
		program_address = program_address + 1

	if program_address > 247:
		program_address = 1

