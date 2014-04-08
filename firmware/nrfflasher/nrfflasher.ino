#include <SPI.h>

#define WREN 0x06
#define ERASEPAGE 0x52
#define PROGRAM 0x02
#define READ 0x03

int NRF_RESET = 9;
int NRF_PROG  = 8;
int FCSN = 10;

void setup()
{
  	
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
void loop()
{
	if (Serial.available() > 0)
	{
		incomingByte = Serial.read();
		switch (incomingByte)
		{
			case 'T':
				Serial.print("OK");
				break;
			default:
				// test test
				break;
		}
	}
}
