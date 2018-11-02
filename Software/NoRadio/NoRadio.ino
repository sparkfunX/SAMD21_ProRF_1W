/*
 * SAMD21_ProRF_1W - No Radio
 *
 * This is the code that ships on the ProRF 1W so that it is not damaged by not
 * having an antenna plugged in when you get it.
 */
#include <SPI.h>
//#include "LoRa.h" // Get the library here: (https://github.com/sandeepmistry/arduino-LoRa)

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


void setup() {
  SerialUSB.begin(115200);
  while(!SerialUSB){};
  SerialUSB.println("SAMD21 ProRF 1W - No Radio");

}

void loop() {
  SerialUSB.println("Hi, this is your ProRF! Radio is off, so not having an antenna is OK.");
  SerialUSB.println("However, before loading new code please give me an antenna :)");
  SerialUSB.println("(3 in. wire soldered into 'Ant' pin or use U.FL by moving 0402 resistor on module)");
  SerialUSB.println("Much appreciated!\n");

  
  delay(2500);
}
