const char szSketchName[]  = "B32_TCoupleDisplay.ino";
const char szFileDate[]    = "12/26/23H";
//Thanks to Rui Santos, https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32
//This sketch, (B32_TCoupleDisplay.ino), and B32_TCoupleModule.ino share WiFi
//communication code from the esp_now.h library.

#include <TFT_eSPI.h>
//#include <SPI.h>
#include <B32_TCoupleLib.h>

eBoardPinColor    eReceiverBoardPinColor    {eBoardPinColor::eBluePin};  //Display currently does not send anything

//char     aucLeftLabel [][wMaxLabelChars]  = {"Head 1", "Head 2", "Head 3" , "Head 4"};
//char     aucRightLabel[][wMaxLabelChars]  = {"Oil 1" , "Oil 2" , "Heater" , "Air"};
char     aucLeftLabel [][wMaxLabelChars]  = {"Cyl 1", "Cyl 2", "Cyl 3" , "Cyl 4"};
char     aucRightLabel[][wMaxLabelChars]  = {"Oil In" , "Oil Out" , "Heater" , "Air"};

// ucRotation mapping is in SetupScreen() definition in B32_TCoupleLib.cpp
#if defined(B32_TTGO_T_DISPLAY)
  const uint8_t ucRotation  = 3;
#endif

#if defined(B32_ILI9341)
  const uint8_t ucRotation  = 0;
#endif

#if defined(B32_ILI9488_ESP32)
  const uint8_t ucRotation  = 3;    //Landscape, USB on the left
#endif

//Function prototypes
void  setup   (void);
void  loop    (void);

void setup(){
  Serial.begin(115200);
  delay(500);

  SetupESP_NOW();

  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  WiFi.macAddress(aucMyMACAddress);
  //bool bOnDisplay= false;
  //ShowMyMAC(bOnDisplay);
  PrintMyMAC();
  SetupScreen(ucRotation);
  DisplayLabels();
  return;
} //setup


void loop(){
  uint16_t     usXCoordinate;
  uint16_t     usYCoordinate;

  uint16_t     *pusXCoordinate;
  uint16_t     *pusYCoordinate;

  pusXCoordinate  = &usXCoordinate;
  pusYCoordinate  = &usYCoordinate;

  bCheckTouch(pusXCoordinate, pusYCoordinate);

  //Loop doesn't do anything to get data but check the no-data timer, it's all driven by receiving data
  if (millis() > lNextMsec){
    Serial << "loop(): We haven't seen any data for over " << 
              (lAliveMsec/1000) << " seconds" << endl;
    ResetTimer();
    DisplayData(stErrorReadings);
  }
  return;
}//loop
//Last line.
