//Beck, TFT_eWidget.h, 12/26/23b
/***************************************************************************************

  TFT display Widget library comprising following classes:
    1. ButtonWidget - button library
    2. SliderWidget - control sliders

***************************************************************************************/
#ifndef _TFT_eWidgetH_
#define _TFT_eWidgetH_

//Standard support
#include <Arduino.h>

// New ESP8266 board package uses ARDUINO_ARCH_ESP8266
// old package defined ESP8266
#if defined (ESP8266)
  #ifndef ARDUINO_ARCH_ESP8266
    #define ARDUINO_ARCH_ESP8266
  #endif
#endif

#if defined (TOUCH_CS)
  #pragma message("TFT_eWidget.h, TOUCH_CS is defined, do #include of FS.h and LittleFS.h")
  // LittleFS is required for touch calibration in TFT_eSPI sketches
  #if defined (ARDUINO_ARCH_ESP8266) || defined (ESP32)
    #include <pgmspace.h>
    #include <FS.h>
    #include <LittleFS.h>   //tried commented out 11/15/23, Beck
  #endif
#else
  #pragma message("TFT_eWidget.h, TOUCH_CS is NOT defined, skip #include of FS.h and LittleFS.h")
#endif

#include <TFT_eSPI.h>

#include "widgets/button/ButtonWidget.h"
#include "widgets/slider/SliderWidget.h"
#include "widgets/meter/Meter.h"
#include "widgets/graph/GraphWidget.h"
#include "widgets/graph/TraceWidget.h"
#endif
