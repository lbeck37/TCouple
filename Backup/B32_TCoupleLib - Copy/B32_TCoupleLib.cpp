//const char szFileName[]  = "B32_TCoupleLib.ino";
//const char szFileDate[]  = "10/19/23c";
#include <B32_TCoupleLib.h>

const uint8_t aucBlackPinMAC[]          = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //BlackPin MAC
const uint8_t aucESP32_ScreenOneDot[]   = {0xB4, 0xE6, 0x2D, 0xC8, 0x0F, 0x55}; //ESP32 w/3.2" LCD
const uint8_t aucESP32_ScreenTwoDot[]   = {0xB4, 0xE6, 0x2D, 0xC2, 0xC9, 0xF5}; //ESP32 w/3.2" LCD

//Define variables to store temperature readings to be sent
double                  dTCouple0_DegF;
double                  dTCouple1_DegF;
double                  dTCouple2_DegF;

long                    lAliveMsec     = 5000;
long                    lCurrentMsec   = 0;
long                    lNextMsec      = 0;

// Variable to store if sending data was successful
String                  szSuccess;

// Create a stMessageStructure to hold incoming sensor readings
stMessageStructure      stIncomingReadings;
stMessageStructure      stOutgoingReadings;

TFT_eSPI                Screen=     TFT_eSPI();  //Class library for TTGO T-Screen

esp_now_peer_info_t     stPeerInfo1;
esp_now_peer_info_t     stPeerInfo2;
esp_now_peer_info_t     stPeerInfo3;
esp_now_peer_info_t     stPeerInfo4;
esp_now_peer_info_t     stPeerInfo5;


void SetupESP_NOW(void){
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("SetupESP_NOW(): Error initializing ESP-NOW");
    return;
  } // if(esp_now_init()!=ESP_OK)

/*
  //Register display module
  memcpy(stPeerInfo.peer_addr, aucReceiverMACAddress, 6);
  stPeerInfo.channel = 0;
  stPeerInfo.encrypt = false;

  //Add peer
  if (esp_now_add_peer(&stPeerInfo) != ESP_OK){
    Serial.println("SetupESP_NOW(): Failed to add peer");
    return;
  }   //if(esp_now_add_peer(&stPeerInfo)!=ESP_OK)

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
*/
  RegisterDisplayBoard(aucBlackPinMAC,        &stPeerInfo1);
  RegisterDisplayBoard(aucESP32_ScreenOneDot, &stPeerInfo2);
  RegisterDisplayBoard(aucESP32_ScreenTwoDot, &stPeerInfo3);
  return;
}   //SetupESP_NOW


void RegisterDisplayBoard(const uint8_t         *aucReceiverMACAddress,
                          esp_now_peer_info_t   &stPeerInfo){
  //Register display module
  memcpy(stPeerInfo.peer_addr, aucReceiverMACAddress, 6);
  stPeerInfo.channel = 0;
  stPeerInfo.encrypt = false;

  //Add peer
  if (esp_now_add_peer(&stPeerInfo) != ESP_OK){
    Serial.println("RegisterDisplayBoard(): Failed to add peer");
    return;
  }   //if(esp_now_add_peer(&stPeerInfo)!=ESP_OK)

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  return;
}   //RegisterDisplayBoard


//Callback when data is received. Used by Display that receives from TCouple Module
void OnDataRecv(const uint8_t *pucMACAddress, const uint8_t *pucIncomingData, int wNumBytes) {
  memcpy(&stIncomingReadings, pucIncomingData, sizeof(stIncomingReadings));

  HandleDataReceived();
  return;
} //OnDataRecv


//Callback when data is received. Used by Display that receives from TCouple Module
void HandleDataReceived(void) {
  ResetTimer();

  PrintTemperatures();
  UpdateScreen(stIncomingReadings);
  return;
} //OnDataRecv

void SendDataToDisplayBoards(void){
  SendDataToDisplayBoard(aucBlackPinMAC);
  SendDataToDisplayBoard(aucESP32_ScreenOneDot);
  SendDataToDisplayBoard(aucESP32_ScreenTwoDot);
  return;
} //SendDataToDisplayBoards


void SendDataToDisplayBoard(const uint8_t *aucDestinationMAC){
  esp_err_t wResult= esp_now_send(aucDestinationMAC,
                                  (uint8_t *)&stOutgoingReadings,
                                  sizeof(stOutgoingReadings));
  if (wResult == ESP_OK) {
    Serial << "SendDataToDisplayBoard(): Sent, no guarantee if received." << endl;
  }
  else {
    Serial << "SendDataToDisplayBoard(): Error sending the data" << endl;
  }
  return;
} //SendDataToDisplayBoard


void SetupScreen(){
  Screen.init();
  Screen.setRotation(1);   //1= USB Right Landscape
  Screen.fillScreen(TFT_BLACK);
  return;
}//SetupScreen stMessageStructure stMessage


//void UpdateScreen(){
void UpdateScreen       (stMessageStructure stReadings){
  Screen.fillScreen     (TFT_BLACK);
  Screen.setTextColor   (TFT_GREEN,TFT_BLACK);
  Screen.setTextFont    (3);
  Screen.setCursor      (0, 0, 2);

  Screen << "My MAC= " << WiFi.macAddress() << endl;

  for (int wTCoupleNum=0; (wTCoupleNum < 5); wTCoupleNum++) {
    Screen << "T" << wTCoupleNum << "= " << stReadings.adTCoupleDegF[wTCoupleNum] << "F, T"
        << (wTCoupleNum + 3)  << "= " << stReadings.adTCoupleDegF[wTCoupleNum +3] << "F" << endl;
  } //for(int wTCoupleNum=0;(wTCoupleNum<5);wTCoupleNum++)
  return;
}   //UpdateScreen


void PrintTemperatures(void){
  for (int wTCoupleNum=0; (wTCoupleNum < wNumTCouples); wTCoupleNum++) {
    Serial << "T" << wTCoupleNum << "= ";
    PrintTemperature(stIncomingReadings.adTCoupleDegF[wTCoupleNum]);
    if (wTCoupleNum < (wNumTCouples - 1)){  //Put a comma after all but last
      Serial << ", ";
    }
  } //for(int wTCoupleNum=0;wTCoupleNum<8;wTCoupleNum++)
  Serial << endl;
  return;
} //PrintTemperatures


//Print the temperature, or the type of fault
void PrintTemperature(double dDegF) {
  //switch statement takes an integer value and executes the case that corresponds
  //Cast the double dDegF to be an int so it becomes an error code
  switch ((int)dDegF){
    case FAULT_OPEN:
      Serial << "FAULT_OPEN";
      break;
    case FAULT_SHORT_GND:
      Serial << "FAULT_SHORT_GND";
      break;
    case FAULT_SHORT_VCC:
      Serial << "FAULT_SHORT_VCC";
      break;
    case NO_MAX31855:
      Serial << "NO_MAX31855";
      break;
    default:
      Serial << dDegF;
      break;
  } //switch
}//PrintTemperature


void ResetTimer(void){
  lNextMsec= (millis() + lAliveMsec);
  return;
} //ResetTimer
//Last line.
