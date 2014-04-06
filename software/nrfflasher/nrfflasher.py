#! /usr/bin/env python

import serial
from time import sleep

print "Hello nrf24LE1! :)\n"


nrfFlasherPort = serial.Serial('COM3', 115200, timeout=1)
sleep(.01)



