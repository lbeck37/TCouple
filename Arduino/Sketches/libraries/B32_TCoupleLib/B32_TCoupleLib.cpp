//const char szFileName[]  = "B32_TCoupleLib.cpp";
//const char szFileDate[]  = 1/26/24b was 12/29/23b";
#include <B32_TCoupleLib.h>
#include <Free_Fonts.h>
#include <Targa15pt7b.h>
//#include <Monofonto20pt7b.h>
#include <Monofonto15pt7b.h>
//#include <gabriele_bad15pt7b.h>
//#include <ObliviousFont15pt7b.h>
#include <string>
#include <stdio.h>

#define DO_TOUCHSCREEN      false

extern enum eBoardPinColor   eReceiverBoardPinColor;

extern char             aucLeftLabel [][wMaxLabelChars];
extern char             aucRightLabel[][wMaxLabelChars];

const uint8_t           aucBlackPinMAC[]          = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //TTGO w/Black pin headers
const uint8_t           aucWhitePinMAC[]          = {0x84, 0xCC, 0xA8, 0x60, 0xB4, 0x2C}; //TTGO w/White pin headers
const uint8_t           aucBluePinMAC[]           = {0x3C, 0x61, 0x05, 0x0B, 0xC5, 0x14}; //TTGO w/Blue  pin headers
const uint8_t           auc3dot2MAC[]             = {0xE0, 0x5A, 0x1B, 0xA2, 0x74, 0x1C}; //Walmart 3.2"
const uint8_t           aucDIYMall3dot5MAC[]      = {0xC0, 0x49, 0xEF, 0x13, 0x73, 0xDC}; //DIYMall 3.5" w/capacitive touch

uint8_t                 aucReceiverMACAddress[6];
uint8_t                 aucMyMACAddress[6];

//ESP32 GPIO pin numbers (range from 0 to 39)
const uint8_t           TCoupleSelect0            = 27;
const uint8_t           TCoupleSelect1            = 26;
const uint8_t           TCoupleSelect2            = 25;
const uint8_t           TCouple_MISO              = 32;
const uint8_t           TCouple_CS                = 17;
const uint8_t           TCouple_SCK               = 33;

const bool              bNoTCouples               = true;
double                  dJunctionDegF;
double                  dDummyJunctionDegF        = 60.00;   //3 ED: L, USB LR/
double                  dDummyAddDegF             =  0.00;

//Define variables to store temperature readings to be sent
double                  dTCouple0_DegF;
double                  dTCouple1_DegF;
double                  dTCouple2_DegF;

long                    lAliveMsec     = 5000;
long                    lCurrentMsec   = 0;
long                    lNextMsec      = 0;

//Variable to store if sending data was successful
String                  szSuccess;

//Create a stMessageStructure to hold incoming sensor readings
stMessageStructure      stIncomingReadings;
stMessageStructure      stOutgoingReadings;
stMessageStructure      stErrorReadings;
stMessageStructure      stLastReadings;

MAX31855                TCoupleObject = MAX31855(TCouple_MISO, TCouple_CS, TCouple_SCK);
TFT_eSPI                Screen        = TFT_eSPI();

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

  PrintTemperatures(stIncomingReadings);
  DisplayData(stIncomingReadings);
  return;
} //OnDataRecv


void SendDataToDisplayBoard(stMessageStructure stReadings){
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


//bool  bCheckTouch(int16_t &sXCoordinate, int16_t &sYCoordinate){
bool  bCheckTouch(uint16_t *pusXCoordParam, uint16_t *pusYCoordParam){
  bool bScreenTouched   = false;

#if DO_TOUCHSCREEN
  if (Screen.getTouch(pusXCoordParam, pusYCoordParam)){
    bScreenTouched= true;
    //Screen << "bCheckTouch(): Screen touched at: " << usYCoordinate << ", " << usXCoordinate << endl;
    Screen << "bCheckTouch(): Screen touched at: " << *pusXCoordParam << ", " << *pusXCoordParam << endl;
  }
#endif  //DO_TOUCHSCREEN

  return bScreenTouched;
} //bCheckTouch


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
      default: 
        Serial << "SelectReceiverMAC(): Bad switch= " << (int)ePinColor << endl;
    } //switch
  } //for(int wMACByteNum=0;...

  return;
} //SelectReceiverMAC


void ShowMyMAC(bool bDisplay){
  const int   wNumBytesInMAC= 6;

  if (bDisplay){
    Screen << "My MAC- ";
    for (int wByteNum= 0; wByteNum < wNumBytesInMAC; wByteNum++){
      Screen << _HEX(aucMyMACAddress[wByteNum]);
      if (wByteNum != 5){
        Screen << " ";
      } //if (wByteNum!=5)
    } //for(int wByteNum=0;...
    Screen << endl;
  } //if(bDisplay)
  else{
    Serial << "My MAC- ";
    for (int wByteNum= 0; wByteNum < wNumBytesInMAC; wByteNum++){
      Serial << _HEX(aucMyMACAddress[wByteNum]);
      if (wByteNum != 5){
        Serial << ":";
      } //if (wByteNum!=5)
    } //for(int wByteNum=0;...
    Serial << endl;
  } //if(bDisplay)else

  return;
} //ShowMyMAC


