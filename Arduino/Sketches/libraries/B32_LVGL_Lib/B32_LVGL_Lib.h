//B32_LVGL_Lib.h, 2/6/24c
#pragma once
#include <lvgl.h>
#include <Arduino_GFX_Library.h>

const int                   wNumTCouples    =    4;
const int                   wMaxReadings    = 1000;

//Message Structure that is used to pass data back an forth
typedef struct stMessageStructure {
  long        lSampleTimeMsec;
  double      adTCoupleDegF[wNumTCouples];
} stMessageStruct;

extern stMessageStruct      astReadings[];
extern Arduino_RGB_Display  *pDisplay;

//Functions protos
void          SaveReading           (void);
void          SetupDisplay          (void);
void          SetupLVGL             (void);
void          DisplayLabel          (const char* szText);
void          DisplayMeterArray     (uint8_t ucNumColumns, uint8_t ucNumRows, uint16_t usPercentScale);
void          DisplayMeter          (int wMeterNumber, lv_coord_t sSize   , lv_align_t ucAlignment,
                                     lv_coord_t sOffsetX, lv_coord_t sOffsetY);
char*         szGetMyMAC            (char* szBuffer);
void          set_value             (void *pIndicator, int wValue);
lv_coord_t    sPercent              (lv_coord_t sNumber, int16_t sPercent);
//Last line
