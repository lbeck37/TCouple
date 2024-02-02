const char szSketchName[]  = "B32_LVGL_MeterExample.ino";
const char szFileDate[]    = "2/2/24C";

#include <lvgl.h>
//#include "../../lv_examples.h"
#include <Streaming.h>

static lv_obj_t *pMeter;

 //Function protos
void setup                (void);
void loop                 (void);
void lv_example_meter_1   (void);
void set_value            (lv_meter_indicator_t *pIndicator, int32_t wValue);

void setup(){
  Serial.begin( 115200 ); /* prepare for possible serial debug */
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  lv_example_meter_1();
  return;
} //setup


void loop(void){
  //Do nothing
  return;
} //loop


/**
 * A simple meter
 */
void lv_example_meter_1(void){
  pMeter= lv_meter_create(lv_scr_act());

  lv_obj_center                       (pMeter);
  lv_obj_set_size                     (pMeter, 200, 200);

  /*Add a scale first*/
  lv_meter_scale_t    *pScale= lv_meter_add_scale(pMeter);

  lv_meter_set_scale_ticks            (pMeter, pScale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks      (pMeter, pScale, 8, 4, 15, lv_color_black(), 10);

  lv_meter_indicator_t *pIndicator;

  /*Add a blue arc to the start*/
  pIndicator = lv_meter_add_arc(pMeter, pScale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
  lv_meter_set_indicator_start_value  (pMeter, pIndicator, 0);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, 20);

  /*Make the tick lines blue at the start of the scale*/
  pIndicator= lv_meter_add_scale_lines(pMeter, pScale, lv_palette_main(LV_PALETTE_BLUE),
                                       lv_palette_main(LV_PALETTE_BLUE), false, 0);
  lv_meter_set_indicator_start_value  (pMeter, pIndicator, 0);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, 20);

  /*Add a red arc to the end*/
  pIndicator= lv_meter_add_arc        (pMeter, pScale, 3, lv_palette_main(LV_PALETTE_RED), 0);
  lv_meter_set_indicator_start_value  (pMeter, pIndicator, 80);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, 100);

  /*Make the tick lines red at the end of the pScale*/
  pIndicator= lv_meter_add_scale_lines(pMeter, pScale, lv_palette_main(LV_PALETTE_RED),
                                       lv_palette_main(LV_PALETTE_RED), false, 0);
  lv_meter_set_indicator_start_value  (pMeter, pIndicator, 80);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, 100);

  /*Add a needle line indicator*/
  pIndicator= lv_meter_add_needle_line(pMeter, pScale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

  /*Create an animation to set the value*/
  lv_anim_t   stAnimation;
  lv_anim_init                  (&stAnimation);
  lv_anim_set_exec_cb           (&stAnimation, set_value);
  lv_anim_set_var               (&stAnimation, pIndicator);
  lv_anim_set_values            (&stAnimation, 0, 100);
  lv_anim_set_time              (&stAnimation, 2000);
  lv_anim_set_repeat_delay      (&stAnimation, 100);
  lv_anim_set_playback_time     (&stAnimation, 500);
  lv_anim_set_playback_delay    (&stAnimation, 100);
  lv_anim_set_repeat_count      (&stAnimation, LV_ANIM_REPEAT_INFINITE);
  lv_anim_start                 (&stAnimation);

  return;
} //lv_example_meter_1


//static void set_value(void * indic, int32_t v)
/*
void set_value(void * indic, int32_t v){
  lv_meter_set_indicator_value(meter, indic, v);
  return;
} //set_value
*/
void set_value(lv_meter_indicator_t *pIndicator, int32_t wValue){
  lv_meter_set_indicator_value(pMeter, pIndicator, wValue);
  return;
} //set_value
//Last line.
