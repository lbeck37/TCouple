const char szFileName[]  = "B32_TCoupleLib.ino";
const char szFileDate[]  = "10/16/23b";
#include <B32_TCoupleLib.h>

#if RED_PIN_RECEIVER
  //Running on BlackPin TTGO, sends data to RedPin TTGO
  uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC};   //RedPin MAC
#endif
#if BLACK_PIN_RECEIVER
  //Running on RedPin TTGO, sends data to BlackPin TTGO
  uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C};   //BlackPin MAC
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
double      dTCouple0_DegF;
double      dTCouple1_DegF;
double      dTCouple2_DegF;

const int   wNumTCouples    = 3;
double      adTCoupleDegF[wNumTCouples];

long        lAliveMsec     = 5000;
long        lCurrentMsec   = 0;
long        lNextMsec      = 0;

// Variable to store if sending data was successful
String      szSuccess;

// Create a stMessageStructure to hold incoming sensor readings
stMessageStructure      stIncomingReadings;
stMessageStructure      stOutgoingReadings;

TFT_eSPI                tft=     TFT_eSPI();  //Class library for TTGO T-Display

esp_now_peer_info_t     stPeerInfo;


void ResetTimer(void){
  lNextMsec= millis() + lAliveMsec;
  return;
} //ResetTimer


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
  memcpy(stPeerInfo.peer_addr, broadcastAddress, 6);
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

  ResetTimer();
  //Serial << "OnDataRecv(): Number of Bytes received= " << wNumBytes << endl;
  adTCoupleDegF[0]= stIncomingReadings.dTCouple0_DegF;
  adTCoupleDegF[1]= stIncomingReadings.dTCouple1_DegF;
  adTCoupleDegF[2]= stIncomingReadings.dTCouple2_DegF;

  PrintTemperatures();
  UpdateDisplay();
  return;
} //OnDataRecv


void SetupDisplay(){
#if WITH_DISPLAY
  // Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
#endif  //WITH_DISPLAY
  tft.init();
  tft.setRotation(1);   //1= USB Right Landscape
  tft.fillScreen(TFT_BLACK);
  return;
}//SetupDisplay


void UpdateDisplay(){
#if WITH_DISPLAY
  // Display Readings on OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("INCOMING READINGS");
  display.setCursor(0, 15);
  display.print("Temperature: ");
  display.print(dIncomingTCouple0_DegF);
  display.cp437(true);
  display.write(248);
  display.print("C");
  display.setCursor(0, 25);
  display.print("Humidity: ");
  display.print(dIncomingTCouple1_DegF);
  display.print("%");
  display.setCursor(0, 35);
  display.print("Pressure: ");
  display.print(dIncomingTCouple2_DegF);
  display.print("hPa");
  display.setCursor(0, 56);
  display.print(szSuccess);
  display.display();
#endif  //WITH_DISPLAY
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  //tft.setTextFont(4);
  tft.setTextFont(3);
  tft.setCursor(0, 0, 2);
  tft << "Thermocouple Temperatures" << endl;

  for (int wTCoupleNum=0; (wTCoupleNum < 5); wTCoupleNum++) {
    //tft.println("T", wTCoupleNum, "= ", adTCoupleDegF[wTCoupleNum]);
    //tft << "Test" << endl;
    tft << "T" << wTCoupleNum << "= " << adTCoupleDegF[wTCoupleNum] << "F, T"
        << (wTCoupleNum + 3) << "= " << adTCoupleDegF[wTCoupleNum +3] << "F" << endl;
  }
  return;
}   //UpdateDisplay


void PrintTemperatures(void){
  for (int wTCoupleNum=0; (wTCoupleNum < wNumTCouples); wTCoupleNum++) {
    Serial << "T" << wTCoupleNum << "= ";
    PrintTemperature(adTCoupleDegF[wTCoupleNum]);
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
//Last line.
