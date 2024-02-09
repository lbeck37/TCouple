const char szSketchName[]  = "B32_SquareLineExample.ino";
const char szFileDate[]    = "2/8/24f";
//Use Arduino_GFX as driver for LVGL calls to write to Waveshare 800x480, 4.3" 16-bit 5-6-5 RGB

#include <B32_LVGL_DriverLib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

//Function protos
void            setup               (void);
void            loop                (void);


void setup(){
  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  const bool bDoGraphics= true;
  if(bDoGraphics){
    Serial << BLOG << " setup(): Call SetupDisplay" << endl;
    SetupDisplay();

    Serial << BLOG << " setup(): Call SetupLVGL" << endl;
    SetupLVGL();

    char  szLabelText[90];
    char  szMACText  [30];

    Serial << BLOG << " setup(): Call DisplayLabel" << endl;
    sprintf(szLabelText, "%s, %s, MAC=%s", szSketchName, szFileDate, szGetMyMAC(szMACText));
    DisplayLabel(szLabelText);
  } //if(bDoGraphics)

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void loop(){
  lv_timer_handler(); /* let the GUI do its work */
  //delay(200);
  return;
} //loop
//Last line.
