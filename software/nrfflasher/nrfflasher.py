#! /usr/bin/env python
import serial
from time import sleep
from sys import argv
import os

open_port_delay = 1.7

def get_args(): # Small command-line parser. Yep it's squared-wheels bicycle
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
			serial_port = serial.Serial(port_num, 9600, timeout=0.3)
			sleep(open_port_delay)
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
	print 'Hello nrf24LE1! :)'
	start_args = get_args()
	if 'verbose' in start_args: debug = True
	else: debug = False
	if ('h' in start_args) or (not (('w' in start_args) or ('e' in start_args) or ('r' in start_args) or ('v' in start_args))): # Help
		print "Don't wait for help."
		exit(0)
	if ((('w' in start_args) and ('r' in start_args)) or
		(('w' in start_args) and ('e' in start_args)) or
		(('r' in start_args) and ('e' in start_args))):
		print 'What the fuck are you doing?!'
		exit(666)
	device_port = device_search()
	if not device_port:
		print '404 Device Not Found'
		exit(1)
	if debug: print 'Device port:', device_port

	serial_port = serial.Serial(device_port[0], 9600, timeout=0.3) # Open port for random actions
	sleep(open_port_delay)
	serial_port.write('V')
	answer = serial_port.read(15)
	if answer[-2:] <> 'OK':
		print '502 Bad Device'
		exit(2)
	if debug: print 'Device f/w version:', answer[:-2]
	if debug: print start_args	# e - erase; w - write; r - read;

	if 'e' in start_args: # Erase
		print 'Erasing...'
		serial_port.write('E')
		#serial_port.timeout = 5 # Fuck!
		sleep(5)
		answer = serial_port.read(2)
		if answer == 'OK':
			print 'Yahoo!'
			exit(0)
		else:
			print 'Erasing error!'
			exit(3)

	if 'r' in start_args: # Read
		print 'Reading...'
		if not 'file' in start_args:
			print 'Get the file!'
			exit(5)
		serial_port.write('R')
		answer = ''
		for i in range(512 * 16):
			answer += serial_port.read(32)
			if answer[-2:] == 'OK': break
		if answer[-2:] <> 'OK':
			print 'Reading error!'
			exit(4)
		answer = answer[:-2]
		for i in answer:
			if answer[-1] == chr(255): answer = answer[:-1]
		print 'Reading complite!\nf/w size: %i bytes' % len(answer)
		open(start_args['file'], 'wb').write(answer)

	if 'w' in start_args: # Write
		print 'Writing...'
		if not 'file' in start_args:
			print 'Get the file!'
			exit(5)
		if start_args['file'][-4:] == '.hex':
			os.system('hex2bin.exe %s' % start_args['file'])
			data = open(start_args['file'][:-3]+'bin', 'rb').read()
			os.remove(start_args['file'][:-3]+'bin')
		else:
			data = open(start_args['file'], 'rb').read()
		serial_port.write('S%i' % len(data))
		sleep(.9)
		answer = serial_port.read(2)
		if answer <> 'OK':
			print 'Size transmission error!'
			exit(6)
		serial_port.write('L')
		answer = serial_port.read(7)
		if answer[-2:] <> 'OK':
			print 'Size transmission error!'
			exit(6)
		answer = answer[:-2]
		if int(answer) <> len(data):
			print 'Size != size!'
			exit(6)
		if debug: print 'Recived file size: %s' % answer
		# W bin_stream
		serial_port.write('F')
		sleep(.1)
		for i, char in enumerate(data):
			serial_port.write(char)
			sleep(.005)
			if (i > 0) and not (i % 512):
				for j in range(10):
					answer = serial_port.read(3)
					if answer == 'ACK': break
				if debug: print answer
		sleep(1)
		answer = serial_port.read(2)
		if answer == 'OK':
			print 'Writing success.'
			exit(0)
		else:
			print 'Writing error!'
			if debug: print '\n%s\n' % answer
			exit(3)

		
	
	if 'v' in start_args: # Verify
		print 'Verifying...'
		if not 'file' in start_args:
			print 'Get the file!'
			exit(5)
		if start_args['file'][-4:] == '.hex':
			os.system('hex2bin.exe %s' % start_args['file'])
			reference_data = open(start_args['file'][:-3]+'bin', 'rb').read()
			os.remove(start_args['file'][:-3]+'bin')
		else:
			reference_data = open(start_args['file'], 'rb').read()
		serial_port.write('R')
		answer = ''
		for i in range(512 * 16):
			answer += serial_port.read(32)
			if answer[-2:] == 'OK': break
			# Timeout handler
		if answer[-2:] <> 'OK':
			print 'Reading error!'
			exit(4)
		if reference_data == answer[:len(reference_data)]:
			print 'Verification success!'
		else:
			print 'Verification failed!'
			

	serial_port.close()

if __name__ == '__main__':
	main()
