const char szSketchName[]  = "B32_LVGL_MeterExample.ino";
const char szFileDate[]    = "2/2/24G"; //not currently working

#include <lvgl.h>
#include <B32_LVGL_Lib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

 //Function protos
void setup                (void);
void loop                 (void);


void setup(){
  Serial.begin( 115200 ); /* prepare for possible serial debug */
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << BLOG << " setup(): Sketch: Call SetupDisplay" << endl;
  SetupDisplay();

  Serial << BLOG << " setup(): Call SetupLVGL" << endl;
  SetupLVGL();

  Serial << BLOG << " setup(): Call DisplayMeter" << endl;
  DisplayMeter();

  Serial << BLOG << " setup(): Done." << endl;
  return;
} //setup


void loop(void){
  //Do nothing
  return;
} //loop
//Last line.
