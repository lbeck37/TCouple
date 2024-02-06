//B32_LVGL_Lib.cpp, 2/6/24b
#include <B32_LVGL_Lib.h>
#include <WiFi.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

stMessageStruct         astReadings[wMaxReadings];

Arduino_ESP32RGBPanel   *pRGBPanel;
Arduino_RGB_Display     *pDisplay;
lv_obj_t                *pMeter;

int32_t         wCurrentReading   =  0;

const int8_t    cDE_Pin           =  5;
const int8_t    cVsyncPin         =  3;
const int8_t    cHsyncPin         = 46;
const int8_t    cPclkPin          =  7;

const int8_t    acRedPin[5]       = { 1,  2, 42, 41, 40};
const int8_t    acBluePin[5]      = {14, 38, 18, 17, 10};
const int8_t    acGreenPin[6]     = {39,  0, 45, 48, 47, 21};

//Following from https://github.com/dronecz/ESP32_S3_HMI
const uint16_t  usHsyncPolarity   = 0;
const uint16_t  usVsyncPolarity   = 0;

const uint16_t  usHsyncFrontPorch = 210;
const uint16_t  usVsyncFrontPorch =  22;

const uint16_t  usHsyncPulseWidth =  30;
const uint16_t  usVsyncPulseWidth =  13;

const uint16_t  usHsyncBackPorch  =  16;
const uint16_t  usVsyncBackPorch  =  10;

const uint16_t  usPclkActiveNeg   =   1;

const uint32_t  uw16MHz           = 16000000;
const uint32_t  uwPreferSpeed     = uw16MHz;

const bool      bUseBigEndian     = false;

const uint16_t  usDEIdleHigh      = 0;
const uint16_t  usPclkIdleHigh    = 0;

lv_coord_t      sScreenWidth;
lv_coord_t      sScreenHeight;

lv_color_t      *pDisplayBuffer;
lv_disp_drv_t   stDisplayDriver;

lv_disp_draw_buf_t    stDrawBuffer;

//Protos for functions only used in this file
void    FlushDataToDisplay  (lv_disp_drv_t *pDisplayDriver, const lv_area_t *pArea, lv_color_t *color_p);
void    SetIndicatorValue   (void *pIndicator, int wValue);
double  dGetDegF            (int wTCouple);


void SaveReading(void){
  int   wDegreePeriodMsec   = 1000;
  long  lCurrentMsec        = millis();

  astReadings[wCurrentReading++].lSampleTimeMsec= lCurrentMsec;

  for(int wTCoupleNum= 0; wTCoupleNum < wNumTCouples; wTCoupleNum++){
    astReadings[wCurrentReading++].adTCoupleDegF[wTCoupleNum]= dGetDegF(wTCoupleNum);
  }
  return;
} //SaveReading


double dGetDegF(int wTCouple){
  //Needles swing back and forth from fMinSwingDegF to fMaxSwingDegF.
  //Gauge #3 goes at half cycle every sec, #2 every 2 sec, #1 every 5 sec and #0 every 10 sec
  double  dDegF;
  double  dMeterPeriodSec[]= {20.00, 10.00, 4.00, 2.00};
  double  dSwingMinDegF     = 100.00;
  double  dSwingMaxDegF     = 450.00;
  double  dRangeDegF        = (dSwingMaxDegF - dSwingMinDegF);

  double  dDegrees            = ((float)((millis() / 1000.00)) / dMeterPeriodSec[wTCouple]);
  double  dRadians            = (dDegrees / 180) * PI;
  double  dPercentRange       = ((sin(dRadians) + 1.00) * 100.00);

  dDegF= (dSwingMinDegF + (dRangeDegF *(dPercentRange / 100.00)));
  return dDegF;
} //dGetDegF


void SetupDisplay(void){
  uint16_t    usScreenWidth      = 800;
  uint16_t    usScreenHeight     = 480;

  Serial << BLOG << " SetupDisplay(): Doing new for Arduino_ESP32RGBPanel & Arduino_RGB_Display" << endl;
  pRGBPanel = new Arduino_ESP32RGBPanel(
          cDE_Pin      , cVsyncPin    , cHsyncPin    , cPclkPin,
          acRedPin  [0], acRedPin  [1], acRedPin  [2], acRedPin  [3], acRedPin  [4],
          acGreenPin[0], acGreenPin[1], acGreenPin[2], acGreenPin[3], acGreenPin[4], acGreenPin[5],
          acBluePin [0], acBluePin [1], acBluePin [2], acBluePin [3], acBluePin [4],
          usHsyncPolarity, usHsyncFrontPorch, usHsyncPulseWidth, usHsyncBackPorch,
          usVsyncPolarity, usVsyncFrontPorch, usVsyncPulseWidth, usVsyncBackPorch,
          usPclkActiveNeg, uwPreferSpeed    , bUseBigEndian    , usDEIdleHigh    , usPclkIdleHigh);

  pDisplay = new Arduino_RGB_Display(usScreenWidth, usScreenHeight, pRGBPanel);

  Serial << BLOG << " SetupDisplay(): Call pDisplay->begin()" << endl;
  if (!pDisplay->begin()){
    Serial << BLOG << " SetupDisplay(): pDisplay->begin() failed" << endl;
  }
  delay(500);
  Serial << BLOG << " SetupDisplay(): Done." << endl;
  return;
} //SetupDisplay


