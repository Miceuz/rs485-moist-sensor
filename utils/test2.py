#!/usr/bin/python
import minimalmodbus
import serial
from time import sleep

ADDRESS = 1
minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
minimalmodbus.PARITY=serial.PARITY_NONE
minimalmodbus.STOPBITS = 2
minimalmodbus.BAUDRATE=19200
minimalmodbus.TIMEOUT=0.5

#print("writing")
#sensor.write_register(4, value=0, functioncode=6)
#print("reading holding")
# print(sensor.read_register(4, functioncode=3))
# print(sensor.read_register(0, functioncode=4))
# print(sensor.read_register(1, functioncode=4))

while True:
	try:
		sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS)
		sensor.debug=True
		print(sensor.read_register(0, functioncode=4), sensor.read_register(1, functioncode=4, numberOfDecimals=1, signed=True))
		sleep(0.1)
	except (IOError, ValueError):
		print("Waiting...")
		sleep(0.3)
