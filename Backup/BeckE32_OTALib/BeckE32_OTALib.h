//BeckE32_OTALib.h, 11/17/23c, was 2/16/22a
#pragma once

void  SetupWebserver      (const char* szWebHostName);
void  HandleOTAWebserver  (void);

/*
Add the following lines to your .ino file
const char szSketchName[]  = "SketchName.ino";
const char szFileDate[]    = "11/17/23a"

#define DO_OTA    true
#if DO_OTA
  #include <BeckE32_OTALib.h>
#endif

const char* szRouterName  = "Aspot24b";
const char* szRouterPW    = "Qazqaz11";
const char* szWebHostName = "WhoKnows";

void setup(void) {
  Serial.begin(115200);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  // Start WiFi and wait for connection to the network
  WiFi.begin(szRouterName, szRouterPW);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial << endl << "setup(): Connected to " << szRouterName
         << ", IP address to connect to is " << WiFi.localIP() << endl;
  .
  .
#if DO_OTA
  Serial << "setup(): Call SetupWebServer(" << szWebHostName << ")" << endl;
  SetupWebserver(szWebHostName);
#endif
  return;
} //setup


void loop(void) {
  .
  .
#if DO_OTA
  HandleOTAWebserver();
#endif
  return;
} //loop
*/
//Last line.
