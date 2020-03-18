#!/usr/bin/python

"""Waits for the sensor to appear on /dev/ttyUSB5, then reads moisture and temperature from it continuously"""

from chirp_modbus import SoilMoistureSensor
from time import sleep
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('serialport', metavar='SERIAL', type=str, help='Serial port')
parser.add_argument('--address', metavar='ADDR', type=int, default=1, choices=range(1, 248), help='An address to set')
parser.add_argument('--baudrate', metavar='BAUD', type=int, default=4, choices=range(0, 7), help='Current baudrate index - [1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]')
args = parser.parse_args()

baudrates=[1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]

ADDRESS = args.address
SERIAL_PORT = args.serialport
BAUDRATE = baudrates[args.baudrate]

print("Reading from sensor address " + str(ADDRESS) + " via port " + SERIAL_PORT + " at baud rate " + str(BAUDRATE))
while True:
	try:
		sensor = SoilMoistureSensor(address = ADDRESS, serialport = SERIAL_PORT, serialbaudrate = BAUDRATE)

		# sensor.sensor.debug=True
		sensor.sensor.precalculate_read_size=False
		sensor.sensor.close_port_after_each_call = True
		#sensor.sensor.serial.timeout = 0.5

		print(
			  " Moisture=" + str(sensor.getMoisture()) +
			  " Temperature=" + str(sensor.getTemperature()))
		sleep(0.3)
	except ValueError as e:
		print e
		print("Waiting...")
		sleep(0.3)
	except IOError as e:
		print e
		print("Waiting...")
		sleep(0.3)
