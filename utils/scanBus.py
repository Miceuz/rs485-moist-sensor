#!/usr/bin/python -u
from chirp_modbus import SoilMoistureSensor
SERIAL_PORT = '/dev/ttyUSB5'
found = SoilMoistureSensor.scanBus(serialport=SERIAL_PORT, findOne=False, verbose=True)
if found:
	print("Found sensors: " + str(found))