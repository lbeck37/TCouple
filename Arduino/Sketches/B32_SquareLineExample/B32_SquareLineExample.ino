const char szSketchName[]  = "B32_SquareLineExample.ino";
const char szFileDate[]    = "2/26/24L";
//Use Arduino_GFX as driver for LVGL calls to write to Waveshare 800x480, 4.3" 16-bit 5-6-5 RGB

#include <B32_LVGL_DriverLib.h>
#include <ui.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

#define DO_SQUARELINE   true

//Function protos
void  setup             (void);
void  loop              (void);
void  DisplayFooter     (void);

void setup(){
  Serial.begin(115200);
  delay(500);
  //Serial.println("Hello from Arduino, 2/23/24");
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << BLOG << " setup(): Call SetupDisplay" << endl;
  SetupDisplay();

  Serial << BLOG << " setup(): Call SetupLVGL" << endl;
  SetupLVGL();
  delay(100);      //I saw a crash in the printout

  Serial << BLOG << " setup(): Call DisplayFooter" << endl;
  DisplayFooter();
  lv_timer_handler();  //let the GUI do its work
  delay(2000);      //Give a chance to read the sketch, date and MAC

#if DO_SQUARELINE
  Serial << BLOG << " setup(): Call ui_init" << endl;
  ui_init();
  delay(100);
#endif  //DO_SQUARELINE

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void loop(){
#if true
  lv_timer_handler(); /* let the GUI do its work */
#endif
  //delay(200);
  return;
} //loop


void DisplayFooter(void){
  char  szLabelText[90];
  char  szMACText  [30];

  Serial << BLOG << " DisplayFooter(): Call DisplayLabel" << endl;
  sprintf(szLabelText, "%s, %s, MAC=%s", szSketchName, szFileDate, szGetMyMAC(szMACText));
  DisplayLabel(szLabelText);
  return;
} //DisplayFooter
//Last line.
