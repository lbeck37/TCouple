const char szSketchName[]  = "B32_TTGO_tutorial.ino";
const char szFileDate[]    = "11/15/23b";
#include<TFT_eSPI.h>
#include<SPI.h>
#include<Streaming.h>

TFT_eSPI  tft= TFT_eSPI();

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Hello World!", 10, 120);
  return;
} //setup


void loop() {
  // put your main code here, to run repeatedly:
  return;
} //loop
//Last line.
