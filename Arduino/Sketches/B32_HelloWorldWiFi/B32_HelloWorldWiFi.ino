const char szSketchName[]  = "B32_HelloWorldWiFi.ino";
const char szFileDate[]    = "2/23/24b";
#include "Arduino.h"
#include <Streaming.h>
#include <WiFi.h>

//const char*     szRouterName  = "Aspot24b";
const char*     szRouterName  = "Lspot";
const char*     szRouterPW    = "Qazqaz11";

void setup(void){
  Serial.begin(115200);
  delay(500);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << "WiFiSetup(): Call WiFi.begin(" <<
            szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);

  Serial << "setup(): Call WiFi.status()";
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial << ".";
  } //while
  Serial << endl;

  Serial << endl << "WiFiSetup(): Connected to " << szRouterName
        << ", IP address to connect to is " << WiFi.localIP() << endl;

  return;
} //setup


void loop(void){
  //Do nothing.
  return;
} //loop
//Last line.
