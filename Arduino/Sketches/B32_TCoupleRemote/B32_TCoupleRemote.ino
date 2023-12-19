const char szSketchName[]  = "B32_TCoupleRemote.ino";
const char szFileDate[]    = "12/18/23c";
/* MAX31855 library example sketch
 This sample code is designed to be used on the MAX31855x8 breakout board.
 The board has a single MAX31855 IC on it, and uses a multiplexer
 to select the correct thermoucouple.  The MAX31855 takes around 100ms to take
 an accurate temperature reading, so the best sample rate one can expect is to
 sample all 8 channels once-per-second.
 There are 2 versions of the MAX31855x8 board: 3.3V and 5V.
 There is a solder jumper on the board that can be changed to go from 5V or 3.3V.
 This sample code shows how to take a temperature reading:
 1. Set the multiplexer to the correct thermocouple channel
 2. Wait 125ms (0.125 seconds) for the MAX31855 to take an accurate reading
 3. Get the temperature from the MAX31855
 In the MAX31855 library, there are 2 functions:
 1. float readJunction([CELCUIS|FAHRENHEIT])
      Returns the internal temperature of the MAX31855 IC
 2. float readThermocouple([CELCUIS|FAHRENHEIT])
      Returns the temperature of the probe connected to the MAX31855
 readJunction() and readThermocouple() will return the temperature,
 or one of these errors:
 #define FAULT_OPEN        10000  // No thermocouple
 #define FAULT_SHORT_GND   10001  // Thermocouple short to ground
 #define FAULT_SHORT_VCC   10002  // Thermocouple short to VCC
 #define NO_MAX31855       10003  // MAX31855 not communicating
 Note:  If you connect the thermocouple probe the wrong way around, the temperature
 will go up instead of down (and vice versa).  No problem, just reverse the terminals.
 Released under WTFPL license, 27 October 2014 by Peter Easton
*/

//This sketch, (B32_TCoupleDisplay.ino), and B32_TCoupleModule.ino share WiFi
//communication code from the esp_now.h library.
#define ILI9341             false
#define TTGO_T_DISPLAY      true
//#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <B32_TCoupleLib.h>

//eBoardPinColor   eReceiverBoardPinColor    {eBoardPinColor::eWhitePin};
eBoardPinColor   eReceiverBoardPinColor    {eBoardPinColor::e3dot2NoPin};

const bool  bNoTCouples     = true;

//ESP32 GPIO pin numbers (range from 0 to 39)
#define T0   27
#define T1   26
#define T2   25
#define MISO 32
#define CS   17
#define SCK  33

//Create the temperature object, defining the pins used for communication
MAX31855 TCoupleObject = MAX31855(MISO, CS, SCK);

double  dJunctionDegF;
double  dDummyJunctionDegF  = 60.00;
double  dDummyAddDegF       =  0.00;

//Function prototypes
void  setup                   (void);
void  loop                    (void);
void  SetupPins               (void);
void  ReadAmbiant             (void);
void  ReadTCouples            (void);

void setup() {
  Serial.begin(115200);

  SetupPins();
  SetupScreen();
  SetupESP_NOW();
  delay(500);

  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  WiFi.macAddress(aucMyMACAddress);
  ShowMyMAC(false);
  return;
}   //setup


void loop() {
  ReadAmbiant();
  ReadTCouples();
  PrintTemperatures();
  SendDataToDisplayBoard();
  UpdateScreen(stOutgoingReadings);

  //No delay, loop as fast as possible
  return;
}   //loop


void SetupPins(void){
  pinMode(T0,   OUTPUT);
  pinMode(T1,   OUTPUT);
  pinMode(T2,   OUTPUT);

  pinMode(MISO, INPUT);
  pinMode(CS,   OUTPUT);
  pinMode(SCK,  OUTPUT);

  delay(200);
  return;
} //SetupPins


void ReadAmbiant(void){
  if (!bNoTCouples){
    dJunctionDegF= TCoupleObject.readJunction(FAHRENHEIT);
  }   //if(!bNoTCouples)
  else{
    dDummyJunctionDegF += 0.10;
    dJunctionDegF= dDummyJunctionDegF;
  }   //if(!bNoTCouples)else
  return;
} //ReadAmbiant


void ReadTCouples(void){
/*
  //Read the temperatures of the 8 thermocouples
  for (int wTCoupleNum=0; (wTCoupleNum < wNumTCouples); wTCoupleNum++) {
    //Select the thermocouple
    digitalWrite(T0, wTCoupleNum & 1? HIGH: LOW);
    digitalWrite(T1, wTCoupleNum & 2? HIGH: LOW);
    digitalWrite(T2, wTCoupleNum & 4? HIGH: LOW);
    //The MAX31855 takes 100ms to sample the TCouple.
    //Wait a bit longer to be safe.  We'll wait 0.125 seconds
    delay(125);

    //adTCoupleDegF[wTCoupleNum]= TCoupleObject.readThermocouple(FAHRENHEIT);
    stOutgoingReadings.adTCoupleDegF[wTCoupleNum]= TCoupleObject.readThermocouple(FAHRENHEIT);
    if (stOutgoingReadings.adTCoupleDegF[wTCoupleNum] == FAULT_OPEN){
      //Break out of for loop, go to top of for loop and next TCouple
      continue;
    } //if(stOutgoingReadings.adTCoupleDegF[wTCoupleNum]==FAULT_OPEN)
  }   //for(int wTCoupleNum=0;wTCoupleNum<8;wTCoupleNum++)
*/
  //Read the temperatures of the 8 thermocouples
  for (int wTCoupleNum=0; (wTCoupleNum < wNumTCouples); wTCoupleNum++) {
    //Select the thermocouple
    digitalWrite(T0, wTCoupleNum & 1? HIGH: LOW);
    digitalWrite(T1, wTCoupleNum & 2? HIGH: LOW);
    digitalWrite(T2, wTCoupleNum & 4? HIGH: LOW);
    //The MAX31855 takes 100ms to sample the TCouple.
    //Wait a bit longer to be safe.  We'll wait 0.125 seconds
    delay(125);

    if (!bNoTCouples){
      //adTCoupleDegF[wTCoupleNum]= TCoupleObject.readThermocouple(FAHRENHEIT);
      stOutgoingReadings.adTCoupleDegF[wTCoupleNum]= TCoupleObject.readThermocouple(FAHRENHEIT);
    } //if(!bNoTCouples)
    else{
      dDummyAddDegF += 0.10;
      stOutgoingReadings.adTCoupleDegF[wTCoupleNum]= (100.00 + (wTCoupleNum * 10.00) + dDummyAddDegF);
    } //if(!bNoTCouples)else

    if (stOutgoingReadings.adTCoupleDegF[wTCoupleNum] == FAULT_OPEN){
      //Break out of for loop, go to top of for loop and next TCouple
      continue;
    } //if(stOutgoingReadings.adTCoupleDegF[wTCoupleNum]==FAULT_OPEN)
  } //for(int wTCoupleNum=0;wTCoupleNum<8;wTCoupleNum++)
  return;
}   //ReadTCouples
//Last line.
