/**
      nrfFlasher firmware code. 
      
      author: volodink
*/

// Include necessary headers
#include <SPI.h>
#include <Flash.h>

// Debug mode activation directive: 1 = on, 0 = off
#define DEBUG 0

// Define operation modes for work with nrf24le1 memory 
#define WREN 0x06
#define ERASEPAGE 0x52
#define PROGRAM 0x02
#define READ 0x03

// Creating strings in atmega's ROM
FLASH_STRING(nrfflasher_version, "0.0.1");
FLASH_STRING(erasing,      "\nErasing...\n");
FLASH_STRING(erasing_ok,   "\nErasing...ok\n");
FLASH_STRING(resetting,    "\nResetting...\n");
FLASH_STRING(resetting_ok, "\nResetting...ok\n");
FLASH_STRING(reading,    "\nReading/dumping ROM data...\n");
FLASH_STRING(reading_ok, "\nReading/dumping ROM data...ok\n");

// Pin definitions for nrf24le1 chip
int NRF_RESET = 9;
int NRF_PROG  = 8;
int FCSN = 10;

// Number of bytes to write to nrf24le1
unsigned long int firmwareFlashSize = 0;

/**
*/
int availableMemory() 
{
  int size = 1024;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
}

/**
*/
void erase_all(){
    #if DEBUG == 1
      erasing.print(Serial);
    #endif
    // erasing
    #if DEBUG == 1
      erasing_ok.print(Serial);
    #endif
}

/**
*/
void reset(){
    #if DEBUG == 1
      resetting.print(Serial);
    #endif
    // resetting
    #if DEBUG == 1
      resetting_ok.print(Serial);
    #endif
}

/**
*/
void read_all(){
    #if DEBUG == 1
      reading.print(Serial);
    #endif
    // reading data
    #if DEBUG == 1
      reading_ok.print(Serial);
    #endif
}

/**
*/
void get_flash_size(){
    #if DEBUG == 1
      Serial.print("\nGetting flash size...\n");
    #endif
    Serial.print(firmwareFlashSize, DEC);
    #if DEBUG == 1
      Serial.print("\nGetting flash size...done\n");
    #endif
}

/**
*/
void receive_flash_size(){
  #if DEBUG == 1
    Serial.print("\nWaiting for flash size...\n");
  #endif
    firmwareFlashSize = Serial.parseInt();
  #if DEBUG == 1
    Serial.print("Got size: ");
    Serial.print(firmwareFlashSize);
    Serial.print("\n");
  #endif
}

/**
*/
void receive_flash_data(){
  #if DEBUG == 1
    Serial.print("Waiting for data...");
  #endif
  // receive flash data
  /*
  if (Serial.available() > 0)
  {
    while (Serial.available() > 0)
    {
      
    }
  }  
  */
  #if DEBUG == 1
    Serial.print("Got data ok.");
  #endif
}

/**
*/
void get_version(){
  nrfflasher_version.print(Serial);
}

/**
*/
void setup(){
  	
	// Configuration and start the SPI library
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV8);
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(MSBFIRST);

	// Serial port configuration
	Serial.begin(9600);
	
	// Initialize the digital pin as an output.
	pinMode(NRF_RESET, OUTPUT);  
	pinMode(NRF_PROG, OUTPUT);
	pinMode(FCSN, OUTPUT);
	
	digitalWrite(NRF_RESET, HIGH);  
	
	// Start delay
	delay(100);
	
	// Ready to go!

} 

/**
  The loop routine runs over and over again forever
*/
int incomingByte = 0;
void loop(){
	if (Serial.available() > 0){
		incomingByte = Serial.read();
		switch (incomingByte)
                {
			case 'T':                      // test
				Serial.print("OK");
				break;
			case 'V':                      // version
                                get_version();
				Serial.print("OK");
				break;
                        case 'E':                      // erase chip  (infopage stays intact)
                                erase_all();
                                Serial.print("OK");
                                break;
                        case 'A':
                                reset();               // reset chip
                                Serial.print("OK");
                                break;
                        case 'R':                      // read rom and dump it to the serial out
                                read_all();
                                Serial.print("OK");
                                break;
                        case 'S':
                                receive_flash_size();  // receive flash length in bytes
                                Serial.print("OK");
                                break;
                        case 'L':
                                get_flash_size();
                                Serial.print("OK");
                                break;
                        case 'F':                      // receive flash content
                                receive_flash_data();
                                Serial.print("OK");
                                break;
                        case 'D':
                                 Serial.print(availableMemory()); // get amount of free memory RAM
                                 Serial.print("OK");
			default:
				// test test
				break;
		}
	}
}

