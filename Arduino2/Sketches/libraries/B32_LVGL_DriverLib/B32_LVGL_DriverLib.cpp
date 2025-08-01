//B32_LVGL_DriverLib.cpp, 3/11/24b, was 2/28/24c
//#include <lvgl.h>
#include <B32_LVGL_DriverLib.h>
//#include <Arduino_GFX_Library.h>
#include <Arduino_GFX.h>
//#include <Arduino_ESP32RGBPanel.h>
#include <databus/Arduino_ESP32RGBPanel.h>
#include <display/Arduino_RGB_Display.h>
#include "touch.h"
#include <WiFi.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

#define WAVESHARE_4DOT3     true
#define ELECROW_7INCH       false

lv_coord_t              sScreenWidth;
lv_coord_t              sScreenHeight;

lv_color_t              *pDisplayBuffer;

//lv_disp_drv_t and lv_disp_draw_buf_t are defined in lv_hal_disp.h
lv_disp_drv_t           stDisplayDriver;
lv_disp_draw_buf_t      stDrawBuffer;

Arduino_ESP32RGBPanel   *pRGBPanel;
Arduino_RGB_Display     *pDisplay;

int32_t         wCurrentReadingNum=  0;

#if WAVESHARE_4DOT3
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
  const uint16_t  usPclkIdleHigh    = 0;
  const uint16_t  usDEIdleHigh      = 0;
/*
  const uint32_t  uw16MHz           = 16000000;
  const uint32_t  uwPreferSpeed     = uw16MHz;
  const bool      bUseBigEndian     = false;
*/
#endif

#if ELECROW_7INCH
  const int8_t    cDE_Pin           = 41;
  const int8_t    cVsyncPin         = 40;
  const int8_t    cHsyncPin         = 39;
  const int8_t    cPclkPin          =  0;

  const int8_t    acRedPin[5]       = {14, 21, 47, 48, 45};
  const int8_t    acBluePin[5]      = {15,  7,  6,  5,  4};
  const int8_t    acGreenPin[6]     = { 9, 46,  3,  8, 16, 1};

  //Following from https://github.com/dronecz/ESP32_S3_HMI
  const uint16_t  usHsyncPolarity   = 0;
  const uint16_t  usVsyncPolarity   = 0;

  const uint16_t  usHsyncFrontPorch =  40;
  const uint16_t  usVsyncFrontPorch =   1;

  const uint16_t  usHsyncPulseWidth =  48;
  const uint16_t  usVsyncPulseWidth =  31;

  const uint16_t  usHsyncBackPorch  =  40;
  const uint16_t  usVsyncBackPorch  =  13;

  const uint16_t  usPclkActiveNeg   = 1;
  const uint16_t  usPclkIdleHigh    = 0;
  const uint16_t  usDEIdleHigh      = 0;
/*
  const uint32_t  uw16MHz           = 16000000;
  const uint32_t  uwPreferSpeed     = uw16MHz;
  const bool      bUseBigEndian     = false;
*/
#endif

const uint32_t    uw16MHz           = 16000000;
const uint32_t    uwPreferSpeed     = uw16MHz;
const bool        bUseBigEndian     = false;

//Protos for functions only used in this file
void FlushToDisplayCallback (lv_disp_drv_t  *pDisplayDriver,const lv_area_t *pArea,lv_color_t *color_p);
void ReadTouchpadCallback   (lv_indev_drv_t *stInputDeviceDriver, lv_indev_data_t *pTouchpadData);

void FlushToDisplayCallback(lv_disp_drv_t *pDisplayDriver, const lv_area_t *pArea, lv_color_t *color_p){
  uint32_t uwWidth  = (pArea->x2 - pArea->x1 + 1);
  uint32_t uwHeight = (pArea->y2 - pArea->y1 + 1);

  Serial << "X";
#if (LV_COLOR_16_SWAP != 0)
  pDisplay->draw16bitBeRGBBitmap(pArea->x1, pArea->y1, (uint16_t *)&color_p->full, uwWidth, uwHeight);
#else
  pDisplay->draw16bitRGBBitmap(pArea->x1, pArea->y1, (uint16_t *)&color_p->full, uwWidth, uwHeight);
#endif

  lv_disp_flush_ready(pDisplayDriver);
  return;
} //FlushToDisplayCallback


