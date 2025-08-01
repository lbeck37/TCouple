//B32_RGBDisplay.cpp, 1/20/24b
#include <B32_RGBDisplayLib.h>

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

const uint16_t  usPanelWidth      = 800;
const uint16_t  usPanelHeight     = 480;

const int8_t    cDE_Pin           =  5;
const int8_t    cVsyncPin         =  3;
const int8_t    cHsyncPin         = 46;
const int8_t    cPclkPin          =  7;

const int8_t    acRedPin[]        = { 1,  2, 42, 41, 40};
const int8_t    acBluePin[]       = {14, 38, 18, 17, 10};
const int8_t    acGreenPin[]      = {39,  0, 45, 48, 47, 21};

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


Arduino_ESP32RGBPanel *pRGBPanel = new Arduino_ESP32RGBPanel(
        cDE_Pin      , cVsyncPin    , cHsyncPin    , cPclkPin,
        acRedPin  [0], acRedPin  [1], acRedPin  [2], acRedPin  [3], acRedPin  [4],
        acGreenPin[0], acGreenPin[1], acGreenPin[2], acGreenPin[3], acGreenPin[4], acGreenPin[5],
        acBluePin [0], acBluePin [1], acBluePin [2], acBluePin [3], acBluePin [4],
        usHsyncPolarity, usHsyncFrontPorch, usHsyncPulseWidth, usHsyncBackPorch,
        usVsyncPolarity, usVsyncFrontPorch, usVsyncPulseWidth, usVsyncBackPorch,
        usPclkActiveNeg, uwPreferSpeed    , bUseBigEndian    , usDEIdleHigh    , usPclkIdleHigh);


Arduino_RGB_Display *pRGBDisplay = new Arduino_RGB_Display(usPanelWidth, usPanelHeight, pRGBPanel);
//Last line
