import minimalmodbus
import serial

class SoilMoistureSensor:

	VERSION = '1.0.1'
	DEFAULT_BAUDRATE = 19200
	DEFAULT_PARITY = serial.PARITY_NONE
	DEFAULT_STOPBITS = 2
	baudrates=[1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200]
	parities=[serial.PARITY_NONE, serial.PARITY_EVEN, serial.PARITY_ODD]

	def __initSensor(self):
		self.sensor = minimalmodbus.Instrument(self.serialport, slaveaddress=self.address)

	def __init__(self, address, serialport, serialbaudrate=DEFAULT_BAUDRATE, serialparity=DEFAULT_PARITY, serialstopbits=DEFAULT_STOPBITS):
		minimalmodbus.CLOSE_PORT_AFTER_EACH_CALL = True
		minimalmodbus.PARITY=serial.PARITY_NONE
		minimalmodbus.STOPBITS = serialstopbits
		minimalmodbus.BAUDRATE=serialbaudrate
		self.address = address
		self.serialport = serialport
		self.__initSensor()

	def getMoisture(self):
		return self.sensor.read_register(0, functioncode=4)

	def getTemperature(self):
		return self.sensor.read_register(1, functioncode=4, numberOfDecimals=1, signed=True)

	def getFwVersion(self):
		return self.sensor.read_register(2, functioncode=4)

	def getAddress(self):
		return self.sensor.read_register(0, functioncode=3)

	def getBaudrate(self):
		return self.sensor.read_register(1, functioncode=3)

	def getParity(self):
		return self.sensor.read_register(2, functioncode=3)

	def getMeasurementInterval(self):
		return self.sensor.read_register(3, functioncode=3)

	def setAddress(self, address):
		if(address <= 0 or address > 247):
			raise ValueError("Address has to be [1..247]")

		self.sensor.write_register(0, value=address, functioncode=6)

		self.address = address
		self.__initSensor()

	def setBaudrate(self, baudrate):
		if(baudrate < 0 or baudrate > 7):
			raise ValueError("Baudrate index has to be [0..7]")

		self.sensor.write_register(1, value=baudrate, functioncode=6)

		minimalmodbus._SERIALPORTS={}
		minimalmodbus.BAUDRATE = self.baudrates[baudrate]
		self.__initSensor()

	def setParity(self, parity):
		if parity < 0 or parity > 2:
			raise ValueError("Parity index has to be [0..2]")

		self.sensor.write_register(2, value=parity, functioncode=6)

		minimalmodbus._SERIALPORTS={}
		minimalmodbus.PARITY = parities[parity]		
		self.__initSensor();

	def setMeasurementInterval(self, interval):
		if interval <= 0 or interval > 2**16-1:
			raise ValueError("Interval has to be [1..65535")
		self.sensor.write_register(3, value=interval, functioncode=6)

	def sleep(self, seconds):
		if seconds <= 0 or seconds > 2**16-1:
			raise ValueError("Sleep time has to be in [1..65535]")
		self.sensor.write_register(4, value=seconds, functioncode=6)
	
	@staticmethod
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
