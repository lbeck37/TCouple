const char szSketchName[]  = "B32_TTGO_tutorial.ino";
const char szFileDate[]    = "11/16/23M";
#include <TFT_eSPI.h>
#include <SPI.h>
#include <Streaming.h>
#include "HousePic128x128.h"

//Global values
const uint8_t   ucFont2         =  2;   //16 pixels
const uint8_t   ucFont4         =  4;   //16 pixels
const uint8_t   ucFont7         =  7;   //48 pixel, 7-segment
const uint8_t   ucPortrait      =  2;   //Portrait : USB Top
const uint8_t   ucLandscape     =  1;   //Landscape: USB Right
const int       wTutorialNumber =  3;

TFT_eSPI  tft= TFT_eSPI();

void setup            (void);
void loop             (void);
void Tutorial1Setup   (void);
void Tutorial1Loop    (void);
void Tutorial2Setup   (void);
void Tutorial2Loop    (void);
void Tutorial3Setup   (void);
void Tutorial3Loop    (void);

void setup() {
  // 0 is USB bottom  Portrait (Default)
  // 1 is USB right   Landscape
  // 2 is USB top     Portrait
  // 3 is USB left    Landscape
  //bool  bLandscape  = false;

  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  switch (wTutorialNumber){
    case 1:
      Tutorial1Setup();
      break;
    case 2:
      Tutorial2Setup();
      break;
    case 3:
      Tutorial3Setup();
      break;
    default:
      Serial << "setup(): Bad switch = " << wTutorialNumber << endl;
  }//switch
  return;
} //setup


void loop() {
  switch (wTutorialNumber){
    case 1:
      Tutorial1Loop();
      break;
    case 2:
      Tutorial2Loop();
      break;
    case 3:
      Tutorial3Loop();
      break;
    default:
      Serial << "loop(): Bad switch = " << wTutorialNumber << endl;
  }//switch
  return;
} //loop


void Tutorial1Setup(void){
  int32_t         wXPos       = 10;
  int32_t         wYPos       =  0;

  tft.init();
  tft.fillScreen  (TFT_BLACK);
  tft.setRotation (ucPortrait);
  tft.setTextColor(TFT_WHITE);
  tft.drawString  (szSketchName, wXPos, (wYPos +=  0), ucFont2);

  tft.setTextColor(TFT_GREEN);
  tft.drawString  (szFileDate  , wXPos, (wYPos += 15), ucFont2);

  tft.setTextColor(TFT_BLUE);
  tft.drawString  ("Font 4"    , wXPos, (wYPos += 22), ucFont4);
  return;
} //Tutorial1Setup


void Tutorial1Loop() {
  static int  wNumber   =  0;
  int32_t     wXPos     =  0;
  int32_t     wYPos     = 60;

  tft.setTextColor(TFT_YELLOW, TFT_BLUE);
  tft.drawString(String(wNumber++), wXPos, wYPos, ucFont7);
  return;
} //Tutorial1Loop


void Tutorial2Setup(void){
  int32_t         wXPos       =  0;
  int32_t         wYPos       =  0;

  tft.init();
  tft.fillScreen  (TFT_BLACK);
  tft.setRotation (ucPortrait);
  tft.setTextColor(TFT_WHITE);
  tft.drawString  (szSketchName, wXPos, (wYPos +=  0), ucFont2);

  tft.setTextColor(TFT_GREEN);
  tft.drawString  (szFileDate  , wXPos, (wYPos += 15), ucFont2);

  tft.drawLine    (0, 30, 100, 100, TFT_WHITE);
  tft.drawLine    (100, 100, 140, 30, TFT_BLUE);

  tft.drawCircle  (50, 140, 30, TFT_YELLOW);
  tft.fillCircle  (80, 200, 20, TFT_YELLOW);

  tft.drawRect    (0, 40, 30, 120, TFT_WHITE);
  tft.fillRect    (5, 130, 40, 40, TFT_PURPLE);

  return;
} //Tutorial2Setup


void Tutorial2Loop() {
  return;
} //Tutorial2Loop


void Tutorial3Setup(void){
  int32_t         wXPos       =  0;
  int32_t         wYPos       =  0;

  tft.init();
  tft.fillScreen  (TFT_BLACK);
  tft.setRotation (ucPortrait);

  tft.setTextColor(TFT_WHITE);
  tft.drawString  (szSketchName, wXPos, (wYPos +=  0), ucFont2);

  tft.setTextColor(TFT_GREEN);
  tft.drawString  (szFileDate  , wXPos, (wYPos += 15), ucFont2);

  tft.setSwapBytes(true);
  wYPos= 40;
  tft.pushImage(wXPos, wYPos, 128, 128, HousePic128x128);

  return;
} //Tutorial3Setup


void Tutorial3Loop() {

  return;
} //Tutorial3Loop
//Last line.
