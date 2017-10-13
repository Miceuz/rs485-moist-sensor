#!/usr/bin/python

"""Waits for the sensor to appear on /dev/ttyUSB5, then reads moisture and temperature from it continuously"""

import minimalmodbus
import serial
from time import sleep

ADDRESS = 1
minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
minimalmodbus.PARITY=serial.PARITY_NONE
minimalmodbus.STOPBITS = 2
minimalmodbus.BAUDRATE=19200
#minimalmodbus.TIMEOUT=0.5

while True:
	try:
		sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS)
		# sensor.debug=True
		# sensor.precalculate_read_size=False
		print("FW version= " + str(hex(sensor.read_register(2, functioncode=4))) +
			  " Moisture=" + str(sensor.read_register(0, functioncode=4)) +
			  " Temperature=" + str(sensor.read_register(1, functioncode=4, numberOfDecimals=1, signed=True)))
		sleep(0.1)
	except (IOError, ValueError):
		print("Waiting...")
		sleep(0.3)
