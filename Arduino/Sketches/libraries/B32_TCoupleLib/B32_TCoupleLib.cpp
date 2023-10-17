//const char szFileName[]  = "B32_TCoupleLib.ino";
//const char szFileDate[]  = "10/17/23b";
#include <B32_TCoupleLib.h>

//Define variables to store temperature readings to be sent
double                  dTCouple0_DegF;
double                  dTCouple1_DegF;
double                  dTCouple2_DegF;

/*
const int               wNumTCouples    = 3;
double                  adTCoupleDegF[wNumTCouples];
*/
//const int               wNumTCouples    = 8;

long                    lAliveMsec     = 5000;
long                    lCurrentMsec   = 0;
long                    lNextMsec      = 0;

// Variable to store if sending data was successful
String                  szSuccess;

// Create a stMessageStructure to hold incoming sensor readings
stMessageStructure      stIncomingReadings;
stMessageStructure      stOutgoingReadings;

TFT_eSPI                tft=     TFT_eSPI();  //Class library for TTGO T-Display

esp_now_peer_info_t     stPeerInfo;


void SetupESP_NOW(void){
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("SetupESP_NOW(): Error initializing ESP-NOW");
    return;
  } // if(esp_now_init()!=ESP_OK)

  //Register remote module
  //memcpy(stPeerInfo.peer_addr, aucRedPinMAC, 6);
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

  return;
}   //SetupESP_NOW


//Callback when data is received. Used by Display that receives from TCouple Module
void OnDataRecv(const uint8_t *pucMACAddress, const uint8_t *pucIncomingData, int wNumBytes) {
  memcpy(&stIncomingReadings, pucIncomingData, sizeof(stIncomingReadings));

/*
  ResetTimer();
  //Serial << "OnDataRecv(): Number of Bytes received= " << wNumBytes << endl;
  adTCoupleDegF[0]= stIncomingReadings.dTCouple0_DegF;
  adTCoupleDegF[1]= stIncomingReadings.dTCouple1_DegF;
  adTCoupleDegF[2]= stIncomingReadings.dTCouple2_DegF;

  PrintTemperatures();
  UpdateDisplay();
*/
  HandleDataReceived();
  return;
} //OnDataRecv


//Callback when data is received. Used by Display that receives from TCouple Module
void HandleDataReceived(void) {
  ResetTimer();
/*
  //Serial << "OnDataRecv(): Number of Bytes received= " << wNumBytes << endl;
  adTCoupleDegF[0]= stIncomingReadings.dTCouple0_DegF;
  adTCoupleDegF[1]= stIncomingReadings.dTCouple1_DegF;
  adTCoupleDegF[2]= stIncomingReadings.dTCouple2_DegF;
*/

  PrintTemperatures();
  //UpdateDisplay();
  UpdateDisplay(stIncomingReadings);
  return;
} //OnDataRecv


void SetupDisplay(){
  tft.init();
  tft.setRotation(1);   //1= USB Right Landscape
  tft.fillScreen(TFT_BLACK);
  return;
}//SetupDisplay stMessageStructure stMessage


//void UpdateDisplay(){
void UpdateDisplay(stMessageStructure stReadings){
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  //tft.setTextFont(4);
  tft.setTextFont(3);
  tft.setCursor(0, 0, 2);

  //tft << szSketchName << ", " << szFileDate << ", " << WiFi.macAddress() << endl;
  tft << "My MAC= " << WiFi.macAddress() << endl;
  //tft << szSketchName << ", " << szFileDate << endl;

  for (int wTCoupleNum=0; (wTCoupleNum < 5); wTCoupleNum++) {
    //tft.println("T", wTCoupleNum, "= ", adTCoupleDegF[wTCoupleNum]);
    //tft << "Test" << endl;
/*
    tft << "T" << wTCoupleNum << "= " << adTCoupleDegF[wTCoupleNum] << "F, T"
        << (wTCoupleNum + 3) << "= " << adTCoupleDegF[wTCoupleNum +3] << "F" << endl;
*/
/*
    tft << "T" << wTCoupleNum << "= " << stIncomingReadings.adTCoupleDegF[wTCoupleNum] << "F, T"
        << (wTCoupleNum + 3)  << "= " << stIncomingReadings.adTCoupleDegF[wTCoupleNum +3] << "F" << endl;
*/
    tft << "T" << wTCoupleNum << "= " << stReadings.adTCoupleDegF[wTCoupleNum] << "F, T"
        << (wTCoupleNum + 3)  << "= " << stReadings.adTCoupleDegF[wTCoupleNum +3] << "F" << endl;
  } //for(int wTCoupleNum=0;(wTCoupleNum<5);wTCoupleNum++)
  return;
}   //UpdateDisplay


void PrintTemperatures(void){
  for (int wTCoupleNum=0; (wTCoupleNum < wNumTCouples); wTCoupleNum++) {
    Serial << "T" << wTCoupleNum << "= ";
    //PrintTemperature(adTCoupleDegF[wTCoupleNum]);
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
