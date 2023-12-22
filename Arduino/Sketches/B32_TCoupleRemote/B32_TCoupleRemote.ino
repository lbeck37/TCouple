const char szSketchName[]  = "B32_TCoupleRemote.ino";
const char szFileDate[]    = "12/22/23C";
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

//In Project Properties (right click project), Sloeber, append -DTTGO_T_DISPLAY to c++ compile
#include <TFT_eSPI.h>
#include <SPI.h>
#include <B32_TCoupleLib.h>

//eBoardPinColor   eReceiverBoardPinColor    {eBoardPinColor::eWhitePin};
eBoardPinColor   eReceiverBoardPinColor    {eBoardPinColor::e3dot2NoPin};

#if defined(B32_TTGO_T_DISPLAY)
  const uint8_t ucRotation  = 3;  //1 WM: P, USB Top/ ED: L, USB UL/ TT: L, USB L
#endif

#if defined(B32_ILI9341)
  const uint8_t ucRotation  = 0;  //0 WM: L, USB L/ DIY: P, USB Bot/ ED: P, USB UR/ TT: P, USB Bot
#endif

#if defined(B32_ILI9488_ESP32)
  const uint8_t ucRotation  = 0;  //0 WM: L, USB L/ DIY: P, USB Bot/ ED: P, USB UR/ TT: P, USB Bot
#endif

//Function prototypes
void  setup                   (void);
void  loop                    (void);


void setup() {
  Serial.begin(115200);

  SetupPins();
  SetupScreen(ucRotation);
  SetupESP_NOW();
  delay(500);

  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  WiFi.macAddress(aucMyMACAddress);
  ShowMyMAC(false);
  return;
}   //setup


void loop() {
  ReadAmbiant             ();
  ReadTCouples            (stOutgoingReadings);
  PrintTemperatures       (stOutgoingReadings);
  SendDataToDisplayBoard  (stOutgoingReadings);
  UpdateScreen            (stOutgoingReadings);
  //No delay except for 125mSec between each tcouple reading
  return;
}   //loop
//Last line.
