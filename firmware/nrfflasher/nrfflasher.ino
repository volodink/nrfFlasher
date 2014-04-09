#include <SPI.h>
#include <Flash.h>

// debug mode activation derective: 1 = on, 0 = off
#define DEBUG 0

#define WREN 0x06
#define ERASEPAGE 0x52
#define PROGRAM 0x02
#define READ 0x03

int NRF_RESET = 9;
int NRF_PROG  = 8;
int FCSN = 10;

unsigned long int firmwareFlashSize = 0;

FLASH_STRING(nrfflasher_version, "0.0.1");

int availableMemory() 
{
  int size = 1024;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
}

void erase_all(){
    #if DEBUG == 1
      Serial.print("Erasing...\n");
    #endif
    // erasing
    #if DEBUG == 1
      Serial.print("Erasing...ok\n");
    #endif
}

void reset(){
    #if DEBUG == 1
      Serial.print("Resetting...\n");
    #endif
}

void read_all(){
    #if DEBUG == 1
      Serial.print("Reading/dumping ROM data...\n");
    #endif
}

void get_flash_size(){
    Serial.print(firmwareFlashSize, DEC);
}

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

void receive_flash_data(){
  #if DEBUG == 1
    Serial.print("Waiting for data...");
  #endif
}

void get_version(){
  nrfflasher_version.print(Serial);
}

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
	delay(1000);
	
	// Ready to go!
} 

// The loop routine runs over and over again forever:
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
                                 Serial.print(availableMemory());
                                 Serial.print("OK");
			default:
				// test test
				break;
		}
	}
}
