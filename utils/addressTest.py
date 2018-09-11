#!/usr/bin/python

"""Looks for sensor with ADDRESS1 and changes it's address to ADDRESS2 then changes it back to ADDRESS1""" 

import minimalmodbus
import serial
from time import sleep
ADDRESS1 = 1
ADDRESS2 = 2

minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
minimalmodbus.PARITY=serial.PARITY_NONE
minimalmodbus.STOPBITS = 2
minimalmodbus.BAUDRATE=19200

minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS1)
sensor.debug=True
print(("reading address: ") + str(sensor.read_register(0, functioncode=3)))
print("writing new address: " + str(ADDRESS2))
sensor.write_register(0, value=ADDRESS2, functioncode=6)

sleep(0.2)
sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS2)
print("reading address from holding register: ")
print(sensor.read_register(0, functioncode=3))

print("writing new address: " + str(ADDRESS1))
sensor.write_register(0, value=ADDRESS1, functioncode=6)

sleep(0.2)
sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS1)
print("reading address from holding register: ")
print(sensor.read_register(0, functioncode=3))
