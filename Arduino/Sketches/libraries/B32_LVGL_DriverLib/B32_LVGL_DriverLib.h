//B32_LVGL_DriverLib.h, 2/8/24b
#pragma once
#include <lvgl.h>
#include <Arduino_GFX_Library.h>

//extern Arduino_RGB_Display      *pDisplay;

//Functions protos
void          SetupDisplay          (void);
void          SetupLVGL             (void);
void          DisplayLabel          (const char* szText);
char*         szGetMyMAC            (char* szBuffer);
//Last line
