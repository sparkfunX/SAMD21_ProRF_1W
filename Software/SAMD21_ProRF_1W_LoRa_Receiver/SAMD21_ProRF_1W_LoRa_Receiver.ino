/*
 * SAMD21_ProRF_1W_LoRa_Receiver
 * 
 * This example uses the source code from Sandeep Mistry's LoRa library (https://github.com/sandeepmistry/arduino-LoRa)
 * to receive point-to-point (P2P) LoRa packets. For the easiest use you should run the SAMD21_ProRF_1W_LoRa_Sender 
 * sketch on another ProRF 1W board as the transmitter.
 * 
 * License: Originally under MIT license.
 */
#include <SPI.h>
#include "LoRa.h" // Get the library here: (https://github.com/sandeepmistry/arduino-LoRa)

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

#define TXEN_PIN 30
#define RXEN_PIN 31

uint16_t C0 = 0;            // Bytes to use in computing the checksum
uint16_t C1 = 0;    

uint32_t timenow = 0;     
uint32_t lastmillis = 0;

struct PC{                  // "Packet Counter" structure definition
  uint32_t bad = 0;
  uint32_t good = 0;
};

PC packetCounter;           // A packet counter to use

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB){};    // Make sure we get to read the title text
  SerialUSB.println("SAMD21 ProRF 1W - LoRa Receiver");

  pinMode(TXEN_PIN, OUTPUT);                      // Setup the pins that control the 1W amplifiers
  pinMode(RXEN_PIN, OUTPUT);                      // Without these lines the RSSI will be greatly reduced
  digitalWrite(TXEN_PIN, LOW);
  digitalWrite(RXEN_PIN, HIGH);

  while(!LoRa.begin(915E6)) {                     // Try to start the module
    SerialUSB.println("Starting LoRa failed!");
    delay(500);
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    handlePacket();   // If a packet was received then check it out
  }
}


void handlePacket()
{
  C0 = 0; // Reset the checksum calculation bytes
  C1 = 0;

  // Compute checksum and print received characters
  SerialUSB.print("Received packet '");
  while (LoRa.available()) {
    char c = (char)LoRa.read();
    SerialUSB.print(c);
    C0 = (C0 + c) % 255;    // Using Fletcher's 16-bit checksum:  https://en.wikipedia.org/wiki/Fletcher%27s_checksum
    C1 = (C1 + C0) % 255;
  }
  
  // print RSSI of packet
  int16_t thisRSSI = LoRa.packetRssi();
  SerialUSB.print("' with RSSI ");
  SerialUSB.println(thisRSSI);
  
  // Compute the checksum result
  uint16_t CB0 = ((C0 + C1) % 255);
  uint16_t CB1 = ((C0 + CB0) % 255);

  if(CB0 || CB1)              // CB0 and CB1 both evaluate to 0 when the checksum is good
  { 
    packetCounter.bad += 1; 
  }
  else
  { 
    packetCounter.good += 1; 
  }

  SerialUSB.print(", Total Good: "); 
  SerialUSB.print(packetCounter.good);
  SerialUSB.print(", Total Bad: ");
  SerialUSB.print(packetCounter.bad);
  SerialUSB.println();
}

