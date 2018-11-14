#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#define __SAMD21G18A__ 1

#define VENDOR_NAME "SparkFun Electronics"
#define PRODUCT_NAME "SparkFun RedBoard Turbo"
#define VOLUME_LABEL "TURBOBOOT"
#define INDEX_URL "https://www.sparkfun.com/products/13912"
#define BOARD_ID "SAMD21G18A-SparkFun-RedBoard-Turbo-v0"

#define USB_VID 0x1B4F
#define USB_PID 0x0015

#define LED_PIN PIN_PA17
#define LED_TX_PIN PIN_PA27
#define LED_RX_PIN PIN_PB31

// 8/2018
// Until the repo is updated ... 
//
// There is an issue in the multiplexing for the neopixel
// The file neopixel.h needs the TURBO BOARD USB VID & PID
// added to the section that deals with this issue for Adafruit
//
// This is needed or the UF2 bootloader won't indicate it's state
// using the neopixel (green - connected to computer, red - connected to power)
//
// Basicall modify the #define to include a check for SFE Turbo (around line 70)
// The values are:
//  ((USB_VID == 0x1B4F) && (USB_PID == 0x0015))

#define BOARD_NEOPIXEL_PIN PIN_PA30
#define BOARD_NEOPIXEL_COUNT 1


#endif
