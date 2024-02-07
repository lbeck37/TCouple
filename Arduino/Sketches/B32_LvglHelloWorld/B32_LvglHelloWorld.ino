const char szSketchName[]  = "B32_LvglHelloWorld.ino";
const char szFileDate[]    = "2/7/24S";
//Use Arduino_GFX as driver for LVGL calls to write to Waveshare 800x480, 4.3" 16-bit 5-6-5 RGB

#include <B32_LVGL_Lib.h>
#include <ESP32TimerInterrupt.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

uint16_t          usMeterPercentScale   = 100;    //Size of meters?
uint8_t           ucNumColumns          =   2;
uint8_t           ucNumRows             =   2;

const uint8_t     ucLvgl_TimerNumber    =   0;

//Create the interrupt timer to handle LVGL work.
ESP32Timer        Lvgl_InterruptTimer   (ucLvgl_TimerNumber);

//Function protos
bool IRAM_ATTR  Lvgl_TimerHandler   (void *pucTimerNumber);
void            setup               (void);
void            loop                (void);


//bool IRAM_ATTR TimerHandler0(void * timerNo)
bool IRAM_ATTR Lvgl_TimerHandler(void *pucTimerNumber){
  Serial << BLOG << " Lvgl_TimerHandler(): Interrupt fired" << endl;

  lv_timer_handler(); /* let the GUI do its work */
  return true;
} //TimerHandler0


void setup(){
  char  szLabelText[90];
  char  szMACText  [30];

  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  // Interval in microsecs
  const unsigned long   ulOneMsec             = 1000;
  unsigned long         ulLvgl_TimerInterval  = (100 * ulOneMsec);

  Serial << BLOG << " setup(): Call Lvgl_InterruptTimer.attachInterruptInterval" << endl;
  if (!Lvgl_InterruptTimer.attachInterruptInterval(ulLvgl_TimerInterval, Lvgl_TimerHandler)){
    Serial << BLOG << " setup(): ERROR: Call to Lvgl_InterruptTimer.attachInterruptInterval FAILED" << endl;
  } //if(!Lvgl_InterruptTimer.attachInterruptInterval(...

/*
  Serial << endl << BLOG  << " setup(): DEBUGGING: Print pointer addresses:" << endl;
  for (int wMeterNum= 0; wMeterNum < wNumTCouples; wMeterNum++) {
      Serial << BLOG << " setup():apMeterArray[" << wMeterNum << "]= " << (long)apMeterArray[wMeterNum] <<
                        ", apNeedleArray[" << wMeterNum << "]= " << (long)apNeedleArray[wMeterNum] << endl;
  }
*/

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
    if (apMeterArray[wMeterNum] && apNeedleArray[wMeterNum]){
      Serial << BLOG << " loop():Call SetNeedleValue, wMeterNum= " << wMeterNum << ", dNeedleValue= " << dNeedleValue << endl;

      Serial << BLOG << " loop():apMeterArray[" << wMeterNum << "]= " << (long)apMeterArray[wMeterNum] <<
                        ", apNeedleArray[" << wMeterNum << "]= " << (long)apNeedleArray[wMeterNum] << endl;

      SetNeedleValue(apMeterArray[wMeterNum], apNeedleArray[wMeterNum], dNeedleValue);
    } //if(pMeter&&apNeedleArray[wMeterNum])
    else{
      if (!apMeterArray[wMeterNum]){
        Serial << BLOG << " loop(): ERROR: apMeterArray[" << wMeterNum << "] is NULL" << endl;
      }
      if (!apNeedleArray[wMeterNum]){
        Serial << BLOG << " loop(): ERROR: apNeedleArray[" << wMeterNum << "] is NULL" << endl;
      }
    } //if(pMeter&&apNeedleArray[wMeterNum])else
  } //for

  //lv_timer_handler(); /* let the GUI do its work */

  delay(2000);
  return;
} //loop
//Last line.
