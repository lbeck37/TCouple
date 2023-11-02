const char szSketchName[]  = "B32_GetMACAddress";
const char szFileDate[]    = "11/1/23b";
//Beck, 10-13-23b, B32_GetMACAddress.ino
// Complete Instructions to Get and Change ESP MAC Address: https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/

#include "WiFi.h"
#include <Streaming.h>

void setup(){
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  WiFi.mode(WIFI_MODE_STA);
  //Serial.println(WiFi.macAddress());
  Serial << endl << "My MAC Address is- " << WiFi.macAddress() << endl;
} //setup


void loop(){
//Do nothing
} //loop
//Last line.
