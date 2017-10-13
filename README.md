# rs485 soil moisture sensor
Digital (RS485 - based) soil moisture and temperature sensor 

A version of [I2C soil moisture sensor](https://github.com/Miceuz/i2c-moisture-sensor) with several updates - antistatic protection, local voltage regulation and RS485 networking.

![](rs485-moist-sensor.png)

This is a contact-less capacitive soil moisture sensor, a continuation of my [I2C soil moisture sensor](https://github.com/Miceuz/i2c-moisture-sensor). This particular version of the sensor is suited for more professional installations where long cable runs is a requirement. It adds a local voltage regulator to mitigate the voltage drop and RS485 transceiver to accommodate long wires, also reverse polarity and an additional ESD protection level is added. 

This is the easiest and the most robust way to read soil moisture and temperature values into Raspberry Pi. I have written a [tutorial](https://github.com/Miceuz/rs485-moist-sensor/tree/master/tutorial) how to do it.

The main features:

 * Capacitive soil moisture sensing
 * Temperature sensing
 * Good stability with temperature
 * Good stability with power variation
 * Suitable for long cable runs
 * Uses industrial Modbus protocol

# Python library

A python library is available via pip. Type `pip install chirp_modbus` to install it. Check the [source code](https://github.com/Miceuz/rs485-moist-sensor/blob/master/utils/lib/chirp_modbus.py) for documentation. Usage example:
```python
from chirp_modbus import SoilMoistureSensor
s = SoilMoistureSensor(1, '/dev/ttyUSB5')
print(s.getTemperature())
print(s.getMoisture())
```


# Technical data

 * Supply voltage - 5V
 * Bus can be run on both 3V and 5V
 * Default serial settings: 19200, 8-bit frame, no parity, 2 stop bits
 * Default Modbus address: 1
 * Current consumption: Active: 9mA, Deep sleep: 11uA

## Principle of operation

Sensor makes periodic measurements (by default, once every 500ms), delays between measurements can be updated by writing to one of the Modbus registers. The results of the measurements are kept in specific Modbus registers and can be read by master any time unless the sensor is in deep sleep. 

Deep sleep is entered by writing a number of seconds to sleep into a special sleep register. The sensor will put the transceiver and all the periphery into the lowest possible power consumption mode and sleep for the specified amoutn of time. After the sleep the sensor will wake up, start taking measurements and be available on the bus till the next sleep command.

Serial communication parameters as baud rate, parity and address can be updated by writing to related registers. The sensor will reset and boot with the new parameters if any of those registers are written.

### Input (read only) registers

Register number | Size (bytes) | Description
----------------|------------- |-------------
       0        |      2       | Soil moisture. Unsigned
       1        |      2       | Temperature. Signed. In tenths of degrees Celsius. I.e. 220 means 22.0C

### Holding (read/write) registers

Register number | Size (bytes) | Valid values | Default value | Description
----------------|--------------|------------- |---------------|-------------
        0       |       2      |  [1 - 247]   |      1        | Modbus address
        1       |       2      |  [0 - 7]     |      4        | Baud rate
        2       |       2      |  [0 - 2]     |      0        | Parity **Note: most cheap ebay USB to RS485 dongles don't support parity properly!** 
        3       |       2      |  [1 - 65535] |     500       | Measurement interval in milliseconds
        4       |       2      |  [1 - 65535] |      0        | Time to sleep in seconds. Write to this register to put the sensor to sleep.

#### Supported baudrates

 Number | Baudrate
--------|---------
   0    |  1200
   1    |  2400
   2    |  4800
   3    |  9600
   4    |  19200
   5    |  38400
   6    |  57600
   7    |  115200

#### Supported parity settings

 Number | Parity
--------|---------
   0    |  None
   1    |  Even
   2    |  Odd

**Note: most of the cheap ebay USB to RS485 dongles and/or their drivers do not support parity setting properly! You will have to use a proper parity supporting adapter to unbrick the sensor if you set this to something that your dongle does not support!** Luckily Adruino with ATmega 16u2 can be used as USB to Serial adapter to solve this.

## How to interpret the readings

Moisture values are relative. Meaning, more moisture will give you higher reading, less moisture - lower reading. It's up to you to calibrate it to amount of water in the soil as readings will be different for different types of soils. 

Moisture is somewhat linear. I test all sensors before shipping and they give about 292 in free air. 

Temperature is measured by the thermistor on the body of the sensor. Calculated absolute measurement accuracy is better than 2%. The returned value is in tenths of degrees Celsius. I.e. value 252 would mean 25.2°C.

## Controller support

The sensor will work fine with any controller supporting serial communication provided you have an RS485 transceiver. There are a lot of Modbus libraries readily available for a variety of micro-processor families. Thea easiest way to try the sensor is to use one of the abundant and cheap USB to RS485 dongles and connect it to a Raspberry pi or even directly to your computer. 

I have written some basic test code in python that is available on [github](https://github.com/Miceuz/i2c-moisture-sensor/blob/master/README.md) page. 

## Waterproofing

The sensor comes coated with PRF202 - a moisture resistant varnish for electronics. It's ok for play around in a flower pot but not enough for outdoor use. **You must add an additional protection to the whole sensor after soldering cable to it!** Some suggestions on making the sensor more robust after attaching the cable:

 * Polyester or epoxy resin - this method is the most bullet proof as the resin is totally resistant to the water. On the par side, note that sensitivity of the sensor will decrease depending on how thick the layer you are going to apply. Also applying the resin in uniform manner presents some challenge.
 * PlastiDip - some of my customers have tried this method - easy to apply by spraying and not a lot of loss of sensitivity.
 * Rubber balloon - yes, just roll a long balloon over the sensor :)  

Be sure to coat the whole thing - the sensor part, the electronics and the cable connection itself so no bare copper or solder is accessible to the water.

