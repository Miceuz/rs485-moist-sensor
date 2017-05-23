#!/usr/bin/python
import minimalmodbus
from time import sleep
ADDRESS1 = 1
ADDRESS2 = 2

minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
sensor = minimalmodbus.Instrument('/dev/ttyUSB5', slaveaddress=ADDRESS1)
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
