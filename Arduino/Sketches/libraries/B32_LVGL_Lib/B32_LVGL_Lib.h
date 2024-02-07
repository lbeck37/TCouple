//B32_LVGL_Lib.h, 2/7/24d
#pragma once
#include <lvgl.h>
#include <Arduino_GFX_Library.h>

const int     wNumTCouples      =    4;
const int     wMaxReadings      = 1000;
//const double  dMeterPeriodSec[] = {20.00, 10.00, 4.00, 2.00};
//const double  dMeterDegPerSec[] = {360.00, 180.00, 90.00, 45.00};
const double  dMeterDegPerSec[] = {400.00, 200.00, 100.00, 50.00};
const double  dSwingMinDegF     = 100.00;
const double  dSwingMaxDegF     = 450.00;

//Message Structure that is used to pass data back an forth
typedef struct stMessageStructure {
  long        lSampleTimeMsec;
  double      adTCoupleDegF[wNumTCouples];
} stMessageStruct;

extern stMessageStruct          astReadings[];

//extern lv_obj_t                 *pMeter;
//extern lv_obj_t                 *pMeterArray[wNumTCouples];
extern lv_obj_t                 *apMeterArray[];
extern Arduino_RGB_Display      *pDisplay;
//extern lv_meter_indicator_t     *pNeedleIndicator[wNumTCouples];
//extern lv_meter_indicator_t     *pNeedleIndicator[wNumTCouples];
extern lv_meter_indicator_t     *apNeedleArray[wNumTCouples];

extern int32_t                  wCurrentReadingNum;

//Functions protos
void          SaveReading           (void);
void          SetupDisplay          (void);
void          SetupLVGL             (void);
void          DisplayLabel          (const char* szText);
void          DisplayMeterArray     (uint8_t ucNumColumns, uint8_t ucNumRows, uint16_t usPercentScale);
void          DisplayMeter          (int wMeterNumber, lv_coord_t sSize   , lv_align_t ucAlignment,
                                     lv_coord_t sOffsetX, lv_coord_t sOffsetY);
void          SetNeedleValue        (lv_obj_t *pMeter, lv_meter_indicator_t *pNeedleIndicator, double dValue);
char*         szGetMyMAC            (char* szBuffer);
void          set_value             (void *pIndicator, int wValue);
lv_coord_t    sPercent              (lv_coord_t sNumber, int16_t sPercent);
//Last line
