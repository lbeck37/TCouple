const char szSketchName[]  = "B32_LvglHelloWorld.ino";
const char szFileDate[]    = "2/6/24E";
//Use Arduino_GFX as driver for LVGL calls to write to Waveshare 800x480, 4.3" 16-bit 5-6-5 RGB

#include <B32_LVGL_Lib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

uint16_t  usMeterPercentScale = 100;
uint8_t   ucNumColumns        = 2;
uint8_t   ucNumRows           = 2;

//Function protos
void  setup         (void);
void  loop          (void);


void setup(){
  char  szLabelText[90];
  char  szMACText  [30];

  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << BLOG << " setup(): Call SetupDisplay" << endl;
  SetupDisplay();

  Serial << BLOG << " setup(): Call SetupLVGL" << endl;
  SetupLVGL();

  Serial << BLOG << " setup(): Call DisplayLabel" << endl;
  sprintf(szLabelText, "%s, %s, MAC=%s", szSketchName, szFileDate, szGetMyMAC(szMACText));
  DisplayLabel(szLabelText);

  Serial << BLOG << " setup(): Call DisplayMeterArray" << endl;
  DisplayMeterArray(ucNumColumns, ucNumRows, usMeterPercentScale);
  return;
} //setup


void loop(){
  lv_timer_handler(); /* let the GUI do its work */
/*
#ifdef CANVAS
  pDisplay->flush();
#endif
*/
  SaveReading();

  Serial << BLOG << " loop(): Call DisplayMeterArray" << endl;
  DisplayMeterArray(ucNumColumns, ucNumRows, usMeterPercentScale);

  delay(50);
  return;
} //loop
//Last line.
