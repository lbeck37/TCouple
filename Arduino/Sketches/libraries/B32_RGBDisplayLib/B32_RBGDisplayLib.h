//B32_RGBDisplayLib.h, 1/24/24c
#pragma once
#include <Arduino_GFX_Library.h>

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

const int                 wNumTCouples=  8;
//Message Structure that is used to pass data back an forth
typedef struct stMessageStruct {
  double      adTCoupleDegF[wNumTCouples];
} stMessageStruct;

extern const char           szSketchName[];
extern const char           szFileDate[];

class RGBScreen{
  public:
    RGBScreen             (uint16_t usWidthPixels= 800, uint16_t usHeightPixels= 480);
    ~RGBScreen            (void);   //destructor
    void  ShowMyMAC       (void);
    void  CreateData      (void);
    void  SetupDisplay    (void);
    void  DisplayLabels   (void);
    void  DisplayData     (void);
    void  RandomDisplay   (void);
  private:
    stMessageStruct       stReadings;
    stMessageStruct       stLastReadings;

    uint8_t         aucMyMACAddress[6];
    uint16_t        usScreenWidth;
    uint16_t        usScreenHeight;

    uint16_t        uwBGColor             = BLACK;
    uint16_t        uwMACColor            = CYAN;
    uint16_t        uwTitleColor          = CYAN;
    uint16_t        uwLabelColor          = YELLOW;
    uint16_t        uwDataColor           = GREEN;

    const int8_t    cDE_Pin               =  5;
    const int8_t    cVsyncPin             =  3;
    const int8_t    cHsyncPin             = 46;
    const int8_t    cPclkPin              =  7;

    const int8_t    acRedPin[5]           = { 1,  2, 42, 41, 40};
    const int8_t    acBluePin[5]          = {14, 38, 18, 17, 10};
    const int8_t    acGreenPin[6]         = {39,  0, 45, 48, 47, 21};

    static const int  wMaxLabelChars            = 20;
    char     aucLeftLabel [4][wMaxLabelChars]   = {"Cyl 1"  , "Cyl 2"   , "Cyl 3"  , "Cyl 4"};
    char     aucRightLabel[4][wMaxLabelChars]   = {"Oil In" , "Oil Out" , "Heater" , "Air"  };

    const int16_t   sLineSpacing          = 70;

    const int16_t   sLeftLabelX           =  0;
    const int16_t   sLeftLabelFirstY      = 100;

    //const int16_t   sLeftDataX            = 120;
    //const int16_t   sLeftDataX            = 180;
    const int16_t   sLeftDataX            = 200;
    const int16_t   sLeftDataFirstY       = sLeftLabelFirstY;

    //const int16_t   sRightLabelX          = 300;
    const int16_t   sRightLabelX          = 400;
    const int16_t   sRightLabelFirstY     = sLeftLabelFirstY;


    //const int16_t   sRightDataX           = 440;
    const int16_t   sRightDataX           = 600;
    //const int16_t   sRightDataX           = 590;
    const int16_t   sRightDataFirstY      = sLeftDataFirstY;

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

    Arduino_ESP32RGBPanel             *pRGBPanel;
    Arduino_RGB_Display               *pRGBDisplay;
};  //RGBScreen
//Last line
