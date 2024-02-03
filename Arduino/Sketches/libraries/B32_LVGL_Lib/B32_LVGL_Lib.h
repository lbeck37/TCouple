//B32_LVGL_Lib.h, 2/2/24k
#pragma once
#include <lvgl.h>
#include <Arduino_GFX_Library.h>

extern Arduino_RGB_Display  *pDisplay;

//Functions protos
void          SetupDisplay          (void);
void          SetupLVGL             (void);
void          DisplayLabel          (const char* szText);
void          Display8Meters        (int wPercentScale);
void          DisplayMeter          (lv_coord_t sSize   , lv_align_t ucAlignment,
                                     lv_coord_t sOffsetX, lv_coord_t sOffsetY);
char*         szGetMyMAC            (char* szBuffer);
void          set_value             (void *pIndicator, int wValue);
lv_coord_t    sPercent              (lv_coord_t sNumber, int16_t sPercent);
//Last line
