const char szSketchName[]  = "B32_LvglHelloWorld.ino";
const char szFileDate[]    = "2/1/24E";
//Use Arduino_GFX as driver for LVGL calls to write to Waveshare 800x480, 4.3" 16-bit 5-6-5 RGB

#include <B32_LVGL_Lib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif


void setup(){
  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;


#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  Serial << BLOG << " setup(): Sketch: Call SetupDisplay" << endl;
  SetupDisplay();

  Serial << BLOG << " setup(): Sketch: Call SetupLVGL" << endl;
  SetupLVGL();

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
