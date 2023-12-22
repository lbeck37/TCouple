const char szSketchName[]  = "B32_TCoupleDisplay.ino";
const char szFileDate[]    = "12/22/23d";
//Thanks to Rui Santos, https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32
//This sketch, (B32_TCoupleDisplay.ino), and B32_TCoupleModule.ino share WiFi
//communication code from the esp_now.h library.

//B32_ILI9341 is defined with "-D" at end of C++ compile line,
//  Project Properties> Sloeber> Configuration> Compile Options> append to C and C++ compile line
#ifdef B32_ILI9341
  #pragma message("B32_TCoupleDisplay.ino, Include TFT_eSPI.h, B32_ILI9341 is defined")
#else
  #pragma message("B32_TCoupleDisplay.ino, Include TFT_eSPI.h, B32_ILI9341 is not defined")
#endif

#include <TFT_eSPI.h>
#include <SPI.h>
#include <B32_TCoupleLib.h>

eBoardPinColor    eReceiverBoardPinColor    {eBoardPinColor::eBluePin};  //Display currently does not send anything

#if defined(B32_TTGO_T_DISPLAY)
  const uint8_t ucRotation  = 3;  //1 WM: P, USB Top/ ED: L, USB UL/ TT: L, USB L
#endif

#if defined(B32_ILI9341)
  const uint8_t ucRotation  = 0;  //0 WM: L, USB L/ DIY: ?, USB Bot/ ED: P, USB UR/ TT: P, USB Bot
#endif

#if defined(B32_ILI9488_ESP32)
  const uint8_t ucRotation  = 0;  //0 WM: L, USB L/ DIY: ?, USB Bot/ ED: P, USB UR/ TT: P, USB Bot
#endif

//Function prototypes
void  setup   (void);
void  loop    (void);

void setup(){
  Serial.begin(115200);
  delay(500);

  SetupScreen(ucRotation);
  SetupESP_NOW();

  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  WiFi.macAddress(aucMyMACAddress);
  ShowMyMAC(false);
  return;
} //setup


void loop(){
  //Loop doesn't have to do anything but check the no-data timer, it's all driven by receiving data
  if (millis() > lNextMsec){
    Serial << "loop(): We haven't seen any data for over " << 
              (lAliveMsec/1000) << " seconds" << endl;
    ResetTimer();
    UpdateScreen(stErrorReadings);
  }
  return;
}//loop
//Last line.