void SetupLVGL(void){
  sScreenWidth    = pDisplay->width();
  sScreenHeight   = pDisplay->height();

  uint32_t    uwMallocNumBytes  = (sizeof(lv_color_t) * sScreenWidth * 40);
  uint32_t    uwMemoryType      = (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

  void        *pUnusedBuffer    = NULL;

  lv_init();

  pDisplayBuffer  = (lv_color_t *)heap_caps_malloc(uwMallocNumBytes, uwMemoryType);
  if (!pDisplayBuffer){
    Serial << BLOG << " SetupLVGL(): LVGL pDisplayBuffer allocate failed!" << endl;
  } //if(!pDisplayBuffer)
  else{
    uint32_t    uwSizeInPixelCount= (sScreenWidth * 40);
    lv_disp_draw_buf_init(&stDrawBuffer, pDisplayBuffer, pUnusedBuffer, uwSizeInPixelCount);

    //Initialize the display
    lv_disp_drv_init      (&stDisplayDriver);

    stDisplayDriver.hor_res      = sScreenWidth;
    stDisplayDriver.ver_res      = sScreenHeight;
    stDisplayDriver.flush_cb     = FlushDataToDisplay;
    stDisplayDriver.draw_buf     = &stDrawBuffer;

    lv_disp_drv_register    (&stDisplayDriver);

    //Initialize the (dummy) input device driver
    static lv_indev_drv_t   stInputDeviceDriver;

    lv_indev_drv_init       (&stInputDeviceDriver);
    stInputDeviceDriver.type= LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register   (&stInputDeviceDriver);

    Serial << BLOG << " SetupLVGL(): Done." << endl;
  } //if(!pDisplayBuffer)else
  return;
} //SetupLVGL


void DisplayLabel(const char* szText){
  lv_obj_t    *pParent= lv_scr_act      ();
  lv_obj_t    *pLabel = lv_label_create (pParent);

  lv_label_set_text (pLabel, szText);
  lv_obj_align      (pLabel, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  return;
} //DisplayLabel


void SetIndicatorValue(void *pIndicator, int wValue){
  lv_meter_set_indicator_value(pMeter, (lv_meter_indicator_t*)pIndicator, wValue);
  return;
} //SetIndicatorValue


void DisplayMeterArray(uint8_t ucNumColumns, uint8_t ucNumRows, uint16_t usPercentScale){
  lv_align_t    ucAlignment         = LV_ALIGN_TOP_LEFT;
  lv_coord_t    sSpacingX           = (pDisplay->width() / 4);
  lv_coord_t    sMeterSize          = sSpacingX;
  lv_coord_t    sSpacingY           = sPercent(sMeterSize, 120);
  lv_coord_t    sFirstRowOffsetY    = sPercent(sMeterSize,  10);
  lv_coord_t    sOffsetX            =  0;
  lv_coord_t    sOffsetY            =  0;
  int           wNumMeters          = (ucNumColumns * ucNumRows);

  sMeterSize        = sPercent(sMeterSize       , usPercentScale);
  sSpacingY         = sPercent(sSpacingY        , usPercentScale);
  sFirstRowOffsetY  = sPercent(sFirstRowOffsetY , usPercentScale);

  Serial << BLOG << " DisplayMeterArray(): Call DisplayMeter " << wNumMeters << " times" << endl;
  for (int wRowNum= 0; wRowNum < ucNumRows; wRowNum++){
    sOffsetY= (sFirstRowOffsetY + (wRowNum * sSpacingY));   //Work in percentage of meter size
    for (int wMeterCount= 0; wMeterCount < ucNumColumns; wMeterCount++){
      sOffsetX= (wMeterCount * sSpacingX);
      DisplayMeter(sMeterSize, ucAlignment, sOffsetX, sOffsetY);
    } //for(int wMeterCount=0;...
  } //for(int wRowNum=0;...

  return;
} //DisplayMeterArray


void DisplayMeter(lv_coord_t sSize, lv_align_t ucAlignment, lv_coord_t sOffsetX, lv_coord_t sOffsetY){
  lv_color_t              stBlueColor         = lv_palette_main(LV_PALETTE_BLUE);
  lv_color_t              stRedColor          = lv_palette_main(LV_PALETTE_RED);
  lv_color_t              stGreyColor         = lv_palette_main(LV_PALETTE_GREY);
  lv_color_t              stBlackColor        = lv_color_black();

  bool                    bMapColorMode       = false;
  int16_t                 sTickWidthMod       =   0;
  uint16_t                usNeedleWidth       =   4;
  int16_t                 sNeedleRadiusMod    = -10;

  uint16_t                usTickWidth         =   2;
  uint16_t                usTickLength        =  10;

  uint16_t                usMajorTickWidth    =   4;
  uint16_t                usMajorTickLength   =  15;
  int16_t                 sMajorTickLabelGap  =  10;

  uint16_t                usArcWidth          =   3;
  int16_t                 sArcRadiusMod       =   0;

  uint16_t                usNumTicks          =  51;
  uint16_t                usMajorTickEveryNth =  10;

  int32_t                 wMeterMin           =   0;
  int32_t                 wMeterMax           = 500;

  uint32_t                uwAngleRange        = 315;
  uint32_t                uwRotation          =  90;

  int32_t                 wBlueArcStartValue  =   0;
  int32_t                 wBlueArcEndValue    = 150;

  int32_t                 wRedArcStartValue   = 350;
  int32_t                 wRedArcEndValue     = 500;

  lv_meter_indicator_t    *pIndicator;

  pMeter= lv_meter_create(lv_scr_act());

  lv_obj_align                        (pMeter, ucAlignment, sOffsetX, sOffsetY);
  lv_obj_set_size                     (pMeter, sSize, sSize);

  //Create the scale
  lv_meter_scale_t  *pScale= lv_meter_add_scale(pMeter);

  lv_meter_set_scale_range(pMeter, pScale, wMeterMin, wMeterMax, uwAngleRange, uwRotation);

  lv_meter_set_scale_ticks            (pMeter, pScale, usNumTicks,
                                       usTickWidth, usTickLength, stGreyColor);

  lv_meter_set_scale_major_ticks      (pMeter, pScale, usMajorTickEveryNth, usMajorTickWidth,
                                       usMajorTickLength, stBlackColor, sMajorTickLabelGap);

  //Add a blue arc to the start of the meter
  pIndicator = lv_meter_add_arc       (pMeter, pScale, usArcWidth, stBlueColor, sArcRadiusMod);

  lv_meter_set_indicator_start_value  (pMeter, pIndicator, wBlueArcStartValue);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, wBlueArcEndValue);

  /*Make the tick lines blue at the start of the scale*/
  pIndicator= lv_meter_add_scale_lines(pMeter, pScale, stBlueColor, stBlueColor,
                                       bMapColorMode, sTickWidthMod);

  lv_meter_set_indicator_start_value  (pMeter, pIndicator, wBlueArcStartValue);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, wBlueArcEndValue);

  //Add a red arc to the end of the meter
  pIndicator= lv_meter_add_arc        (pMeter, pScale, usArcWidth, stRedColor, sArcRadiusMod);
  lv_meter_set_indicator_start_value  (pMeter, pIndicator, wRedArcStartValue);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, wRedArcEndValue);

  //Make the tick lines red at the end of the pScale
  pIndicator= lv_meter_add_scale_lines(pMeter, pScale, stRedColor, stRedColor,
                                       bMapColorMode, sTickWidthMod);

  lv_meter_set_indicator_start_value  (pMeter, pIndicator, wRedArcStartValue);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, wRedArcEndValue);

  //Add a needle line indicator
  pIndicator= lv_meter_add_needle_line(pMeter, pScale, usNeedleWidth, stGreyColor, sNeedleRadiusMod);

  /*Create an animation to set the value*/
  lv_anim_t   stAnimation;
  lv_anim_init                  (&stAnimation);
  lv_anim_set_exec_cb           (&stAnimation, SetIndicatorValue);
  lv_anim_set_var               (&stAnimation, pIndicator);
  lv_anim_set_values            (&stAnimation, 0, 100);
  lv_anim_set_time              (&stAnimation, 2000);
  lv_anim_set_repeat_delay      (&stAnimation, 100);
  lv_anim_set_playback_time     (&stAnimation, 500);
  lv_anim_set_playback_delay    (&stAnimation, 100);
  lv_anim_set_repeat_count      (&stAnimation, LV_ANIM_REPEAT_INFINITE);
  lv_anim_start                 (&stAnimation);

  return;
} //DisplayMeter


