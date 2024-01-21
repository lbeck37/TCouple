const char szSketchName[]  = "B32_GFX_HelloWorld.ino";
const char szFileDate[]    = "1/20/24b";

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6, SCK: 13, MOSI: 11, MISO: 12
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-S2 various dev board  : CS: 34, DC: 38, RST: 33, BL: 21, SCK: 36, MOSI: 35, MISO: nil
 * ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil
 * ESP8266 various dev board   : CS: 15, DC:  4, RST:  2, BL:  5, SCK: 14, MOSI: 13, MISO: 12
 * Raspberry Pi Pico dev board : CS: 17, DC: 27, RST: 26, BL: 28, SCK: 18, MOSI: 19, MISO: 16
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23, SCK: 19, MOSI: 21, MISO: 20
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3, SCK: 10, MOSI: 12, MISO: 11
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23, SCK: 13, MOSI: 11, MISO: 12
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13, SCK: 11, MOSI:  9, MISO: 10
 * Seeeduino XIAO dev board    : CS:  3, DC:  2, RST:  1, BL:  0, SCK:  8, MOSI: 10, MISO:  9
 * Teensy 4.1 dev board        : CS: 39, DC: 41, RST: 40, BL: 22, SCK: 13, MOSI: 11, MISO: 12
 ******************************************************************************/
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

const uint16_t  usHsyncPolarity   = 0;
const uint16_t  usVsyncPolarity   = 0;

const uint16_t  usHsyncFrontPorch = 0;
const uint16_t  usVsyncFrontPorch = 0;

const uint16_t  usHsyncPulseWidth = 0;
const uint16_t  usVsyncPulseWidth = 0;

const uint16_t  usHsyncBackPorch  = 0;
const uint16_t  usVsyncBackPorch  = 0;

const uint16_t  usPclkActiveNeg   = 0;

const uint32_t  uwPreferSpeed     = 0;

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
        acRedPin[0]  , acRedPin[1]  , acRedPin[2]  , acRedPin[3]  , acRedPin[4],
        acGreenPin[0], acGreenPin[1], acGreenPin[2], acGreenPin[3], acGreenPin[4], acGreenPin[5],
        acBluePin[0] , acBluePin[1] , acBluePin[2] , acBluePin[3] , acBluePin[4],
        usHsyncPolarity, usHsyncFrontPorch, usHsyncPulseWidth, usHsyncBackPorch,
        usVsyncPolarity, usVsyncFrontPorch, usVsyncPulseWidth, usVsyncBackPorch,
        usPclkActiveNeg, uwPreferSpeed    , bUseBigEndian    , usDEIdleHigh    , usPclkIdleHigh);


Arduino_RGB_Display *pRGBDisplay = new Arduino_RGB_Display(usPanelWidth, usPanelHeight, pRGBPanel);
#endif


void setup(void)
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX Hello World example");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!pRGBDisplay->begin())
  {
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


void loop()
{
  pRGBDisplay->setCursor(random(pRGBDisplay->width()), random(pRGBDisplay->height()));
  pRGBDisplay->setTextColor(random(0xffff), random(0xffff));
  pRGBDisplay->setTextSize(random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);
  pRGBDisplay->println("Hello World!");

  delay(1000); // 1 second
} //loop
//Last line.
