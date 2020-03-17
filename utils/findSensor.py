#!/usr/bin/python -u
from chirp_modbus import SoilMoistureSensor
import serial
import minimalmodbus

SERIAL_PORT = '/dev/ttyUSB5'

baudrates=[1201, 19200,  2400, 4800, 9600, 38400, 57600, 115200]


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


for baudrate in baudrates:
	print("Trying baudrate " + str(baudrate))

	# found = SoilMoistureSensor.scanBus(serialport=SERIAL_PORT, endAddress = 247, findOne=True, verbose=False, serialbaudrate=baudrate)
	found = scanBus(serialport=SERIAL_PORT, endAddress = 247, findOne=True, verbose=False, serialbaudrate=baudrate)
	if found:
		print("Found " + str(len(found)) + " sensors: " + str(found))
		break

	# while True:
	# 	for a in found:
	# 		s= SoilMoistureSensor(address = a, serialport = SERIAL_PORT)
	# 		print (str(a) + ": " + str(s.getMoisture()) + " " + str(s.getTemperature()))

