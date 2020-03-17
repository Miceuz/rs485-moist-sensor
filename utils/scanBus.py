#!/usr/bin/python -u
from chirp_modbus import SoilMoistureSensor
import serial
import minimalmodbus

SERIAL_PORT = '/dev/ttyUSB5'
DEFAULT_BAUDRATE = 19200
DEFAULT_PARITY = serial.PARITY_NONE
DEFAULT_STOPBITS = 2

def scanBus(serialport, startAddress = 1, endAddress = 247, verbose=False, findOne=False, serialbaudrate=DEFAULT_BAUDRATE, serialparity=DEFAULT_PARITY, serialstopbits=DEFAULT_STOPBITS):
	addresses=[]
	if verbose:
		print("Scanning bus from " +str(startAddress) + " to " + str(endAddress))
	for i in range(startAddress, endAddress+1):
		try:
			if verbose:
				print('Trying address: ' + str(i))

			minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
			minimalmodbus.PARITY=serial.PARITY_NONE
			minimalmodbus.STOPBITS = serialstopbits
			minimalmodbus.BAUDRATE = serialbaudrate
			
			sensor = minimalmodbus.Instrument(serialport, slaveaddress=i)
			sensor.serial.baudrate = serialbaudrate
			sensor.serial.stopbits = serialstopbits
			sensor.close_port_after_each_call = True

			addressRead = sensor.read_register(0, functioncode=3)
			if(i == addressRead):
				addresses.append(i)
				if verbose:
					print('FOUND!')
				if findOne:
					return addresses
		except (IOError):
			if verbose:
				print("nope...")
			pass
	return addresses

while True:
	found = scanBus(serialport=SERIAL_PORT, endAddress = 80, findOne=False, verbose=False, serialbaudrate=19200, serialstopbits=2)
	if found:
		print("Found " + str(len(found)) + " sensors: " + str(found))

	# while True:
	# 	for a in found:
	# 		s= SoilMoistureSensor(address = a, serialport = SERIAL_PORT)
	# 		print (str(a) + ": " + str(s.getMoisture()) + " " + str(s.getTemperature()))
