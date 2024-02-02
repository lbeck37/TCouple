//B32_LVGL_Lib.h, 2/2/24b
#pragma once
#include <lvgl.h>
#include <Arduino_GFX_Library.h>

extern Arduino_RGB_Display  *pDisplay;

//Functions protos
void  SetupDisplay        (void);
void  SetupLVGL           (void);
void DisplayMeter         (void);
void set_value            (void *pIndicator, int wValue);
//Last line
