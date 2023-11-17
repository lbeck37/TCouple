const char szSketchName[]  = "B32_TTGO_tutorial.ino";
const char szFileDate[]    = "11/16/23f";
#include<TFT_eSPI.h>
#include<SPI.h>
#include<Streaming.h>

//Global values
const uint8_t   ucFont2     =  2;   //16 pixels
const uint8_t   ucFont4     =  4;   //16 pixels
const uint8_t   ucFont7     =  7;   //48 pixel, 7-segment

TFT_eSPI  tft= TFT_eSPI();

void setup() {
  int32_t         wXPos       = 10;
  int32_t         wYPos       =  0;
  // 0 is USB top     Portrait (Default)
  // 1 is USB right   Landscape
  // 2 is USB bottom  Portrait
  // 3 is USB left    Landscape
  bool            bLandscape  = true;

  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  
  tft.init();

  if(bLandscape){
    tft.setRotation(1);
  } //if(bLandscape)
  else{
    tft.setRotation(2);
  } //if(bLandscape)else

  tft.fillScreen(TFT_BLACK);
  //tft.fillScreen(0x8172);
  //tft.drawString("Hello World!", (wXPos += 10), 10);
  tft.setTextColor(TFT_RED);
  tft.drawString(szSketchName, wXPos, (wYPos +=  0), ucFont2);

  tft.setTextColor(TFT_GREEN);
  tft.drawString(szFileDate  , wXPos, (wYPos += 15), ucFont2);

  tft.setTextColor(TFT_BLUE);
  tft.drawString("Font 4"    , wXPos, (wYPos += 22), ucFont4);
  //tft.drawString("5", wXPos  , 100, ucFont7);
  return;
} //setup


void loop() {
  static int  wNumber   =  0;
  int32_t     wXPos     = 30;
  int32_t     wYPos     = 60;

  tft.setTextColor(TFT_YELLOW, TFT_BLUE);
  tft.drawString(String(wNumber++), wXPos, wYPos, ucFont7);
  return;
} //loop
//Last line.
