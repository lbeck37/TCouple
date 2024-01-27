//const char szFileName[]  = "B32_ESPNowLib.cpp";
//const char szFileDate[]  = 1/27/24c";
#include <B32_ESPNowLib.h>

//TTGO boards with different color pin headers
const uint8_t           aucBlackPinMAC[]          = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //Black pin headers
const uint8_t           aucWhitePinMAC[]          = {0x84, 0xCC, 0xA8, 0x60, 0xB4, 0x2C}; //White pin headers
const uint8_t           aucBluePinMAC[]           = {0x3C, 0x61, 0x05, 0x0B, 0xC5, 0x14}; //Blue  pin headers

const uint8_t           auc3dot2MAC[]             = {0xE0, 0x5A, 0x1B, 0xA2, 0x74, 0x1C}; //Walmart 3.2"
const uint8_t           aucDIYMall3dot5MAC[]      = {0xC0, 0x49, 0xEF, 0x13, 0x73, 0xDC}; //DIYMall 3.5"

const uint8_t           aucWaveShareAMAC[]        = {0x66, 0x5C, 0x4D, 0xB8, 0x87, 0x45}; //Waveshare 4.3"

uint8_t                 aucReceiverMACAddress[6];
uint8_t                 aucMyMACAddress[6];

//Define variables to store temperature readings to be sent
double                  dTCouple0_DegF;
double                  dTCouple1_DegF;
double                  dTCouple2_DegF;

long                    lAliveMsec     = 5000;
long                    lCurrentMsec   = 0;
long                    lNextMsec      = 0;

//Variable to store if sending data was successful
String                  szSuccess;

//Create a stMessageStruct to hold incoming sensor readings
stMessageStruct         stIncomingReadings;
stMessageStruct         stOutgoingReadings;
stMessageStruct         stErrorReadings;
stMessageStruct         stLastReadings;

esp_now_peer_info_t     stPeerInfo;

//Protos for functions not seen externally
void    SelectReceiverMAC     (enum eBoardPinColor ePinColor);

//Proto for data handler in the Arduino sketch .ino
void    HandleDataReceived    (void);


void SetupESPNow(void){
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("SetupESPNow(): Call to esp_now_init() failed");
    return;
  } // if(esp_now_init()!=ESP_OK)

  SelectReceiverMAC(eReceiverBoardPinColor);

  //Register display module
  memcpy(stPeerInfo.peer_addr, aucReceiverMACAddress, 6);
  stPeerInfo.channel = 0;
  stPeerInfo.encrypt = false;

  //Add peer
  if (esp_now_add_peer(&stPeerInfo) != ESP_OK){
    Serial.println("SetupESPNow(): Failed to add peer");
    return;
  }   //if(esp_now_add_peer(&stPeerInfo)!=ESP_OK)

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  //Initialize structs & create error readings for no new readings
  for(int wReading= 0; wReading < wNumReadings; wReading++){
    stIncomingReadings.adReading[wReading]=    0.00;
    stOutgoingReadings.adReading[wReading]=    0.00;
    stLastReadings    .adReading[wReading]=    0.00;
    stErrorReadings   .adReading[wReading]= -999.0;
  } //for(int wReading=0;...
  return;
}   //SetupESPNow


//Callback when data is received. Used by Display that receives from TCouple Module
void OnDataRecv(const uint8_t *pucMACAddress, const uint8_t *pucIncomingData, int wNumBytes) {
  memcpy(&stIncomingReadings, pucIncomingData, sizeof(stIncomingReadings));

  HandleDataReceived();
  return;
} //OnDataRecv


void SendDataToDisplayBoard(stMessageStruct stReadings){
   esp_err_t wResult= esp_now_send(aucReceiverMACAddress,
                                  (uint8_t *)&stReadings,
                                  sizeof(stReadings));
  if (wResult == ESP_OK) {
    Serial << "SendDataToDisplayBoard(): Sent, no guarantee if received." << endl;
  }
  else {
    Serial << "SendDataToDisplayBoard(): Error sending the data" << endl;
  }
  return;
} //SendDataToDisplayBoard


void SelectReceiverMAC(enum eBoardPinColor ePinColor){
  //Copy MAC address one byte at a time
  for (int wMACByteNum= 0; wMACByteNum < 6; wMACByteNum++){
    switch (ePinColor){
      case eBoardPinColor::eBluePin:
        aucReceiverMACAddress[wMACByteNum]= aucBluePinMAC[wMACByteNum];
        break;
      case eBoardPinColor::eWhitePin:
        aucReceiverMACAddress[wMACByteNum]= aucWhitePinMAC[wMACByteNum];
        break;
      case eBoardPinColor::eBlackPin:
        aucReceiverMACAddress[wMACByteNum]= aucBlackPinMAC[wMACByteNum];
        break;
      case eBoardPinColor::e3dot2NoPin:
        aucReceiverMACAddress[wMACByteNum]= auc3dot2MAC[wMACByteNum];
        break;
      case eBoardPinColor::eDIYMall3dot5NoPin:
        aucReceiverMACAddress[wMACByteNum]= aucDIYMall3dot5MAC[wMACByteNum];
        break;
      case eBoardPinColor::eWaveshare4dot3A:
        aucReceiverMACAddress[wMACByteNum]= aucWaveShareAMAC[wMACByteNum];
        break;
      default: 
        Serial << "SelectReceiverMAC(): Bad switch= " << (int)ePinColor << endl;
    } //switch
  } //for(int wMACByteNum=0;...

  return;
} //SelectReceiverMAC


void ResetTimer(void){
  lNextMsec= (millis() + lAliveMsec);
  return;
} //ResetTimer


void PrintReadings(stMessageStruct stReadings){
  for (int wReadingNum=0; (wReadingNum < wNumReadings); wReadingNum++) {
    Serial << "D" << wReadingNum << "= " << stReadings.adReading[wReadingNum];
    //PrintDataPoint(stReadings.adDataPoint[wReadingNum]);
    if (wReadingNum < (wNumReadings - 1)){  //Put a comma after all but last
      Serial << ", ";
    }
  } //for(int wReadingNum=0;wReadingNum<8;wReadingNum++)
  Serial << endl;
  return;
} //PrintReadings
//Last line.
