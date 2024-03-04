//B32_LVGL_DriverLib.h, 2/28/24b
#pragma once
#include <lvgl.h>

//Functions protos
void          SetupDisplay          (void);
void          SetupLVGL             (void);
void          DisplayLabel          (const char* szText);
char*         szGetMyMAC            (char* szBuffer);
//Last line
