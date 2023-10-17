//B32_TCoupleLib.h, 10/16/23h
#pragma once

#include <Streaming.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>
#include <MAX31855.h>

#define WITH_DISPLAY     false

#define ONE_DOT_RECEIVER    false       //ESP32 w/o USB-C, returned to Amazon
#define TWO_DOT_RECEIVER    false        //ESP32 w/o USB-C, returned to Amazon

/*
#define RED_PIN_RECEIVER    true         //TTGO with red header pins, Remote tcouple reader
#define BLACK_PIN_RECEIVER  false        //TTGO with black header pins, tcouple display
*/

//Message Structure that is used to pass data back an forth
typedef struct stMessageStructure {
  double dTCouple0_DegF;
  double dTCouple1_DegF;
  double dTCouple2_DegF;
} stMessageStructure;

extern const char           szFileName[];
extern const char           szFileDate[];

extern uint8_t              aucReceiverMACAddress[];

//Define variables to store temperature readings to be sent
extern double               dTCouple0_DegF;
extern double               dTCouple1_DegF;
extern double               dTCouple2_DegF;

extern const int            wNumTCouples;
extern double               adTCoupleDegF[];

extern long                 lAliveMsec;
extern long                 lCurrentMsec;
extern long                 lNextMsec;

// Variable to store if sending data was successful
extern String               szSuccess;

//Create an stMessageStructure to hold incoming sensor readings
extern stMessageStructure   stIncomingReadings;
extern stMessageStructure   stOutgoingReadings;

extern TFT_eSPI             tft;  //Class library for TTGO T-Display
extern esp_now_peer_info_t  stPeerInfo;

//Function prototypes
void  setup             (void);
void  loop              (void);
void  SetupDisplay      (void);
void  SetupESP_NOW      (void);
void  OnDataRecv        (const uint8_t *pucMACAddress,
                         const uint8_t *pucIncomingData, int wNumBytes);
void  OnDataSent        (const uint8_t *mac_addr, esp_now_send_status_t status);
void  UpdateDisplay     (void);
void  PrintTemperatures (void);
void  PrintTemperature  (double dDegF);
void  ResetTimer        (void);
//Last line
