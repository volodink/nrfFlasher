/**
      nrfFlasher firmware code. 
      
      author: volodink
*/

/**
      IMPORTANT NOTES:
      1. Erasing performed on 0-30 pages for test only. If your firmware is bigger than 30 pages, add 32 in page counter.
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

//#define START_PROGRAM() { digitalWrite(FCSN, LOW); SPI.transfer(WREN); digitalWrite(FCSN, HIGH); delay(1); digitalWrite(FCSN, LOW); SPI.transfer(PROGRAM); }

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
    Erase page 
    
    description:
    Performs erase page by its number.
*/
void erase_page(unsigned char pagenum)
{
  digitalWrite(NRF_PROG, HIGH);
  delay(10);
  digitalWrite(NRF_RESET, LOW);
  delay(1);
  digitalWrite(NRF_RESET, HIGH);
  delay(1);

  digitalWrite(FCSN, LOW);
  SPI.transfer(WREN);
  digitalWrite(FCSN, HIGH);
  delay(1);
  digitalWrite(FCSN, LOW);
  SPI.transfer(ERASEPAGE);
  SPI.transfer(pagenum);
  digitalWrite(FCSN, HIGH);
  delay(10);

  digitalWrite(NRF_PROG, LOW);
  delay(10);
  digitalWrite(NRF_RESET, HIGH);
  delay(10);
}

/**
*/
void erase_all()
{
    int pageCounter = 0;
    #if DEBUG == 1
      erasing.print(Serial);
    #endif
    // erasing
    for (pageCounter = 0; pageCounter < 31; pageCounter++)
    {
      erase_page(pageCounter);
    }
    #if DEBUG == 1
      erasing_ok.print(Serial);
    #endif
}

/**
*/
void reset()
{
    #if DEBUG == 1
      resetting.print(Serial);
    #endif
    // resetting
    digitalWrite(NRF_RESET, LOW);
    delay(100);
    digitalWrite(NRF_RESET, HIGH);
    delay(100);
    #if DEBUG == 1
      resetting_ok.print(Serial);
    #endif
}

/**
*/
void read_data()
{
  unsigned char myChar;
  
  digitalWrite(NRF_PROG, HIGH);
  delay(10);
  digitalWrite(NRF_RESET, LOW);
  delay(1);
  digitalWrite(NRF_RESET, HIGH);
  delay(1);

  digitalWrite(FCSN, LOW);
  delay(1);
  SPI.transfer(READ);
  SPI.transfer(0);
  SPI.transfer(0);  
  //Serial.print("\n");
  for (int i = 0; i < 16384-512; i++)
  {
    myChar = SPI.transfer(0xFF);
    Serial.write(myChar);
    delay(1);
    //PrintHex8(&myChar, 1);
    //Serial.print("  ");
    //if (i % 16 == 0) Serial.print("\n");
  }
  
  digitalWrite(FCSN, HIGH);
  delay(10);

  digitalWrite(NRF_PROG, LOW);
  delay(1);
  digitalWrite(NRF_RESET, HIGH);
  delay(1);
}

/**
*/
void read_all()
{
    #if DEBUG == 1
      reading.print(Serial);
    #endif
    // reading data
    read_data();
    #if DEBUG == 1
      reading_ok.print(Serial);
    #endif
}

/**
*/
void get_flash_size()
{
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
void receive_flash_size()
{
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

//unsigned char data_array[512];

/**
*/
void receive_flash_data()
{
  unsigned int address = 0;
  unsigned char myChar;

  #if DEBUG == 1
    Serial.print("Waiting for data...");
  #endif
  
  // begin writing
  digitalWrite(NRF_PROG, HIGH);
  delay(100);
  digitalWrite(NRF_RESET, LOW);
  delay(1);
  digitalWrite(NRF_RESET, HIGH);
  delay(1);
  
  while (firmwareFlashSize > 0)
  {
      if (firmwareFlashSize <= 512)
      {  
         //Serial.print("section 1\n");
         // start writing page
         digitalWrite(FCSN, LOW); SPI.transfer(WREN); digitalWrite(FCSN, HIGH); delay(1); digitalWrite(FCSN, LOW); SPI.transfer(PROGRAM);
         
         // set address
         // low byte
         SPI.transfer((unsigned char) (address & 0xFF));
         // high byte
         SPI.transfer((unsigned char) ((address & 0xFF00) >> 8));   
         delay(1);
         
         // get and write data
         for (int i = 0; i < firmwareFlashSize; i++)
         {
           myChar = Serial.read();
           delay(1);
           SPI.transfer(myChar);
           delay(1);          
         }
                  
         // end writing
         digitalWrite(FCSN, HIGH); delay(100);
         
         // increment page;
         //Serial.println(address);
         //Serial.print("\n");
         //address = address + 0x02;
         //Serial.println(address);
         //Serial.print("\n");
         //Serial.println(firmwareFlashSize);
         //Serial.print("\n");
         firmwareFlashSize = 0;         
         //Serial.print(firmwareFlashSize);
         //Serial.print("\n");    
      }
      else
      {  // if flash size greater than one page
         
         //Serial.print("section 2\n");
         // start writing page
         digitalWrite(FCSN, LOW); SPI.transfer(WREN); digitalWrite(FCSN, HIGH); delay(1); digitalWrite(FCSN, LOW); SPI.transfer(PROGRAM);
         
         // set address
         
        // Serial.print("Low: ");
        //   myChar = address & 0xFF;
         //  Serial.println(myChar);
        // Serial.print("High: ");
        //   myChar = (address & 0xFF00) >> 8;
        //   Serial.println(myChar);
         
         // low byte
         SPI.transfer((unsigned char) (address & 0xFF));
         // high byte
         SPI.transfer((unsigned char) ((address & 0xFF00) >> 8));   
         delay(1);
         
         // get and write data
         
         for (int i = 0; i < 512; i++)
         {
           myChar = Serial.read();
           delay(1);
           SPI.transfer(myChar);
           delay(1);          
         }
         
         // end writing
         digitalWrite(FCSN, HIGH); delay(100);
         
         // increment page;
         //Serial.println(address);
         //Serial.println("\n");
         address = address + 0x02;
         //Serial.println(address);
         //Serial.println("\n");
         
         // decrement flash size counter
         firmwareFlashSize = firmwareFlashSize - 512;
         //Serial.println(firmwareFlashSize);
         //Serial.println("\n");
      }
 }  
    // set address
    // low byte
    //  SPI.transfer((unsigned char) (address & 0xFF));
    // high byte
    //  SPI.transfer((unsigned char) ((address >> 8) & 0xFF));      
    
  
  // finish writing
  digitalWrite(NRF_PROG, LOW);
  delay(100);
  digitalWrite(NRF_RESET, HIGH);
  delay(100);
 
  #if DEBUG == 1
    Serial.print("Got data ok.");
  #endif
}

/**
*/
void get_version()
{
  nrfflasher_version.print(Serial);
}

/**
*/
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
	
	//for (int i = 0; i < 512; i++) data_array[i] = 0;


        // Start delay
	delay(100);
	
	// Ready to go!

} 

/**
  The loop routine runs over and over again forever
*/
int incomingByte = 0;
void loop()
{
	if (Serial.available() > 0)
	{
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
            //case 'D':
			//	Serial.print(availableMemory()); // get amount of free memory RAM
            //    Serial.print("OK");
			default:
				// test test
				break;
		}
	}
}
