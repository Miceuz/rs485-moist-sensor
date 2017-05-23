#!/usr/bin/python
import minimalmodbus
from time import sleep
ADDRESS = 1
minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS)
#print("writing")
#sensor.write_register(4, value=0, functioncode=6)
#print("reading holding")
#print(sensor.read_register(4, functioncode=3))

while True:
	print(sensor.read_register(0, functioncode=4))
	print(sensor.read_register(1, functioncode=4, numberOfDecimals=1, signed=True))
	sleep(0.1)