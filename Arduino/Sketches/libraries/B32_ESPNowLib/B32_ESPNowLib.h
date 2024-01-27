//B32_ESPNowLib.h, 1/27/24a
#pragma once
#pragma message("B32_ESPNowLib.h, Begin, include esp_now.h")
//#define TOUCH_CS

//#include <lvgl.h>
//#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <esp_now.h>
#include <WiFi.h>
//#include <MAX31855.h>
#include <Streaming.h>

const int                 wNumDataPoints=  8;

//Message Structure that is used to pass data back an forth
typedef struct stMessageStruct {
  double      adTCoupleDegF[wNumDataPoints];
} stMessageStruct;

enum class eBoardPinColor {
  eBluePin= 1,
  eWhitePin,
  eBlackPin,
  e3dot2NoPin,
  eDIYMall3dot5NoPin,
  eWaveshare4dot3A,
  eLastPin
}; //eBoardPinColor

extern enum eBoardPinColor   eReceiverBoardPinColor;

/*
const int                   wMaxLabelChars        = 20;
const int16_t               sLineSpacing          = 35;

const int16_t               sLeftLabelX           =  0;
const int16_t               sLeftLabelFirstY      = 70;

const int16_t               sLeftDataX           = 80;
//const int16_t               sLeftDataX           = 90;
const int16_t               sLeftDataFirstY      = sLeftLabelFirstY;

const int16_t               sRightLabelX          = 200;
//const int16_t               sRightLabelX          = 190;
const int16_t               sRightLabelFirstY     = sLeftLabelFirstY;


//const int16_t               sRightDataX          = 300;
const int16_t               sRightDataX          = 290;
const int16_t               sRightDataFirstY     = sLeftDataFirstY;
*/


//Message Structure that is used to pass data back an forth
typedef struct stMessageStructure {
  double      adTCoupleDegF[wNumDataPoints];
} stMessageStructure;

extern uint8_t              aucMyMACAddress[];

//Create stMessageStructure structures to hold readings
extern stMessageStructure   stIncomingReadings;
extern stMessageStructure   stOutgoingReadings;
extern stMessageStructure   stErrorReadings;
extern stMessageStructure   stLastReadings;

/*
extern const char           szSketchName[];
extern const char           szFileDate[];

extern const uint8_t        aucESP32_ScreenOneDot[];     //ESP32 w/3.2" LCD
extern const uint8_t        aucESP32_ScreenTwoDot[];     //ESP32 w/3.2" LCD
*/

//Define variables to store temperature readings to be sent
extern double               dTCouple0_DegF;
extern double               dTCouple1_DegF;
extern double               dTCouple2_DegF;

extern long                 lAliveMsec;
extern long                 lCurrentMsec;
extern long                 lNextMsec;

/*
extern MAX31855             TCoupleObject;
extern TFT_eSPI             Screen;  //Class library for TTGO T-Display
*/
extern esp_now_peer_info_t  stPeerInfo;


//Function prototypes
void  SetupESPNow                 (void);
void  OnDataRecv                  (const uint8_t *pucMACAddress,
                                   const uint8_t *pucIncomingData, int wNumBytes);
void  OnDataSent                  (const uint8_t *mac_addr, esp_now_send_status_t status);
void  HandleDataReceived          (void);
void  SendDataToDisplayBoard      (stMessageStructure stReadings);
/*
bool  bCheckTouch                 (uint16_t *pusXCoordParam, uint16_t *pusYCoordParam);
void  SetupScreen                 (uint8_t ucRotation);
void  DisplayLabels               (void);
void  UpdateTTGOScreen            (stMessageStructure stReadings);
void  DisplayData                 (stMessageStructure stReadings);
void  ReadAmbiant                 (void);
void  ReadTCouples                (stMessageStructure& stReadings);
void  PrintTemperatures           (stMessageStructure stReadings);
void  PrintTemperature            (double dDegF);
*/
/*
void  ShowMyMAC                   (bool bDisplay);
void  PrintMyMAC                  (void);
*/
//void  SetupPins                   (void);
void  ResetTimer                  (void);
//Last line
