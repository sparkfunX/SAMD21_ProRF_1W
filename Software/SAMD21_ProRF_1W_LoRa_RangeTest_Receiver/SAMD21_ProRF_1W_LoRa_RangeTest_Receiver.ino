/*
 * SAMD21_ProRF_1W_LoRa_RangeTest_Receiver
 * 
 * This example uses the source code from Sandeep Mistry's LoRa library (https://github.com/sandeepmistry/arduino-LoRa)
 * to receive point-to-point (P2P) LoRa packets. When a packet is received correctly GPS coordinates and other data is 
 * printed to the serial port to be logged for a range test. The success of packets is based on the Fletcher's 16-bit checksum 
 * which is implemented in SAMD21_ProRF_1W_LoRa_Sender. 
 * 
 * License: Originally under MIT license.
 */
#include <SPI.h>
#include "LoRa.h"

/* 
This sketch comes with a modified LoRa.h file that includes pin definitions for the 1W SAMD21 ProRF. 
If you do not have those definitions then they are re-listed here.

#define LORA_DEFAULT_SPI           SPI
//#define LORA_DEFAULT_SPI_FREQUENCY 8E6 
#define LORA_DEFAULT_SPI_FREQUENCY 500000 
#define LORA_DEFAULT_SS_PIN        12
#define LORA_DEFAULT_RESET_PIN     13
#define LORA_DEFAULT_DIO0_PIN      6

*/

#include <SparkFun_I2C_GPS_Arduino_Library.h> //Use Library Manager or download here: https://github.com/sparkfun/SparkFun_I2C_GPS_Arduino_Library
I2CGPS myI2CGPS; //Hook object to the library

#include <TinyGPS++.h> //From: https://github.com/mikalhart/TinyGPSPlus
TinyGPSPlus gps; //Declare gps object

#define TXEN_PIN 30
#define RXEN_PIN 31

#define PORT SerialUSB

double lat = 0;
double lng = 0;
int hour, minute, second;
uint8_t status = 0x00;
uint16_t logcount = 0;

#define LOC_VALID_MASK 0x01
#define TIM_VALID_MASK 0x02

uint16_t C0 = 0;
uint16_t C1 = 0;

struct PC{
  uint32_t missed = 0;
  uint32_t bad = 0;
  uint32_t good = 0;
};

PC packetCounter;

void setup() {
  PORT.begin(9600);
  while (!PORT){};

  Serial.begin(9600); // For sending data to the regular prorf

  PORT.println("P2P Range Test");
  PORT.println("Transmitter: SAMD21 ProRF 1W, Fiberglass Antenna");
  PORT.println("Receiver: SAMD21 ProRF 1W, Big Duck Antenna");

  pinMode(TXEN_PIN, OUTPUT);
  pinMode(RXEN_PIN, OUTPUT);
  digitalWrite(TXEN_PIN, LOW);
  digitalWrite(RXEN_PIN, HIGH); // Enable reception

  while(!LoRa.begin(915E6)) {
    PORT.println("Starting LoRa failed!");
    delay(200);
  }

  if (myI2CGPS.begin() == false)
  {
    while (myI2CGPS.begin() == false)
    {
       PORT.println("Module failed to respond. Please check wiring.");
       delay(100);
    }
  }
  PORT.println("GPS module found! Beginning Test\n");
}

void loop() {

  while (myI2CGPS.available()) //available() returns the number of new bytes available from the GPS module
  {
    gps.encode(myI2CGPS.read()); //Feed the GPS parser
  }
  if (gps.time.isUpdated()) //Check to see if new GPS info is available
  {
    updateInfo();
  }
  look4LoRa();
}

void updateInfo()
{ 
  if (gps.time.isValid())
  {
    status |= TIM_VALID_MASK;
    
    hour = gps.time.hour();
    minute = gps.time.minute();
    second = gps.time.second();
  }
  else
  {
    status &= ~TIM_VALID_MASK; // Indicate that time is invalid

    hour = 0x00;
    minute = 0x00;
    second = 0x00;
  }
  if (gps.location.isValid())
  {
    status |= LOC_VALID_MASK;    // Indicate that location is valid

    lat = gps.location.lat();
    lng = gps.location.lng();
  }
  else
  {
    status &= ~LOC_VALID_MASK; // Indicate that location is invalid

    lat = 0;
    lng = 0;
  }
}

void look4LoRa( void )
{
  // Look for incoming messages
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  int16_t thisRSSI = LoRa.packetRssi();
  if (packetSize) {
    
    PORT.print(logcount++); 
    
    PORT.print(", SysTime (ms): ");
    PORT.print(millis());

    PORT.print(", GPS_STAT: ");
    PORT.print(status);

    PORT.print(", LAT: ");
    PORT.print(lat, 10);

    PORT.print(", LNG: ");
    PORT.print(lng, 10);

    PORT.print(", Hour: ");
    PORT.print(hour);
    PORT.print(", Minute: ");
    PORT.print(minute);
    PORT.print(", Second: ");
    PORT.print(second);
    
    PORT.print(", Packet: '");

      // read packet
      while (LoRa.available()) {
        char c = (char)LoRa.read();
        PORT.print(c);
        Serial.write(c);
        C0 = (C0 + c) % 255;
        C1 = (C1 + C0) % 255;
      }
      

      // print RSSI of packet
      PORT.print("', RSSI: ");
      PORT.print(thisRSSI);

      // Checksum
      uint16_t CB0 = ((C0 + C1) % 255);
      uint16_t CB1 = ((C0 + CB0) % 255);

      PORT.print(", PcktOK: ");
      if(CB0 || CB1){ packetCounter.bad += 1; PORT.print(0); }
      else{ packetCounter.good += 1; PORT.print(1); }

      PORT.print(", TotalOK: ");
      PORT.print(packetCounter.good);
      PORT.print(", TotalBad: ");
      PORT.print(packetCounter.bad);
      PORT.println();
  }
}

