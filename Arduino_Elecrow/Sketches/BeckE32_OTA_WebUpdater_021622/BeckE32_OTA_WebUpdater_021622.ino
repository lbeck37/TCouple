// LastMinuteEngineers.com
//Works with ESP32 1.0.6, 2/17/22
const char szSketchName[]  = "BeckE32_OTA_WebUpdater_021622.ino";
const char szFileDate[]    = "2/15/24n, was 2/20/22x";

#define DO_MAX6675      false
#define DO_MAX31855     false
#define DO_OTA          true

#if DO_OTA
  #include <BeckE32_OTALib.h>
  #include <WiFi.h>
#endif

#if DO_MAX6675
  #include <max6675.h>
#endif
#if DO_MAX31855
  #include <MAX31855.h>
#endif

#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

const char* szRouterName  = "Aspot24b";
const char* szRouterPW    = "Qazqaz11";

const char* szWebHostName = "OTADemo";
const float fLibraryError = 2000.00;

//ESP32
// https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
//static const byte    cSPI_MOSI_Pin    = 23;     // MasterOutSlaveIn is not used, chips are read only
static const byte      cSPI_MISO_Pin    = 19;
static const byte      cSPI_CLK_Pin     = 18;
static const int       wNumThermos      =  3;

static const uint8_t   acSPI_CS_Pin[] {0, 2, 4, 5};

#if DO_MAX31855
MAX31855  MAX31855_Object0(acSPI_CS_Pin[0]);     //Dummy so we can call thermocouples 1, 2 and 3
MAX31855  MAX31855_Object1(acSPI_CS_Pin[1]);
MAX31855  MAX31855_Object2(acSPI_CS_Pin[2]);
MAX31855  MAX31855_Object3(acSPI_CS_Pin[3]);

//MAX31855  aThermos[wNumThermos + 1]= {MAX31855_Object0, MAX31855_Object1, MAX31855_Object2, MAX31855_Object3};
//MAX31855  aThermos[]= {MAX31855_Object0, MAX31855_Object1, MAX31855_Object2, MAX31855_Object3};
MAX31855  aoMAX31855[] {MAX31855_Object0, MAX31855_Object1, MAX31855_Object2, MAX31855_Object3};
#endif

#if DO_MAX6675
//Test SPI bus by connecting a MAX6675 and a MAX31855 on a small BB
//The pin-outs for the two break-out boards are the same except CS and MISO are reversed
//I had to connect red thermo wire to "+" on the MAX6675
MAX6675   oMAX6675_Obj0(cSPI_CLK_Pin, acSPI_CS_Pin[0], cSPI_MISO_Pin);
MAX6675   oMAX6675_Obj1(cSPI_CLK_Pin, acSPI_CS_Pin[1], cSPI_MISO_Pin);
MAX6675   oMAX6675_Obj2(cSPI_CLK_Pin, acSPI_CS_Pin[2], cSPI_MISO_Pin);
MAX6675   oMAX6675_Obj3(cSPI_CLK_Pin, acSPI_CS_Pin[3], cSPI_MISO_Pin);

MAX6675  aoMAX6675[] {oMAX6675_Obj0, oMAX6675_Obj1, oMAX6675_Obj2, oMAX6675_Obj3};
#endif


void setup(void) {
  Serial.begin(115200);

  Serial << BLOG << endl << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

#if DO_MAX6675 || DO_MAX31855
  Serial << " setup(): Built for WRover-Kit, SPI MISO(green) on pin " << cSPI_MISO_Pin << ", SPI CLK(orange) on pin " << cSPI_CLK_Pin << endl;

  for (int wThermo= 1; wThermo <= wNumThermos; wThermo++){
    Serial << BLOG << " setup(): Thermo #" << wThermo << " CS pin is " << acSPI_CS_Pin[wThermo] << endl;
  } //for
#endif

#if DO_OTA
  // Start WiFi and wait for connection to the network
  Serial << BLOG  << "setup(): Call  WiFi.begin(" << szRouterName << ", " << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);
  Serial << BLOG  << " setup(): Wait for WiFi.status() to return WL_CONNECTED" << szRouterName << ", " << szRouterPW << ")" << endl;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial << endl << BLOG << " setup(): Connected to " << szRouterName << ", IP address to connect to is " << WiFi.localIP() << endl;
#endif

#if DO_MAX31855     //MAX6675 thermocouple reader doesn't have a setup routine
  for (int wThermo= 1; wThermo > wNumThermos; wThermo++){
    aoMAX31855[wThermo].begin();
  } //for

  for (int wThermo= 1; wThermo <= wNumThermos; wThermo++){
    uint16_t usChipID= aoMAX31855[wThermo].getChipID();
    if (usChipID != MAX31855_ID){
      Serial << BLOG << " loop(): ERROR: Thermocouple Number " << wThermo << " is not connected." << endl;
    }
  } //for
#endif

#if DO_OTA
  Serial << BLOG << " setup(): Call SetupWebServer(" << szWebHostName << ")" << endl;
  SetupWebserver(szWebHostName);
#endif
  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void loop(void) {
  int32_t   wRawData;
  float     afDegreeC[] { 37.00, 37.01, 37.02, 37.03};

#if DO_MAX31855
  for (int wThermo= 1; wThermo <= wNumThermos; wThermo++){
    wRawData= aoMAX31855[wThermo].readRawData();
    afDegreeC[wThermo]= aoMAX31855[wThermo].getTemperature(wRawData);
    if (afDegreeC[wThermo] == fLibraryError){
      Serial << BLOG << " loop(): ERROR: MAX31855 Thermocouple Number " << wThermo << " is open, shorted or there is a communication error. " << endl;
    }
    else{
      Serial << BLOG << " loop(): Thermocouple Number " << wThermo << " is at " << afDegreeC[wThermo] << "C" << endl;
    }
  } //for
#endif

#if DO_MAX6675
  double adMAX6675DegF[4];
  for (int wThermo= 1; wThermo <= wNumThermos; wThermo++){
    double dfMAX6675DegC= aoMAX6675[wThermo].readCelsius();
    double dfMAX6675DegF= aoMAX6675[wThermo].readFahrenheit();
    adMAX6675DegF[wThermo]= dfMAX6675DegF;
    //Serial << BLOG << "loop(): Thermocouple Number " << wThermo << " is at " << dfMAX6675DegF << "F" << ", " << dfMAX6675DegC << "C" << endl;
   }
  Serial << BLOG << " loop(): Thermo #1= " << adMAX6675DegF[1] << "F, #2= " << adMAX6675DegF[2] << "F, #3=" << adMAX6675DegF[3] << endl;
#endif
  delay(1000);

#if DO_OTA
  HandleOTAWebserver();
#endif
  return;
} //loop
//Last Line.
