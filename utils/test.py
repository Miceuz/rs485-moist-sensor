#!/usr/bin/python

"""Waits for the sensor to appear on /dev/ttyUSB5, then reads moisture and temperature from it continuously"""

import chirp_modbus
from time import sleep
import minimalmodbus

ADDRESS = 1
minimalmodbus.TIMEOUT=0.5

# for baudrate in chirp_modbus.SoilMoistureSensor.baudrates:
# 	print("Checking baudrate " + str(baudrate))
# 	minimalmodbus.BAUDRATE = baudrate
# 	chirp_modbus.SoilMoistureSensor.scanBus(verbose=True, findOne=True)

while True:
	try:
		sensor = chirp_modbus.SoilMoistureSensor(address=ADDRESS, serialport='/dev/ttyUSB5')

		# sensor.sensor.debug=True
		sensor.sensor.precalculate_read_size=False
		print(
			  " Moisture=" + str(sensor.getMoisture()) +
			  " Temperature=" + str(sensor.getTemperature()))
		sleep(1)
	except ValueError as e:
		print e
		print("Waiting...")
		sleep(0.3)
	except IOError as e:
		print e
		print("Waiting...")
		sleep(0.3)
