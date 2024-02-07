const char szSketchName[]  = "B32_LvglHelloWorld.ino";
const char szFileDate[]    = "2/7/24J";
//Use Arduino_GFX as driver for LVGL calls to write to Waveshare 800x480, 4.3" 16-bit 5-6-5 RGB

#include <B32_LVGL_Lib.h>
#include <ESP32TimerInterrupt.h>
//#include <ESP32TimerInterrupt.hpp>
//#include "ESP32TimerInterrupt.h"
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

uint16_t  usMeterPercentScale = 100;
uint8_t   ucNumColumns        = 2;
uint8_t   ucNumRows           = 2;

//Function protos
void  setup         (void);
void  loop          (void);


void setup(){
  char  szLabelText[90];
  char  szMACText  [30];

  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << endl << BLOG  << " setup(): DEBUGGING: Print pointer addresses:" << endl;
  for (int wMeterNum= 0; wMeterNum < wNumTCouples; wMeterNum++) {
      Serial << BLOG << " setup():pMeterArray[" << wMeterNum << "]= " << (long)pMeterArray[wMeterNum] <<
                        ", pNeedleIndicator[" << wMeterNum << "]= " << (long)pNeedleIndicator[wMeterNum] << endl;
  }

  //Initialize values for 0 element
  astReadings[0].lSampleTimeMsec= 0;
  for (int wMeter= 0; wMeter < wNumTCouples; wMeter++){
    astReadings[0].adTCoupleDegF[wMeter]= dSwingMinDegF;
  }

  Serial << BLOG << " setup(): Call SetupDisplay" << endl;
  SetupDisplay();

  Serial << BLOG << " setup(): Call SetupLVGL" << endl;
  SetupLVGL();

  Serial << BLOG << " setup(): Call DisplayLabel" << endl;
  sprintf(szLabelText, "%s, %s, MAC=%s", szSketchName, szFileDate, szGetMyMAC(szMACText));
  DisplayLabel(szLabelText);

  Serial << BLOG << " setup(): Call DisplayMeterArray" << endl;
  DisplayMeterArray(ucNumColumns, ucNumRows, usMeterPercentScale);

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void loop(){
//  lv_timer_handler(); /* let the GUI do its work */
/*
#ifdef CANVAS
  pDisplay->flush();
#endif
*/
  Serial << endl;
  SaveReading();

  for (int wMeterNum= 0; wMeterNum < wNumTCouples; wMeterNum++){
    double  dNeedleValue= astReadings[wCurrentReadingNum].adTCoupleDegF[wMeterNum];
    if (pMeterArray[wMeterNum] && pNeedleIndicator[wMeterNum]){
      Serial << BLOG << " loop():Call SetNeedleValue, wMeterNum= " << wMeterNum << ", dNeedleValue= " << dNeedleValue << endl;

      Serial << BLOG << " loop():pMeterArray[" << wMeterNum << "]= " << (long)pMeterArray[wMeterNum] <<
                        ", pNeedleIndicator[" << wMeterNum << "]= " << (long)pNeedleIndicator[wMeterNum] << endl;

      SetNeedleValue(pMeterArray[wMeterNum], pNeedleIndicator[wMeterNum], dNeedleValue);
    } //if(pMeter&&pNeedleIndicator[wMeterNum])
    else{
      if (!pMeterArray[wMeterNum]){
        Serial << BLOG << " loop(): ERROR: pMeterArray[" << wMeterNum << "] is NULL" << endl;
      }
      if (!pNeedleIndicator[wMeterNum]){
        Serial << BLOG << " loop(): ERROR: pNeedleIndicator[" << wMeterNum << "] is NULL" << endl;
      }
    } //if(pMeter&&pNeedleIndicator[wMeterNum])else
  } //for

  lv_timer_handler(); /* let the GUI do its work */

  delay(2000);
  return;
} //loop
//Last line.
