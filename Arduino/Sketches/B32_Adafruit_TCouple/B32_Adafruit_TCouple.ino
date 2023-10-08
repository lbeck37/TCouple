const char szSketchName[]  = "B32_Adafruit_TCouple.ino";
const char szFileDate[]    = "10/7/23f";
/***************************************************
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K
  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269
  Uses SPI to communicate, 3 pins are required to interface

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Streaming.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"
//#include "Adafruit_MAX31855_Beck.h"

// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.

//TTGO T-Display ESP32 pins 13, 17, 25, 26, 27, 32 and 33 are OK to use
//RandomNerdTutorials "ESP32 Pinout Reference: Which GPIO pins should you use?"

const int wTCoupleSPIDataOut    = 32;
const int wTCoupleSPIClk        = 33;
const int wTCoupleCS[]          = {25, 26, 27};

/*
// initialize the Thermocouple
Adafruit_MAX31855 TCouple1(wTCoupleSPIClk, wTCoupleCS[0], wTCoupleSPIDataOut);
Adafruit_MAX31855 TCouple2(wTCoupleSPIClk, wTCoupleCS[1], wTCoupleSPIDataOut);
Adafruit_MAX31855 TCouple3(wTCoupleSPIClk, wTCoupleCS[2], wTCoupleSPIDataOut);
*/

Adafruit_MAX31855 TCouple[]= {Adafruit_MAX31855(wTCoupleSPIClk, wTCoupleCS[0], wTCoupleSPIDataOut),
		                      Adafruit_MAX31855(wTCoupleSPIClk, wTCoupleCS[1], wTCoupleSPIDataOut),
		                      Adafruit_MAX31855(wTCoupleSPIClk, wTCoupleCS[2], wTCoupleSPIDataOut)};
// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS);

// Example creating a thermocouple instance with hardware SPI
// on SPI1 using specified CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS, SPI1);

//Function prototypes
void setup();
void loop();

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  //while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc

  Serial.println("setup(): MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);
  Serial.print("setup(): Initializing sensor...");
  //if (!TCouple1.begin()) {
  if (!TCouple[0].begin()) {
    Serial.println("setup(): ERROR.");
    while (1) delay(10);
  }

  // OPTIONAL: Can configure fault checks as desired (default is ALL)
  // Multiple checks can be logically OR'd together.
  // thermocouple.setFaultChecks(MAX31855_FAULT_OPEN | MAX31855_FAULT_SHORT_VCC);  // short to GND fault is ignored

  Serial.println("\nsetup(): DONE.");
}

void loop() {
  // basic readout test, just print the current temp
   Serial.print("\nloop(): Internal Temp = ");
   //.println(TCouple1.readInternal());
   Serial.println(TCouple[0].readInternal());

   //double c = thermocouple.readCelsius();
   //double c = TCouple1.readCelsius();
   double c = TCouple[0].readCelsius();
   if (isnan(c)) {
     Serial.println("loop(): Thermocouple fault(s) detected!");
     //uint8_t e = TCouple1.readError();
     uint8_t e = TCouple[0].readError();
     if (e & MAX31855_FAULT_OPEN) Serial.println("loop(): FAULT: Thermocouple is open - no connections.");
     if (e & MAX31855_FAULT_SHORT_GND) Serial.println("loop(): FAULT: Thermocouple is short-circuited to GND.");
     if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("loop(): FAULT: Thermocouple is short-circuited to VCC.");
   }	//if(isnan(c))
   else {
     Serial << "loop(): C = " << c << endl;
	 Serial << "loop(): F = " << (c*1.8 +32.0) << endl;
   }	//if(isnan(c))else

   delay(5000);
}
