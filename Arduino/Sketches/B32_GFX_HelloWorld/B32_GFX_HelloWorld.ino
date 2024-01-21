const char szSketchName[]  = "B32_GFX_HelloWorld.ino";
const char szFileDate[]    = "1/20/24d";

#include <Arduino_GFX_Library.h>

#if false
  #define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

  /* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
  #if defined(DISPLAY_DEV_KIT)
    Arduino_GFX *gfx = create_default_Arduino_GFX();
  #else /* !defined(DISPLAY_DEV_KIT) */
    /* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
    Arduino_DataBus *bus = create_default_Arduino_DataBus();

    /* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
    Arduino_GFX *gfx = new Arduino_ILI9341(bus, DF_GFX_RST, 0 /* rotation */, false /* IPS */);
  #endif /* !defined(DISPLAY_DEV_KIT) */
  /*******************************************************************************
   * End of Arduino_GFX setting
   ******************************************************************************/
#else
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

/*
const int8_t    cRotation         = 0;
const bool      bAutoFlush        = true;
Arduino_DataBus *pArduino_DataBus = NULL;
*/

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_ESP32RGBPanel *pRGBPanel = new Arduino_ESP32RGBPanel(
        cDE_Pin      , cVsyncPin    , cHsyncPin    , cPclkPin,
        acRedPin  [0], acRedPin  [1], acRedPin  [2], acRedPin  [3], acRedPin  [4],
        acGreenPin[0], acGreenPin[1], acGreenPin[2], acGreenPin[3], acGreenPin[4], acGreenPin[5],
        acBluePin [0], acBluePin [1], acBluePin [2], acBluePin [3], acBluePin [4],
        usHsyncPolarity, usHsyncFrontPorch, usHsyncPulseWidth, usHsyncBackPorch,
        usVsyncPolarity, usVsyncFrontPorch, usVsyncPulseWidth, usVsyncBackPorch,
        usPclkActiveNeg, uwPreferSpeed    , bUseBigEndian    , usDEIdleHigh    , usPclkIdleHigh);


Arduino_RGB_Display *pRGBDisplay = new Arduino_RGB_Display(usPanelWidth, usPanelHeight, pRGBPanel);
#endif


void setup(void){
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX Hello World example");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!pRGBDisplay->begin()){
    Serial.println("pRGBDisplay->begin() failed!");
  }
  pRGBDisplay->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  pRGBDisplay->setCursor(10, 10);
  pRGBDisplay->setTextColor(RED);
  pRGBDisplay->println("Hello World!");

  delay(5000); // 5 seconds
} //setup


void loop(){
  pRGBDisplay->setCursor(random(pRGBDisplay->width()), random(pRGBDisplay->height()));
  pRGBDisplay->setTextColor(random(0xffff), random(0xffff));
  pRGBDisplay->setTextSize(random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);
  pRGBDisplay->println("Hello World!");

  delay(1000); // 1 second
} //loop
//Last line.
