const char szSketchName[]  = "B32_LvglHelloWorld.ino";
const char szFileDate[]    = "2/6/24C";
//Use Arduino_GFX as driver for LVGL calls to write to Waveshare 800x480, 4.3" 16-bit 5-6-5 RGB

#include <B32_LVGL_Lib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

//Function protos
void  setup         (void);
void  loop          (void);


void setup(){
  char  szLabelText[90];
  char  szMACText  [30];

  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << BLOG << " setup(): Sketch: Call SetupDisplay" << endl;
  SetupDisplay();

  Serial << BLOG << " setup(): Sketch: Call SetupLVGL" << endl;
  SetupLVGL();

  Serial << BLOG << " setup(): Sketch: Call DisplayLabel" << endl;
  sprintf(szLabelText, "%s, %s, MAC=%s", szSketchName, szFileDate, szGetMyMAC(szMACText));
  DisplayLabel(szLabelText);

  Serial << BLOG << " setup(): Sketch: Call Display8Meters" << endl;
  uint16_t  usMeterPercentScale = 100;
  uint8_t   ucNumColumns        = 2;
  uint8_t   ucNumRows           = 2;
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
  delay(5);
  return;
} //loop
//Last line.
