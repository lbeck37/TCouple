const char szSketchName[]  = "B32_Adafruit_TCouple.ino";
const char szFileDate[]    = "10/10/23d";
/***************************************************
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K
  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269
  Uses SPI to communicate, 3 pins are required to interface

  Initially by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
//Default thermocouple amp will be MAX31855 due to 14 bit vs 12 bit and being newer?
#define DO_MAX6675		false

#include <Arduino.h>
#include <Streaming.h>
#include <SPI.h>
#if DO_MAX6675
  #include "Adafruit_MAX6675.h"
#else
  #include "Adafruit_MAX31855.h"
#endif

//TTGO T-Display ESP32 pins 13, 17, 25, 26, 27, 32 and 33 are OK to use
//RandomNerdTutorials "ESP32 Pinout Reference: Which GPIO pins should you use?"

const int     wTCoupleSPIDataOut    = 32;
const int     wTCoupleSPIClk        = 33;
const double  dError                = -99.999;

#if DO_MAX6675
  const int wNumTCouples        = 3;
  const int wTCoupleCS[]        = {25, 26, 27};
#else
  //8 channel MAX31855 board
  const int wNumTCouples        =  8;
  const int w8ChanBoardCS		    = 17;
  const int wTCoupleSelectBit[] = {27, 26, 25};
  const int wNumSelectBits		  = 3;
#endif

#if DO_MAX6675
Adafruit_MAX6675 TCouple[]=
	{Adafruit_MAX6675(wTCoupleSPIClk, wTCoupleCS[0], wTCoupleSPIDataOut),
	 Adafruit_MAX6675(wTCoupleSPIClk, wTCoupleCS[1], wTCoupleSPIDataOut),
	 Adafruit_MAX6675(wTCoupleSPIClk, wTCoupleCS[2], wTCoupleSPIDataOut)};
#else
Adafruit_MAX31855 TCouple=
	Adafruit_MAX31855(wTCoupleSPIClk, w8ChanBoardCS, wTCoupleSPIDataOut);
#endif

//Function prototypes
void 	    setup			      (void);
void 	    loop			      (void);
double    dDegF           (double dDegC);
void      SetupPins       (void);
void      ReadTCouples    (void);
void      SelectTCouple   (int wTCoupleNum);

#if DO_MAX6675
  void 	  Read_MAX6675	  (void);
#else
  double 	dRead_MAX31855  (void);
#endif

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

#if DO_MAX6675
  //Nothing to set up
#else
  Serial << "setup():  8 channel MAX31855 test" << endl;
  // wait for MAX chip to stabilize
  delay(500);
  Serial << "setup(): Initializing sensor" << endl;
  if (!TCouple.begin()) {
    Serial << "setup(): ERROR" << endl;
    while (1){
      delay(10);
    } //while
  } //if(!TCouple.begin())
#endif
  SetupPins();
  Serial << "setup(): DONE." << endl;
  return;
} //setup


void loop() {
  // basic readout test, just print the current temp
#if DO_MAX6675
	Read_MAX6675();
#else
	ReadTCouples();
#endif
  delay(5000);
} //loop


void ReadTCouples(void){
  for (int wTCoupleNum= 0; wTCoupleNum < 8; wTCoupleNum++){
    SelectTCouple(wTCoupleNum);
    double dTCoupleDegF= dRead_MAX31855();
    if ( dTCoupleDegF != dError){
      Serial << "ReadTCouples(): TCouple= " << wTCoupleNum << ", DegF= " << dTCoupleDegF << endl;
    }
    else {
      Serial << "ReadTCouples(): TCouple= " << wTCoupleNum << ", ERROR" << dTCoupleDegF << endl;
    }
  }
  return;
} //ReadTCouples


void SelectTCouple(int wTCoupleNum){
  digitalWrite(wTCoupleSelectBit[0], wTCoupleNum & 1? HIGH: LOW);
  digitalWrite(wTCoupleSelectBit[1], wTCoupleNum & 2? HIGH: LOW);
  digitalWrite(wTCoupleSelectBit[2], wTCoupleNum & 4? HIGH: LOW);
  return;
} //SelectTCouple


double dDegF(double dDegC){
  return ((dDegC * 9.0) / 5.0) + 32.0;
} //dDegF


void SetupPins(void){
	Serial << "SetupPins(): Begin" << endl;
	pinMode(wTCoupleSPIDataOut	, INPUT);
	pinMode(wTCoupleSPIClk		  , OUTPUT);
	pinMode(w8ChanBoardCS		    , OUTPUT);

	for(int wSelectBit= 0; wSelectBit < wNumSelectBits; wSelectBit++){
		pinMode(wTCoupleSelectBit[wSelectBit], OUTPUT);
	}	//for(int wSelectBit=0;...
	return;
}	//SetupPins


#if !DO_MAX6675
double dRead_MAX31855(){
   Serial.print("\nRead_MAX31855(): Internal Temp = ");
   double   dReturn;
   double   dInternalDegF= dDegF(TCouple.readInternal());
   Serial << "Read_MAX31855(): Read_MAX31855(): Internal DegF= " << dInternalDegF << endl;

   double dDegTCoupleDegC= TCouple.readCelsius();
   if (isnan(dDegTCoupleDegC)) {
     dReturn= dError;
     Serial << "Read_MAX31855(): Read_MAX31855(): Thermocouple fault(s) detected!" << endl;
     //uint8_t e = TCouple1.readError();
     uint8_t ucError= TCouple.readError();

     if (ucError & MAX31855_FAULT_OPEN){
       Serial << "Read_MAX31855(): Read_MAX31855(): FAULT: Thermocouple is open - no connections." << endl;
     }
     if (ucError & MAX31855_FAULT_SHORT_GND){
       Serial << "Read_MAX31855(): Read_MAX31855(): FAULT: Thermocouple is short-circuited to GND." << endl;
     }
     if (ucError & MAX31855_FAULT_SHORT_VCC){
       Serial.println("loop(): FAULT: Thermocouple is short-circuited to VCC.");
       Serial << "Read_MAX31855(): Read_MAX31855(): FAULT: Thermocouple is short-circuited to VCC." << endl;
     }
   }	//if(isnan(dDegC))
   else {
     dReturn= dDegF(dDegTCoupleDegC);
     Serial << "Read_MAX31855(): TCouple DegF= " << dReturn << endl;
   }	//if(isnan(c))else
   return dReturn;
}	//Read_MAX31855


#else
void Read_MAX6675(){
  for (int wTCoupleNum= 0; wTCoupleNum < wNumTCouples; wTCoupleNum++) {
    Serial << "Read_MAX6675(): TCouple= " << wTCoupleNum <<
        ", Deg F= " << TCouple[wTCoupleNum].readFarenheit() << endl;
  } //for(int wTCoupleNum=0;...
  Serial << endl;
  return;
} //Read_MAX6675
#endif
//Last line.