void FlushDataToDisplay(lv_disp_drv_t *pDisplayDriver, const lv_area_t *pArea, lv_color_t *color_p)
{
  uint32_t uwWidth  = (pArea->x2 - pArea->x1 + 1);
  uint32_t uwHeight = (pArea->y2 - pArea->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  pDisplay->draw16bitBeRGBBitmap(pArea->x1, pArea->y1, (uint16_t *)&color_p->full, uwWidth, uwHeight);
#else
  pDisplay->draw16bitRGBBitmap(pArea->x1, pArea->y1, (uint16_t *)&color_p->full, uwWidth, uwHeight);
#endif

  lv_disp_flush_ready(pDisplayDriver);
  return;
} //FlushDataToDisplay


char* szGetMyMAC(char* szBuffer){
  uint8_t     aucMyMACAddress[6];

  WiFi.macAddress(aucMyMACAddress);
  sprintf(szBuffer, "%x %x %x %x %x %x", aucMyMACAddress[0], aucMyMACAddress[1], aucMyMACAddress[2],
                                         aucMyMACAddress[3], aucMyMACAddress[4], aucMyMACAddress[5]);
  return szBuffer;
} //szGetMyMAC


lv_coord_t sPercent(lv_coord_t sNumber, int16_t sPercent){
  lv_coord_t sReturn;
  return((sNumber * sPercent) / 100);
  } //sPercent
//Last line
