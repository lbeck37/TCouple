//B32_TCoupleLib.h, 12/21/23b
#pragma once
#pragma message("B32_TCoupleLib.h, Begin, include TFT_eSPI.h, if not already included")
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
//#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>
#include <MAX31855.h>
#include <Streaming.h>

enum class eBoardPinColor {
  eBluePin= 1,
  eWhitePin,
  eBlackPin,
  e3dot2NoPin,
  eLastPin
}; //eBoardPinColor

const int   wNumTCouples= 8;

//Message Structure that is used to pass data back an forth
typedef struct stMessageStructure {
  double      adTCoupleDegF[wNumTCouples];
} stMessageStructure;

extern uint8_t              aucMyMACAddress[];

//Create stMessageStructure structures to hold readings
extern stMessageStructure   stIncomingReadings;
extern stMessageStructure   stOutgoingReadings;
extern stMessageStructure   stErrorReadings;

//extern uint8_t              auc100ByteBuffer[100];
extern const char           szSketchName[];
extern const char           szFileDate[];

extern const uint8_t        aucESP32_ScreenOneDot[];     //ESP32 w/3.2" LCD
extern const uint8_t        aucESP32_ScreenTwoDot[];     //ESP32 w/3.2" LCD

//Define variables to store temperature readings to be sent
extern double               dTCouple0_DegF;
extern double               dTCouple1_DegF;
extern double               dTCouple2_DegF;

extern long                 lAliveMsec;
extern long                 lCurrentMsec;
extern long                 lNextMsec;

extern TFT_eSPI             Screen;  //Class library for TTGO T-Display
extern esp_now_peer_info_t  stPeerInfo;

//Function prototypes
//void  SetupScreen                 (void);
void  SetupScreen                 (uint8_t ucRotation);
void  SetupESP_NOW                (void);
void  OnDataRecv                  (const uint8_t *pucMACAddress,
                                   const uint8_t *pucIncomingData, int wNumBytes);
void  OnDataSent                  (const uint8_t *mac_addr, esp_now_send_status_t status);
void  HandleDataReceived          (void);
void  SendDataToDisplayBoard      (void);
void  UpdateScreen                (stMessageStructure stReadings);
void  PrintTemperatures           (void);
void  PrintTemperature            (double dDegF);
void  ShowMyMAC                   (bool bDisplay);
void  ResetTimer                  (void);
//Last line
