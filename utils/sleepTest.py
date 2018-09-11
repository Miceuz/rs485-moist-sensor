#!/usr/bin/python

"""Puts sensor to sleep and waits for it to wakeup"""

import minimalmodbus
import serial
from time import sleep
from sys import exit

ADDRESS = 1
SLEEPTIME = 33
minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
minimalmodbus.PARITY=serial.PARITY_NONE
minimalmodbus.STOPBITS = 2
minimalmodbus.BAUDRATE=19200
#minimalmodbus.TIMEOUT=0.5

sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS)
#sensor.debug=True
print("Reading from sensor...", sensor.read_register(0, functioncode=4))
print("Going to sleep for " + str(SLEEPTIME) + " seconds")

try:
	sensor.write_register(4, value=SLEEPTIME, functioncode=6)
except ValueError:
	pass 
sleep(0.01)

sleptSeconds = 0
while True:
	try:
		sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS)
		print("Sleep done", sensor.read_register(0, functioncode=4), sensor.read_register(1, functioncode=4, numberOfDecimals=1, signed=True))
		if sleptSeconds < SLEEPTIME:
			raise Exception("Slept too little!!!")
		exit() 
	except (IOError, ValueError):
		sleep(1)
		sleptSeconds += 1
		print("Slept " + str(sleptSeconds) + " seconds")
		if sleptSeconds > SLEEPTIME:
			raise Exception("Sleeping for too long!!!")
