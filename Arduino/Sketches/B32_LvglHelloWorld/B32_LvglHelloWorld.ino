const char szSketchName[]  = "B32_LvglHelloWorld.ino";
const char szFileDate[]    = "2/2/24K";
//Use Arduino_GFX as driver for LVGL calls to write to Waveshare 800x480, 4.3" 16-bit 5-6-5 RGB

#include <B32_LVGL_Lib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

//Function protos
void  setup         (void);
void  loop          (void);
void  DisplayLabel  (void);

void setup(){
  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << BLOG << " setup(): Sketch: Call SetupDisplay" << endl;
  SetupDisplay();

  Serial << BLOG << " setup(): Sketch: Call SetupLVGL" << endl;
  SetupLVGL();

  Serial << BLOG << " setup(): Sketch: Call DisplayLabel" << endl;
  DisplayLabel();

  Serial << BLOG << " setup(): Sketch: Call Display8Meters" << endl;
  Display8Meters();

  return;
} //setup


void DisplayLabel(void){
  lv_obj_t *label         = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello Arduino! (V" GFX_STR(LVGL_VERSION_MAJOR) "." GFX_STR(LVGL_VERSION_MINOR) "." GFX_STR(LVGL_VERSION_PATCH) ")");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  return;
} //DisplayLabel


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