void PrintMyMAC(void){
  bool bOnDisplay= false;
  ShowMyMAC(bOnDisplay);

  return;
} //PrintMyMAC


void SetupScreen(uint8_t ucRotation){
  //A is WalMart 3.2", B is ElectroDragon 3.2", C is DIYMall 3.5" touchscreen, D is TTGO_Display

  //Screen.setRotation  (0);  //0 WM: L, USB L  / DIY: P, USB Bot/ ED: P, USB UR/ TT: P, USB Bot
  //Screen.setRotation  (1);  //1 WM: P, USB Top/ DIY: L, USB R  / ED: L, USB UL/ TT: L, USB R
  //Screen.setRotation  (2);  //2 ED: P  USB LL /
  //Screen.setRotation  (3);  //3 ED: L, USB LR / DIY: L, USB L  / TT: L, USB L
  //Screen.setRotation  (4);  //4 ED: P, USB LL, text right-to-left
  //Screen.fillScreen   (LB_BLACK);

  Screen.init         ();
  Screen.setRotation  (ucRotation);
  Screen.fillScreen   (TFT_BLACK);

  Screen.setTextColor   (TFT_GREEN,TFT_BLACK);
  Screen.setTextSize    (1);
  Screen.setCursor      (0, 0);

#ifdef B32_TTGO_T_DISPLAY
  Screen.setTextFont    (3);
#else
  //Screen.setFreeFont    (&Targa15pt7b);
  Screen.setTextFont    (2);
#endif  //B32_TTGO_T_DISPLAY

  Screen << szSketchName << " " << szFileDate << endl;
  ShowMyMAC(true);
  return;
}//SetupScreen


void UpdateTTGOScreen(stMessageStructure stReadings){
 Screen.fillScreen     (TFT_BLACK);
 //Screen.setTextColor   (TFT_GREEN,TFT_BLACK);
 Screen.setTextColor   (TFT_YELLOW,TFT_BLACK);
 Screen.setTextFont    (3);
 //Screen.setTextFont    (5);
 //Screen.setTextFont    (4);
 Screen.setTextSize    (1);
 Screen.setCursor      (0, 0, 2);

 Screen << szSketchName << " " << szFileDate << endl;
 ShowMyMAC(true);

 for (int wTCoupleNum=0; (wTCoupleNum < 5); wTCoupleNum++) {
   Screen << "T" << wTCoupleNum << "= " << stReadings.adTCoupleDegF[wTCoupleNum] << "F, T"
       << (wTCoupleNum + 3)  << "= " << stReadings.adTCoupleDegF[wTCoupleNum +3] << "F" << endl;
 } //for(int wTCoupleNum=0;(wTCoupleNum<5);wTCoupleNum++)
 return;
}   //UpdateTTGOScreen


void DisplayLabels(void){
#ifdef B32_TTGO_T_DISPLAY
  Screen.setTextFont    (3);
#else
  Screen.setFreeFont    (&Targa15pt7b);
#endif  //B32_TTGO_T_DISPLAY

  //Screen.setTextColor(TFT_YELLOW, TFT_BLACK);
  //Screen.setTextColor(TFT_ORANGE, TFT_BLACK);
  Screen.setTextColor(TFT_CYAN, TFT_BLACK);
  for (int wLineNum= 0; wLineNum < 4; wLineNum++) {
    Screen.setCursor(sLeftLabelX, (sLeftLabelFirstY + (wLineNum * sLineSpacing)));
    Screen << aucLeftLabel[wLineNum];

    Screen.setCursor(sRightLabelX, (sRightLabelFirstY + (wLineNum * sLineSpacing)));
    Screen << aucRightLabel[wLineNum];
  } //for(int wLineNum= 0...
  return;
} //DisplayLabels


