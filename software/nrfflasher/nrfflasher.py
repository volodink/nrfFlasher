#! /usr/bin/env python
import serial
from time import sleep
from sys import argv

def get_args():	# Small command-line parser. Yep it's squared-wheels bicycle
	if not argv[1:]: return {'filename': argv[0]}
	data = ' '.join(argv[1:])
	data = data.split(' -')
	data[0] = data[0][1:]
	data = dict([element.split(' ', 1) if ' ' in element else [element] + [True] for element in data]) # Magic
	data.update({'filename': argv[0]})
	return data

def device_search():
	for port_num in range(256):
		answer = None
		try:
			serial_port = serial.Serial(port_num, 9600, timeout=0.1)
			sleep(2.6)
			serial_port.write('T')
			answer = serial_port.read(2)
			port_name = serial_port.portstr
			serial_port.close()
		except serial.SerialException:
			pass
		if answer == 'OK':
			return port_num, port_name
	return None

def main():
	print get_args()
	exit()
	print 'Hello nrf24LE1! :)\n'
	device_port = device_search()
	if not device_port:
		print '404 Device Not Found'
		exit(1)
	print 'Device port:', device_port
	serial_port = serial.Serial(device_port[0], 9600, timeout=0.1)
	sleep(2.6)
	serial_port.write('V')
	answer = serial_port.read(15)
	serial_port.close()
	if answer[-2:] <> 'OK':
		print '502 Bad Device'
		exit(2)
	print 'Device f/w version:', answer[:-2]

if __name__ == '__main__':
	main()
