//B32_TCoupleLib.h, 10/16/23f
#pragma once

#include <Streaming.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>
#include <MAX31855.h>

#define WITH_DISPLAY     false

#if WITH_DISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#endif

#define ONE_DOT_RECEIVER    false       //ESP32 w/o USB-C, returned to Amazon
#define TWO_DOT_RECEIVER    false        //ESP32 w/o USB-C, returned to Amazon

#define RED_PIN_RECEIVER    true       //TTGO with red header pins, Remote tcouple reader
#define BLACK_PIN_RECEIVER  false        //TTGO with black header pins, tcouple display

//Message Structure that is used to pass data back an forth
typedef struct stMessageStructure {
  double dTCouple0_DegF;
  double dTCouple1_DegF;
  double dTCouple2_DegF;
} stMessageStructure;

#if RED_PIN_RECEIVER
  //Running on BlackPin TTGO, sends data to RedPin TTGO
  //extern uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC};   //RedPin MAC
  extern uint8_t broadcastAddress[];
#endif
#if BLACK_PIN_RECEIVER
  //Running on RedPin TTGO, sends data to BlackPin TTGO
  extern uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C};   //BlackPin MAC
#endif  //TWO_DOT_RECEIVER

/*
//Red pin TTGO to be connected to 8x TCouple board and transmit to black pin TTGO
//Black pin TTGO is the display module
//From B32_GetMACAddress.ino
uint8_t OneDotMAC[]     = {0x48, 0xE7, 0x29, 0xAF, 0x7B, 0xDC};  //Returned to Amazon
uint8_t TwoDotMAC[]     = {0x48, 0xE7, 0x29, 0xB6, 0xC3, 0xA0};  //Returned to Amazon
uint8_t aucRedPinMAC[]  = {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC}; //RedPin MAC
uint8_t aucBlackPinMAC[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //BlackPin MAC
*/
//Define variables to store temperature readings to be sent
extern double         dTCouple0_DegF;
extern double         dTCouple1_DegF;
extern double         dTCouple2_DegF;

extern const int      wNumTCouples;
extern double         adTCoupleDegF[];

extern long           lAliveMsec;
extern long           lCurrentMsec;
extern long           lNextMsec;

// Variable to store if sending data was successful
extern String         szSuccess;

// Create a stMessageStructure to hold incoming sensor readings
extern stMessageStructure   stIncomingReadings;
extern stMessageStructure   stOutgoingReadings;

extern TFT_eSPI             tft;  //Class library for TTGO T-Display
extern esp_now_peer_info_t  stPeerInfo;

//Function prototypes
void  setup             (void);
void  loop              (void);
void  ResetTimer        (void);
void  SetupDisplay      (void);
void  SetupESP_NOW      (void);
void  OnDataRecv        (const uint8_t *pucMACAddress,
                         const uint8_t *pucIncomingData, int wNumBytes);
void  OnDataSent        (const uint8_t *mac_addr, esp_now_send_status_t status);
void  UpdateDisplay     (void);
void  PrintTemperatures (void);
void  PrintTemperature  (double dDegF);
//Last line
