#! /usr/bin/env python
from re import search

import serial
from time import sleep

print "Hello nrf24LE1! :)\n"


serialPort = serial.Serial('COM10', 9600, timeout=1)
sleep(2)
serialPort.write('T')
print serialPort.read(100)
sleep(.1)

serialPort.write('T')
print serialPort.read(100)
sleep(.1)

serialPort.write('T')
print serialPort.read(100)
sleep(.1)

serialPort.write('T')
print serialPort.read(100)
sleep(.1)


