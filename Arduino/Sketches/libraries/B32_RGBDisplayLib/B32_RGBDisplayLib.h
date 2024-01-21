//B32_RGBDisplay.h, 1/20/24a
#pragma once
#include <Arduino_GFX_Library.h>

extern const uint16_t         usPanelWidth;
extern const uint16_t         usPanelHeight;

extern Arduino_RGB_Display    *pRGBDisplay;

void  CreateRGBDisplay    (void);

//Last line
