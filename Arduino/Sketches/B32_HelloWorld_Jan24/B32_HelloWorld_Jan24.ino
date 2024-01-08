const char szSketchName[]  = "B32_HelloWorld_Jan24.ino";
const char szFileDate[]    = "1/7/24g";

#include <Arduino.h>
#include <Streaming.h>

const int   wOneSecond= 1000;
const int   wMaxnumber= 10;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  return;
} //setup

void loop() {
  static int  wCount= 0;

  Serial << endl << "wCount= " << wCount++ << endl;

  for (int wNumber= 0; wNumber <= wMaxnumber; wNumber++){
    Serial << "wNumber= "   << wNumber               <<
              ", Squared= " << (wNumber * wNumber)   << 
              ", Cubed= "   << (int)pow(wNumber,  3) << 
              ", 10th= "    << (int)pow(wNumber, 10) << 
              ", 50th= "    << (int)pow(wNumber, 50) << endl;
    delay(wOneSecond / 2);
  }

  delay (wOneSecond * 3);
  return;
} //loop
//Last line