#!/usr/bin/python

"""Waits for the sensor to appear on /dev/ttyUSB5, then reads moisture and temperature from it continuously"""

import minimalmodbus
import chirp_modbus
from time import sleep

ADDRESS = 1
#minimalmodbus.TIMEOUT=0.5

while True:
	try:
		sensor = chirp_modbus.SoilMoistureSensor(address=ADDRESS, serialport='/dev/ttyUSB5')
		sensor.sensor.debug=True
		# sensor.sensor.precalculate_read_size=False
		#sensor.sensor.precalculate_read_size=False
		print(
			  " Moisture=" + str(sensor.getMoisture()) +
			  " Temperature=" + str(sensor.getTemperature()))
		sleep(0.1)
	except (IOError, ValueError):
		print("Waiting...")
		sleep(0.3)
