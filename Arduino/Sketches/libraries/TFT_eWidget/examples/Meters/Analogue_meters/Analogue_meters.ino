/* Beck, Analogue_meters.ino 12/26/23c
  Example animated analog meters
  Needs Font 2 (also Font 4 if using large scale label)
  Requires widget library here: https://github.com/Bodmer/TFT_eWidget*/

#include <TFT_eSPI.h>
#include <TFT_eWidget.h>

TFT_eSPI      Screen          = TFT_eSPI();

MeterWidget   AmpMeter        = MeterWidget(&Screen);
MeterWidget   VoltMeter       = MeterWidget(&Screen);
MeterWidget   OhmMeter        = MeterWidget(&Screen);

const int     wLoopPeriodMsec =  35; // Display updates every 35 ms

uint16_t    usAmpMeterXloc    =   0;
uint16_t    usAmpMeterYloc    =   0;

uint16_t    usVoltMeterXloc   =   0;
uint16_t    usVoltMeterYloc   = 128;

uint16_t    usOhmMeterXloc    =   0;
uint16_t    usOhmMeterYloc    = 256;

float       fAmpFullScale     =   2.0;
float       fVoltFullScale    =  10.0;
float       fOhmFullScale     = 100.0;

uint16_t    usAmpRedStart     =  75;
uint16_t    usAmpRedEnd       = 100;
uint16_t    usAmpOrgStart     =  50;
uint16_t    usAmpOrgEnd       =  75;
uint16_t    usAmpYelStart     =  25;
uint16_t    usAmpYelEnd       =  50;
uint16_t    usAmpGrnStart     =   0;
uint16_t    usAmpGrnEnd       =  25;

uint16_t    usVoltRedStart    =   0;
uint16_t    usVoltRedEnd      = 100;
uint16_t    usVoltOrgStart    =  25;
uint16_t    usVoltOrgEnd      =  75;
uint16_t    usVoltYelStart    =   0;
uint16_t    usVoltYelEnd      =   0;
uint16_t    usVoltGrnStart    =  40;
uint16_t    usVoltGrnEnd      =  60;

char  szAmpUnits[]            = "mA";
char  szVoltUnits[]           = "V";
char  szOhmUnits[]            = "R";

char  aszAmpTick[][10]        = {"0", "0.5" , "1.0" , "1.5" , "2.0"};
char  aszVoltTick[][10]       = {"0", "2.5" , "5.0" , "7.5" , "10.0"};
char  aszOhmTick[][10]        = {"0", ""    , "50"  , ""    , "100"};

// Meter is 239 pixels wide and 126 pixels high
// Colour draw order is red, orange, yellow, green. So red can be full scale with green drawn
// last on top to indicate a "safe" zone.
// Colour zones are set as a start and end percentage of full scale (0-100)
// No coloured zones if not defined
// If start and end of a colour zone are the same then that colour is not used



void setup(void){
  Screen.init           ();
  Screen.setRotation    (0);
  Screen.fillScreen     (TFT_BLACK);

  AmpMeter.setZones     (usAmpRedStart , usAmpRedEnd , usAmpOrgStart , usAmpOrgEnd , usAmpYelStart , usAmpYelEnd , usAmpGrnStart , usAmpGrnEnd);
  VoltMeter.setZones    (usVoltRedStart, usVoltRedEnd, usVoltOrgStart, usVoltOrgEnd, usVoltYelStart, usVoltYelEnd, usVoltGrnStart, usVoltGrnEnd);

  AmpMeter.analogMeter  (usAmpMeterXloc , usAmpMeterYloc , fAmpFullScale , szAmpUnits,
                         aszAmpTick[0]  , aszAmpTick[1]  , aszAmpTick[2] , aszAmpTick[3]);

  VoltMeter.analogMeter (usVoltMeterXloc, usVoltMeterYloc, fVoltFullScale, szVoltUnits,
                         aszVoltTick[0] , aszVoltTick[1] , aszVoltTick[2], aszVoltTick[3]);

  OhmMeter.analogMeter  (usOhmMeterXloc , usOhmMeterYloc , fOhmFullScale , szOhmUnits,
                         aszOhmTick[0]  , aszOhmTick[1]  , aszOhmTick[2] , aszOhmTick[3]);
  return;
} //setup


void loop(){
  static double           dDegree         = 0.0;
  static unsigned long    ulUpdateMillis  = 0;
  float                   fCurrent;
  float                   fVoltage;
  float                   fResistance;

  if ((millis() - ulUpdateMillis) >= wLoopPeriodMsec){
    ulUpdateMillis= millis();
    //Create a Sine wave for testing, fValue is in range 0 to 100
    dDegree += 4.0;
    if (dDegree > 360.0){
      dDegree= 0.0;
    } //if(dDegree>360)

    float fValue= 50.0 + (50.0 * sin((dDegree) * DEG_TO_RAD));

    fCurrent= fMapValue     (fValue, (float)0.0, (float)100.0, (float)0.0, (float)2.0);
    AmpMeter.updateNeedle   (fCurrent, 0);

    fVoltage= fMapValue     (fValue, (float)0.0, (float)100.0, (float)0.0, (float)10.0);
    VoltMeter.updateNeedle  (fVoltage, 0);

    fResistance= fMapValue  (fValue, (float)0.0, (float)100.0, (float)0.0, (float)100.0);
    OhmMeter.updateNeedle   (fResistance, 0);
  } //if((millis()-ulUpdateMillis)>=wLoopPeriodMsec)
  return;
} //loop


float fMapValue(float ip, float ipmin, float ipmax, float tomin, float tomax){
  return tomin + (((tomax - tomin) * (ip - ipmin))/ (ipmax - ipmin));
} //fMapValue
//Last line.
