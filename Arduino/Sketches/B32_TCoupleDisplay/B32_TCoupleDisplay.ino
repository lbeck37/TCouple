const char szSketchName[]  = "B32_TCoupleDisplay.ino";
const char szFileDate[]    = "10/19/23c";
//Thanks to Rui Santos, https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32

//This sketch, (B32_TCoupleDisplay.ino), and B32_TCoupleModule.ino share WiFi
//communication code from the esp_now.h library.
#include <B32_TCoupleLib.h>

uint8_t aucReceiverMACAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC};   //RedPin MAC

//Function prototypes
void  setup   (void);
void  loop    (void);

void setup(){
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  SetupScreen();
  SetupESP_NOW();
  Serial << endl << "My MAC Address is- " << WiFi.macAddress() << endl;
  return;
} //setup


void loop(){
  //Loop doesn't have to anything, it's all driven by the receiving of data
  if (millis() > lNextMsec){
    Serial << "loop(): We're still here but we haven't seen any data." << endl;
    ResetTimer();
  }
  return;
}//loop
//Last line.
