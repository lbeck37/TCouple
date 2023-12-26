/* Beck, Analogue_meters.ino 12/26/23b
  Example animated analogue meters
  Needs Font 2 (also Font 4 if using large scale label)
  Requires widget library here: https://github.com/Bodmer/TFT_eWidget*/

#include <TFT_eSPI.h>     // Hardware-specific library*/
#include <TFT_eWidget.h>  // Widget library

TFT_eSPI      Screen    = TFT_eSPI();      // Invoke custom library

MeterWidget   AmpMeter          = MeterWidget(&Screen);
MeterWidget   VoltMeter         = MeterWidget(&Screen);
MeterWidget   OhmMeter          = MeterWidget(&Screen);

const int     wLoopPeriodMsec   = 35; // Display updates every 35 ms

void setup(void){
  uint16_t    usAmpMeterXloc    = 0;
  uint16_t    usAmpMeterYloc    = 0;

  uint16_t    usVoltMeterXloc   = 0;
  uint16_t    usVoltMeterYloc   = 128;

  uint16_t    usOhmMeterXloc    = 0;
  uint16_t    usOhmMeterYloc    = 256;

  float       fAmpFullScale     = 2.0;
  float       fVoltFullScale    = 10.0;
  float       fOhmFullScale     = 100.0;

  const char  szAmpUnits[]      = "mA";
  const char  szVoltUnits[]     = "V";
  const char  szOhmUnits[]      = "R";

  const char  aszAmpTick[][10]  = {"0", "0.5", "1.0", "1.5", "2.0"};
  const char  aszVoltTick[][10] = {"0", "2.5", "5", "7.5", "10"};
  const char  aszOhmTick[][10]  = {"0", "", "50", "", "100"};

  Screen.init         ();
  Screen.setRotation  (0);
  Screen.fillScreen   (TFT_BLACK);

  Serial.begin(115200); // For debug

  // Meter is 239 pixels wide and 126 pixels high
  // Colour draw order is red, orange, yellow, green. So red can be full scale with green drawn
  // last on top to indicate a "safe" zone.
  // Colour zones are set as a start and end percentage of full scale (0-100)
  // No coloured zones if not defined
  // If start and end of a colour zone are the same then that colour is not used
  //                     --Red--  -Org-   -Yell-  -Grn-
  AmpMeter.setZones     (75, 100, 50, 75, 25, 50, 0, 25); // Example here red starts at 75% and ends at 100% of full scale
  VoltMeter.setZones    (0, 100, 25, 75, 0, 0, 40, 60);

  AmpMeter.analogMeter  (usAmpMeterXloc , usAmpMeterYloc , fAmpFullScale , szAmpUnits,
                         aszAmpTick[0]  , aszAmpTick[1]  , aszAmpTick[2] , aszAmpTick[3]);

  VoltMeter.analogMeter (usVoltMeterXloc, usVoltMeterYloc, fVoltFullScale, szVoltUnits,
                         aszVoltTick[0] , aszVoltTick[1] , aszVoltTick[2], aszVoltTick[3]);

  OhmMeter.analogMeter  (usOhmMeterXloc , usOhmMeterYloc , fOhmFullScale , szOhmUnits,
                         aszOhmTick[0]  , aszOhmTick[1]  , aszOhmTick[2] , aszOhmTick[3]);
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
} //loop


float fMapValue(float ip, float ipmin, float ipmax, float tomin, float tomax){
  return tomin + (((tomax - tomin) * (ip - ipmin))/ (ipmax - ipmin));
} //fMapValue
//Last line.
