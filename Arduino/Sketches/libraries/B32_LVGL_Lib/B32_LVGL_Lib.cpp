//B32_LVGL_Lib.cpp, 2/2/24c
#include <B32_LVGL_Lib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

Arduino_ESP32RGBPanel   *pRGBPanel;
Arduino_RGB_Display     *pDisplay;
lv_obj_t                *pMeter;

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

uint32_t             screenWidth;
uint32_t             screenHeight;
lv_disp_draw_buf_t   draw_buf;
lv_color_t           *disp_draw_buf;
lv_disp_drv_t        disp_drv;

//Protos for functions only used in this file
void  my_disp_flush   (lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);


void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  pDisplay->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  pDisplay->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
  return;
} //my_disp_flush


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
/*
  else{
    Serial << BLOG << " SetupDisplay(): Clear screen with call to pDisplay->fillScreen(BLACK)" << endl;
    pDisplay->fillScreen(BLACK);
  }
*/
  delay(500);
  Serial << BLOG << " SetupDisplay(): Done." << endl;
  return;
} //SetupDisplay


void SetupLVGL(void){
  lv_init();

  screenWidth = pDisplay->width();
  screenHeight = pDisplay->height();
#ifdef ESP32
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 40, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else
  disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * 40);
#endif
  if (!disp_draw_buf)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  } //if(!disp_draw_buf)
  else
  {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 40);

    /* Initialize the display */
    lv_disp_drv_init      (&disp_drv);

    disp_drv.hor_res      = screenWidth;
    disp_drv.ver_res      = screenHeight;
    disp_drv.flush_cb     = my_disp_flush;
    disp_drv.draw_buf     = &draw_buf;
    lv_disp_drv_register  (&disp_drv);

    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t   indev_drv;

    lv_indev_drv_init       (&indev_drv);
    indev_drv.type          = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register   (&indev_drv);

    Serial << BLOG << " SetupLVGL(): Done." << endl;
  } //if(!disp_draw_buf)else
  return;
} //SetupLVGL


void set_value(void *pIndicator, int wValue){
  lv_meter_set_indicator_value(pMeter, (lv_meter_indicator_t *)pIndicator, wValue);
  return;
} //set_value


void Display8Meters(void){
  //Arranged in 2 rows of 4
  lv_align_t    ucAlignment     = LV_ALIGN_TOP_LEFT;
  lv_coord_t    sScreenWidth    = pDisplay->width();
  lv_coord_t    sSpacingX       = (sScreenWidth / 4);
  lv_coord_t    sMeterSize      = sSpacingX;
  lv_coord_t    sSpacingY       = sPercent(sMeterSize, 120);
  lv_coord_t    sOffsetX        =  0;
  lv_coord_t    sOffsetY        =  0;

  Serial << BLOG << " Display8Meters(): Call DisplayMeter 8 times" << endl;
  for (int wRowNum= 0; wRowNum < 2; wRowNum++){
    sOffsetY= (wRowNum * sSpacingY);   //Work in percentage of meter size
    for (int wMeterCount= 0; wMeterCount < 4; wMeterCount++){
      sOffsetX= (wMeterCount * sSpacingX);
      DisplayMeter(sMeterSize, ucAlignment, sOffsetX, sOffsetY);
    } //for(int wMeterCount=0;...
  } //for(int wRowNum=0;...

  return;
} //Display8Meters


lv_coord_t sPercent(lv_coord_t sNumber, int16_t sPercent){
  lv_coord_t sReturn;

  return((sNumber * sPercent) / 100);
  } //sPercent


void DisplayMeter(lv_coord_t sSize, lv_align_t ucAlignment, lv_coord_t sOffsetX, lv_coord_t sOffsetY){
  pMeter= lv_meter_create(lv_scr_act());

  //lv_obj_center                       (pMeter);
  lv_obj_align                        (pMeter, ucAlignment, sOffsetX, sOffsetY);
  lv_obj_set_size                     (pMeter, sSize, sSize);

  /*Add a scale first*/
  lv_meter_scale_t    *pScale=        lv_meter_add_scale(pMeter);

  lv_meter_set_scale_ticks            (pMeter, pScale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks      (pMeter, pScale, 8, 4, 15, lv_color_black(), 10);

  lv_meter_indicator_t *pIndicator;

  /*Add a blue arc to the start*/
  pIndicator = lv_meter_add_arc(pMeter, pScale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
  lv_meter_set_indicator_start_value  (pMeter, pIndicator, 0);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, 20);

  /*Make the tick lines blue at the start of the scale*/
  pIndicator= lv_meter_add_scale_lines(pMeter, pScale, lv_palette_main(LV_PALETTE_BLUE),
                                       lv_palette_main(LV_PALETTE_BLUE), false, 0);
  lv_meter_set_indicator_start_value  (pMeter, pIndicator, 0);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, 20);

  /*Add a red arc to the end*/
  pIndicator= lv_meter_add_arc        (pMeter, pScale, 3, lv_palette_main(LV_PALETTE_RED), 0);
  lv_meter_set_indicator_start_value  (pMeter, pIndicator, 80);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, 100);

  /*Make the tick lines red at the end of the pScale*/
  pIndicator= lv_meter_add_scale_lines(pMeter, pScale, lv_palette_main(LV_PALETTE_RED),
                                       lv_palette_main(LV_PALETTE_RED), false, 0);
  lv_meter_set_indicator_start_value  (pMeter, pIndicator, 80);
  lv_meter_set_indicator_end_value    (pMeter, pIndicator, 100);

  /*Add a needle line indicator*/
  pIndicator= lv_meter_add_needle_line(pMeter, pScale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

  /*Create an animation to set the value*/
  lv_anim_t   stAnimation;
  lv_anim_init                  (&stAnimation);
  lv_anim_set_exec_cb           (&stAnimation, set_value);
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
//Last line
