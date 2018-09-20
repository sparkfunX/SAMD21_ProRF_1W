/*
 * SAMD21_ProRF_1W_LoRa_Receiver
 * 
 * This example uses the source code from Sandeep Mistry's LoRa library (https://github.com/sandeepmistry/arduino-LoRa)
 * to send point-to-point (P2P) LoRa packets. For the easiest use you should run the SAMD21_ProRF_1W_LoRa_Receiver 
 * sketch on another ProRF 1W board as the reciever.
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
#define TEST_LENGTH 25    // Choose the total number of characters to send in each packet

int counter = 0;          // Counts the number of messages sent

uint16_t C0 = 0;          // Used in calculation of the checksum, to be verified by the receiver
uint16_t C1 = 0;

uint32_t timenow = 0;
uint32_t lastmillis = 0;

void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB){};
  SerialUSB.println("SAMD21 ProRF 1W - LoRa Sender");

  randomSeed(analogRead(0));  // Provide a random seed

  pinMode(TXEN_PIN, OUTPUT);      // Enable the 1W amplifier output
  pinMode(RXEN_PIN, OUTPUT);      // Without doing so the outgoing signal will be very weak
  digitalWrite(TXEN_PIN, HIGH);
  digitalWrite(RXEN_PIN, LOW);

  while (!LoRa.begin(915E6)) {
    SerialUSB.println("Starting LoRa failed!");
    delay(500);
  }
}

void loop() {
  SerialUSB.print("Sending packet: ");
  SerialUSB.print(counter);
  SerialUSB.print(", time since last send: ");

  timenow = millis();
  SerialUSB.println(timenow-lastmillis);
  lastmillis = timenow;


  // send packet
  LoRa.beginPacket();
  C0 = 0; 
  C1 = 0;
  for(uint8_t indi = 0; indi < TEST_LENGTH-2; indi++) // In this loop we choose random uppercase characters to send
  {                                                   // Additionally we compute Fletcher's 16-bit checksum (https://en.wikipedia.org/wiki/Fletcher%27s_checksum)
    char c = random(65,91);
    LoRa.write(c);
    C0 = (C0 + c) % 255;
    C1 = (C1 + C0) % 255;
  }
  uint16_t CB0 = 255 - ((C0 + C1) % 255);
  uint16_t CB1 = 255 - ((C0 + CB0) % 255);
  LoRa.write((uint8_t)(CB0 & 0xFF));
  LoRa.write((uint8_t)(CB1 & 0xFF));
  LoRa.endPacket();

  counter++;
  
  delay(500);
}
