#!/usr/bin/python

"""Waits for the sensor to appear on /dev/ttyUSB5, then reads moisture and temperature from it continuously"""
import sys
from time import sleep
from chirp_modbus import SoilMoistureSensor

SERIAL_PORT = '/dev/ttyUSB5'

print("Looking for sensors... this might take a while")
found = SoilMoistureSensor.scanBus(serialport=SERIAL_PORT, verbose=True, findOne=True)

if not found:
	print("No sensors on the bus")
	sys.exit()

sensor = SoilMoistureSensor(found[0], SERIAL_PORT)

while True:
	try:
		# sensor.debug=True
		# sensor.precalculate_read_size=False
		print("FW version= " + str(hex(sensor.getFwVersion())) +
			  " Moisture=" + str(sensor.getMoisture()) +
			  " Temperature=" + str(sensor.getTemperature()))

		print("setting address to 2")
		sensor.setAddress(2)
		sleep(0.1)
		print("new address is set: " + str(sensor.getAddress()))

		print("setting address to 1")
		sensor.setAddress(1)
		sleep(0.1)
		print("new address is set: " + str(sensor.getAddress()))

		sleep(0.1)
	except ValueError as e:
		print(e.message)
		print("Waiting...")
		sleep(0.3)
	except IOError as e:
		print(e.message)
		print("Waiting...")
		sleep(0.3)