void DisplayData(stMessageStructure stReadings){
  char    ac100ByteBuffer[100];

#ifdef B32_TTGO_T_DISPLAY
 Screen.setTextFont    (3);
#else
 //Screen.setFreeFont    (&Targa15pt7b);
 Screen.setFreeFont    (&Monofonto15pt7b);
 //Screen.setFreeFont    (&gabriele_bad15pt7b);
 //Screen.setFreeFont    (&ObliviousFont15pt7b);
#endif  //B32_TTGO_T_DISPLAY

 Screen.setTextColor(TFT_YELLOW,TFT_BLACK);

 for (int wLineNum= 0; wLineNum < 4; wLineNum++) {
   int wLeftIndex   = wLineNum;
   int wRightIndex  = wLineNum + 4;
   if (stLastReadings.adTCoupleDegF[wLeftIndex] != stReadings.adTCoupleDegF[wLeftIndex]){
     Screen.setCursor(sLeftDataX, (sLeftDataFirstY + (wLineNum * sLineSpacing)));
     Screen.setTextColor(TFT_BLACK,TFT_BLACK);
     //Screen << stLastReadings.adTCoupleDegF[wLeftIndex];
     //Screen.printf("6.2f", stLastReadings.adTCoupleDegF[wLeftIndex]);
     sprintf(ac100ByteBuffer, "%5.0ff", stLastReadings.adTCoupleDegF[wLeftIndex]);
     Screen << ac100ByteBuffer;

     Screen.setCursor(sLeftDataX, (sLeftDataFirstY + (wLineNum * sLineSpacing)));
     Screen.setTextColor(TFT_YELLOW,TFT_BLACK);
     //Screen << stReadings.adTCoupleDegF[wLeftIndex] << "F";
     //Screen.printf("6.2f", stLastReadings.adTCoupleDegF[wLeftIndex]);
     sprintf(ac100ByteBuffer, "%5.0ff", stReadings.adTCoupleDegF[wLeftIndex]);
     Screen << ac100ByteBuffer;
   }

   if (stLastReadings.adTCoupleDegF[wRightIndex] != stReadings.adTCoupleDegF[wRightIndex]){
     //Screen.setCursor(sRightDataX, ((sLeftLabelFirstY + 2 * sLineSpacing) + (wLineNum * sLineSpacing)));
     Screen.setCursor(sRightDataX, (sRightDataFirstY + (wLineNum * sLineSpacing)));
     Screen.setTextColor(TFT_BLACK,TFT_BLACK);
     //Screen << stLastReadings.adTCoupleDegF[wRightIndex];
     sprintf(ac100ByteBuffer, "%5.0ff", stLastReadings.adTCoupleDegF[wRightIndex]);
     Screen << ac100ByteBuffer;

     Screen.setCursor(sRightDataX, (sRightDataFirstY + (wLineNum * sLineSpacing)));
     Screen.setTextColor(TFT_YELLOW,TFT_BLACK);
     //Screen << stReadings.adTCoupleDegF[wRightIndex] << "F";
     sprintf(ac100ByteBuffer, "%5.0ff", stReadings.adTCoupleDegF[wRightIndex]);
     Screen << ac100ByteBuffer;
   }
 } //for(int wLineNum= 0...
 for (int wTCoupleNum= 0; wTCoupleNum < wNumTCouples; wTCoupleNum++) {
   stLastReadings.adTCoupleDegF[wTCoupleNum]= stReadings.adTCoupleDegF[wTCoupleNum];
 }
 return;
}   //DisplayData


 void ReadAmbiant(void){
   if (!bNoTCouples){
     dJunctionDegF= TCoupleObject.readJunction(FAHRENHEIT);
   }   //if(!bNoTCouples)
   else{
     dDummyJunctionDegF += 0.10;
     dJunctionDegF= dDummyJunctionDegF;
   }   //if(!bNoTCouples)else
   return;
 } //ReadAmbiant


 void ReadTCouples(stMessageStructure& stReadings){
   //Read the temperatures of the 8 thermocouples
   for (int wTCoupleNum=0; (wTCoupleNum < wNumTCouples); wTCoupleNum++) {
     //Select the thermocouple
     digitalWrite(TCoupleSelect0, wTCoupleNum & 1? HIGH: LOW);
     digitalWrite(TCoupleSelect1, wTCoupleNum & 2? HIGH: LOW);
     digitalWrite(TCoupleSelect2, wTCoupleNum & 4? HIGH: LOW);
     //The MAX31855 takes 100ms to sample the TCouple.
     //Wait a bit longer to be safe.  We'll wait 0.125 seconds
     delay(125);

     if (!bNoTCouples){
       stReadings.adTCoupleDegF[wTCoupleNum]= TCoupleObject.readThermocouple(FAHRENHEIT);
     } //if(!bNoTCouples)
     else{
       dDummyAddDegF += 0.10;
       stReadings.adTCoupleDegF[wTCoupleNum]= (100.00 + (wTCoupleNum * 10.00) + dDummyAddDegF);
     } //if(!bNoTCouples)else

     if (stReadings.adTCoupleDegF[wTCoupleNum] == FAULT_OPEN){
       //Break out of for loop, go to top of for loop and next TCouple
       continue;
     } //if(stReadings.adTCoupleDegF[wTCoupleNum]==FAULT_OPEN)
   } //for(int wTCoupleNum=0;wTCoupleNum<8;wTCoupleNum++)
   return;
 }   //ReadTCouples


 void PrintTemperatures(stMessageStructure stReadings){
   for (int wTCoupleNum=0; (wTCoupleNum < wNumTCouples); wTCoupleNum++) {
     Serial << "T" << wTCoupleNum << "= ";
     PrintTemperature(stReadings.adTCoupleDegF[wTCoupleNum]);
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


void SetupPins(void){
  pinMode(TCoupleSelect0, OUTPUT);
  pinMode(TCoupleSelect1, OUTPUT);
  pinMode(TCoupleSelect2, OUTPUT);

  pinMode(TCouple_MISO,   INPUT);
  pinMode(TCouple_CS,     OUTPUT);
  pinMode(TCouple_SCK,    OUTPUT);

  delay(200);
  return;
} //SetupPins


void ResetTimer(void){
  lNextMsec= (millis() + lAliveMsec);
  return;
} //ResetTimer
//Last line.
