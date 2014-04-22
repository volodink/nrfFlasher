nrfFlasher
==========

arduino based flasher/burner for nrf24le1 + python pc-side flash utility.

Usage:

Erase chip (without InfoPage)
nrfflasher.exe -e

Write firmware to chip
nrfflasher.exe -w -file myhex.hex


To see more debug info, simply add 
-verbose 

Examples:
nrfflasher.exe -e -verbose
nrfflasher.exe -w -verbose -file myhex.hex
