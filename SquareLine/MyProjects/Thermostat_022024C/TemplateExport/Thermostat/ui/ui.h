// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.6
// Project name: Thermostat_022024C

#ifndef _THERMOSTAT_022024C_UI_H
#define _THERMOSTAT_022024C_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl-3/lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_Thermostat_Screen
void ui_Thermostat_Screen_screen_init(void);
extern lv_obj_t * ui_Thermostat_Screen;
extern lv_obj_t * ui_Header;
extern lv_obj_t * ui_Date_Header;
extern lv_obj_t * ui_Time_Header;
extern lv_obj_t * ui_Label2;
extern lv_obj_t * ui_Arc_Group;
void ui_event_Setting_Arc(lv_event_t * e);
extern lv_obj_t * ui_Setting_Arc;
extern lv_obj_t * ui_Temp_Bg;
extern lv_obj_t * ui_Temp_Num_Bg;
extern lv_obj_t * ui_Celsius_Label;
extern lv_obj_t * ui_Label_Slider;
void ui_event_Fan_Speed_Control(lv_event_t * e);
extern lv_obj_t * ui_Fan_Speed_Control;
extern lv_obj_t * ui____initial_actions0;



LV_FONT_DECLARE(ui_font_Number);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
