//const char szFileName[]  = "B32_TCoupleLib.cpp";
//const char szFileDate[]  = 12/16/23g";
#include <B32_TCoupleLib.h>

extern enum eBoardPinColor   eReceiverBoardPinColor;

const uint8_t           aucBlackPinMAC[]          = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //BlackPin MAC
const uint8_t           aucWhitePinMAC[]          = {0x84, 0xCC, 0xA8, 0x60, 0xB4, 0x2C}; //WhitePin MAC
const uint8_t           aucBluePinMAC[]           = {0x3C, 0x61, 0x05, 0x0B, 0xC5, 0x14}; //BluePin MAC

uint8_t                 aucReceiverMACAddress[6];
uint8_t                 aucMyMACAddress[6];

uint8_t                 auc100ByteBuffer[100];

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
stMessageStructure      stErrorReadings;

TFT_eSPI                Screen=     TFT_eSPI();  //Class library for TTGO T-Screen

esp_now_peer_info_t     stPeerInfo;

//Protos for functions not seen externally
void        SelectReceiverMAC    (enum eBoardPinColor ePinColor);


void SetupESP_NOW(void){
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("SetupESP_NOW(): Error initializing ESP-NOW");
    return;
  } // if(esp_now_init()!=ESP_OK)

  SelectReceiverMAC(eReceiverBoardPinColor);

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

  //Create error readings displayed when new readings have not been received
  for(int wReading= 0; wReading < wNumTCouples; wReading++){
    stErrorReadings.adTCoupleDegF[wReading]= -99.99;
  } //for(int wReading=0;...
  return;
}   //SetupESP_NOW


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


void SendDataToDisplayBoard(void){
   esp_err_t wResult= esp_now_send(aucReceiverMACAddress,
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
      default: 
        Serial << "SelectReceiverMAC(): Bad switch= " << (int)ePinColor << endl;
    } //switch
  } //for(int wMACByteNum=0;...

/*
  Serial << "SelectReceiverMAC(): Sending data to ";
	for (int wByteNum= 0; wByteNum < 6; wByteNum++){
	  Serial << _HEX(aucReceiverMACAddress[wByteNum]);
	  if (wByteNum != 5){
	    Serial << ":";
	  } //if (wByteNum!=5)
	} //for(int wByteNum=0;...
	Serial << endl;
*/

  return;
} //SelectReceiverMAC


/*
char acShowMAC(uint8_t acOutputBuffer[], uint8_t aucMACAddress[], bool bDisplay){
  const int   wNumBytesInMAC= 6;
  //The 6 even bytes (0, 2, 4, 6, 8, 10) in acOutputBuffer are the 6 MAC address bytes
  //The 5  odd bytes (1, 3, 5, 7, 9) between them are either a ":" or " "
  for (int wByteNum= 0; wByteNum < wNumBytesInMAC; wByteNum++){
    acOutputBuffer[(2 * wByteNum)]= _HEX(aucMACAddress[wByteNum]);
  } //for(int wByteNum=0;...

  //Fill in space, between each 2-character wide hex byte, with either a ":" or " "
  for (int wByteNum= 1; wByteNum <= 9; (wByteNum += 2)){
    if (bDisplay){
      acOutputBuffer[wByteNum]= " ";
    } //if(bDisplay)
    else {
      acOutputBuffer[wByteNum]= ":";
    } //if(bDisplay)else
  } //for(int wByteNum=0;...

  return acOutputBuffer;
} //acShowMAC
*/
/*
  Serial << "SelectReceiverMAC(): Sending data to ";
  for (int wByteNum= 0; wByteNum < 6; wByteNum++){
    Serial << _HEX(aucReceiverMACAddress[wByteNum]);
    if (wByteNum != 5){
    if (bDisplay){
      Serial << " ";
    else{

    } //if (wByteNum!=5)
  } //for(int wByteNum=0;...
  Serial << endl;
*/
void ShowMAC(uint8_t aucMACAddress[], bool bDisplay){
  const int   wNumBytesInMAC= 6;
  for (int wByteNum= 0; wByteNum < wNumBytesInMAC; wByteNum++){
    if (bDisplay){
      Screen << _HEX(aucMACAddress[wByteNum]);
      if (wByteNum != 5){
        Screen << " ";
      } //if (wByteNum!=5)
    } //if(bDisplay)
    else{
      Serial << _HEX(aucMACAddress[wByteNum]);
      if (wByteNum != 5){
        Serial << ":";
      } //if (wByteNum!=5)
    } ////if(bDisplay)else
  } //for(int wByteNum=0;...

  return;
} //ShowMAC


void SetupScreen(){
  Screen.init();
  //Screen.setRotation(1);   //1= USB Right Landscape
  Screen.setRotation(3);   //3= USB Left Landscape
  Screen.fillScreen(TFT_BLACK);
  return;
}//SetupScreen stMessageStructure stMessage


 void UpdateScreen(stMessageStructure stReadings){
  Screen.fillScreen     (TFT_BLACK);
  Screen.setTextColor   (TFT_GREEN,TFT_BLACK);
  //Screen.setTextFont    (3);
  Screen.setTextFont    (5);
  Screen.setTextSize    (1);
  Screen.setCursor      (0, 0, 2);

  //Screen << "UpdateScreen(): My MAC= " << WiFi.macAddress() << endl;
  Screen << szSketchName << " " << szFileDate << endl;
  //Screen << "My MAC= " << WiFi.macAddress() << endl;
  //Screen << "My MAC= " << acShowMAC(auc100ByteBuffer, uint8_t aucMACAddress[], true) << endl;
  Screen << "My MAC is ";
  ShowMAC(aucMyMACAddress, true);
  Screen<< endl;

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