void ReadTouchpadCallback(lv_indev_drv_t *stInputDeviceDriver, lv_indev_data_t *pTouchpadData){
  //Based on B32_Elecrow_Demo.ino
  if (touch_has_signal()){
    if (touch_touched()){
      pTouchpadData->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      pTouchpadData->point.x = uwLastTouchX;
      pTouchpadData->point.y = uwLastTouchY;

      //Serial << BLOG << " ReadTouchpadCallback(): Data X=" << pTouchpadData->point.x << endl;
      //Serial << BLOG << " ReadTouchpadCallback(): Data Y=" << pTouchpadData->point.y << endl;
      //Serial << "Y";
    } //if(touch_touched())
    else if (touch_released()){
      pTouchpadData->state = LV_INDEV_STATE_REL;
    } //if(touch_released()
  } //if(touch_has_signal())
  else
  {
    pTouchpadData->state = LV_INDEV_STATE_REL;
  } //if(touch_has_signal())else
  return;
} //ReadTouchpadCallback


void SetupDisplay(void){
  uint16_t    usScreenWidth      = 800;
  uint16_t    usScreenHeight     = 480;

  Serial << BLOG << " SetupDisplay(): Doing new Arduino_ESP32RGBPanel" << endl;
  pRGBPanel = new Arduino_ESP32RGBPanel(
          cDE_Pin      , cVsyncPin    , cHsyncPin    , cPclkPin,
          acRedPin  [0], acRedPin  [1], acRedPin  [2], acRedPin  [3], acRedPin  [4],
          acGreenPin[0], acGreenPin[1], acGreenPin[2], acGreenPin[3], acGreenPin[4], acGreenPin[5],
          acBluePin [0], acBluePin [1], acBluePin [2], acBluePin [3], acBluePin [4],
          usHsyncPolarity, usHsyncFrontPorch, usHsyncPulseWidth, usHsyncBackPorch,
          usVsyncPolarity, usVsyncFrontPorch, usVsyncPulseWidth, usVsyncBackPorch,
          usPclkActiveNeg, uwPreferSpeed    , bUseBigEndian    , usDEIdleHigh    , usPclkIdleHigh);

  Serial << BLOG << " SetupDisplay(): Doing new Arduino_RGB_Display" << endl;
  pDisplay = new Arduino_RGB_Display(usScreenWidth, usScreenHeight, pRGBPanel);

  Serial << BLOG << " SetupDisplay(): Call pDisplay->begin()" << endl;
  if (!pDisplay->begin()){
    Serial << BLOG << " SetupDisplay(): pDisplay->begin() failed" << endl;
  }

  Serial << BLOG << " SetupDisplay(): Call touch_init" << endl;
  touch_init();

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

  Serial << BLOG << " SetupLVGL(): Do new heap_caps_malloc() for pDisplayBuffer" << endl;
  pDisplayBuffer  = (lv_color_t *)heap_caps_malloc(uwMallocNumBytes, uwMemoryType);
  if (!pDisplayBuffer){
    Serial << BLOG << " SetupLVGL(): LVGL pDisplayBuffer allocate failed!" << endl;
  } //if(!pDisplayBuffer)
  else{
    uint32_t    uwSizeInPixelCount= (sScreenWidth * 40);
    Serial << BLOG << " SetupLVGL(): Call lv_disp_draw_buf_init(&stDrawBuffer, pDisplayBuffer,..." << endl;
    lv_disp_draw_buf_init(&stDrawBuffer, pDisplayBuffer, pUnusedBuffer, uwSizeInPixelCount);

    //Initialize the display
    Serial << BLOG << " SetupLVGL(): Call lv_disp_drv_init(&stDisplayDriver)" << endl;
    lv_disp_drv_init(&stDisplayDriver);

    stDisplayDriver.hor_res      = sScreenWidth;
    stDisplayDriver.ver_res      = sScreenHeight;
    stDisplayDriver.flush_cb     = FlushToDisplayCallback;
    stDisplayDriver.draw_buf     = &stDrawBuffer;

    Serial << BLOG << " SetupLVGL(): Call lv_disp_drv_register(&stDisplayDriver)" << endl;
    lv_disp_drv_register(&stDisplayDriver);

    //Initialize the input device driver
    static lv_indev_drv_t   stInputDeviceDriver;

    lv_indev_drv_init     (&stInputDeviceDriver);
    stInputDeviceDriver.type    = LV_INDEV_TYPE_POINTER;
    stInputDeviceDriver.read_cb = ReadTouchpadCallback;
    lv_indev_drv_register (&stInputDeviceDriver);

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


char* szGetMyMAC(char* szBuffer){
  uint8_t     aucMyMACAddress[6];

  WiFi.macAddress(aucMyMACAddress);
  sprintf(szBuffer, "%x %x %x %x %x %x", aucMyMACAddress[0], aucMyMACAddress[1], aucMyMACAddress[2],
                                         aucMyMACAddress[3], aucMyMACAddress[4], aucMyMACAddress[5]);
  return szBuffer;
} //szGetMyMAC
//Last line
