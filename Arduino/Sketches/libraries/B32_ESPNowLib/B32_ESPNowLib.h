//B32_ESPNowLib.h, 1/27/24f
#pragma once
#pragma message("B32_ESPNowLib.h, Begin, include esp_now.h")
#include <esp_now.h>
#include <WiFi.h>
#include <Streaming.h>

const int                 wNumReadings=  8;

//Message Structure that is used to pass data back an forth
typedef struct stMessageStruct {
  double      adReading[wNumReadings];
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

extern enum eBoardPinColor  eReceiverBoardPinColor;

extern uint8_t              aucMyMACAddress[];

//Create stMessageStructure structures to hold readings
extern stMessageStruct      stIncomingReadings;
extern stMessageStruct      stOutgoingReadings;
extern stMessageStruct      stErrorReadings;
extern stMessageStruct      stLastReadings;

//Define variables to store temperature readings to be sent
extern double               dTCouple0_DegF;
extern double               dTCouple1_DegF;
extern double               dTCouple2_DegF;

extern long                 lAliveMsec;
extern long                 lCurrentMsec;
extern long                 lNextMsec;

extern esp_now_peer_info_t  stPeerInfo;


//Function prototypes
void  SetupESPNow                 (void);
void  OnDataRecv                  (const uint8_t *pucMACAddress,
                                   const uint8_t *pucIncomingData, int wNumBytes);
void  OnDataSent                  (const uint8_t *mac_addr, esp_now_send_status_t status);
void  HandleDataReceived          (void);
void  SendDataToDisplayBoard      (stMessageStruct stReadings);
void  PrintReadings               (stMessageStruct stReadings);
void  ResetTimer                  (void);
//Last line
