//B32_LVGL_DriverLib.h, 2/8/24c
#pragma once
#include <lvgl.h>
#include <Arduino_GFX_Library.h>

//Functions protos
void          SetupDisplay          (void);
void          SetupLVGL             (void);
void          DisplayLabel          (const char* szText);
char*         szGetMyMAC            (char* szBuffer);
//Last line
