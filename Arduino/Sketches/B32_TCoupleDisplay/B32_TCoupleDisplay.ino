const char szSketchName[]  = "B32_TCoupleDisplay.ino";
const char szFileDate[]    = "12/18/23e";
//Thanks to Rui Santos, https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32

//This sketch, (B32_TCoupleDisplay.ino), and B32_TCoupleModule.ino share WiFi
//communication code from the esp_now.h library.
//#include <TFT_eSPI_DriverDefines.h>
//#define B32_ILI9341             true
//#define B32_TTGO_T_DISPLAY      false
#ifndef B32_ILI9341
 #define B32_ILI9341
#endif

#include <TFT_eSPI.h>
#include <SPI.h>
#include <B32_TCoupleLib.h>

eBoardPinColor    eReceiverBoardPinColor    {eBoardPinColor::eBluePin};  //Display currently does not send anything

//Function prototypes
void  setup   (void);
void  loop    (void);

void setup(){
  Serial.begin(115200);
  delay(500);

  SetupScreen();
  SetupESP_NOW();

  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  WiFi.macAddress(aucMyMACAddress);
  ShowMyMAC(false);
  return;
} //setup


void loop(){
  //Loop doesn't have to anything, it's all driven by the receiving of data
  if (millis() > lNextMsec){
    Serial << "loop(): We haven't seen any data for over " << 
              (lAliveMsec/1000) << " seconds" << endl;
    ResetTimer();
    UpdateScreen(stErrorReadings);
  }
  return;
}//loop
//Last line.
