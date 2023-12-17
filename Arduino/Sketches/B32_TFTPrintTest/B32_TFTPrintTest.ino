const char szSketchName[]  = "B32_TFTPrintTest.ino";
const char szFileDate[]    = "12/16/23b";

//Make sure the pin connections for the larger displays are correct by editing:
//  Sketches/libraries/TFT_eSPI/User_Setups/Setup1_ILI9341.h
//    (included from Sketches/libraries/TFT_eSPI/User_Setup_Select.h)

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <WiFi.h>
#include <B32_TCoupleLib.h>
#include <Streaming.h>

#define TFT_GREY 0x5AEB // New colour

//TFT_eSPI Screen= TFT_eSPI();  // Invoke library

stMessageStructure   stDummyReadings;

void setup				      (void);
void loop				        (void);


void setup(void) {
	Serial.begin(115200);
	delay(100);
	Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
	WiFi.mode(WIFI_MODE_STA);
	return;
}//setup


void loop() {
  static int  wCount= 0;
  float       fDummyDegF;
  static int  wFraction    = 0;

  Serial << "loop(): Call Screen.init()" << endl;
  Screen.init         ();
  //Screen.setRotation  (3);                    //USB at lower right
  Screen.setRotation  (1);                    //USB at upper left
  Screen.fillScreen   (TFT_BLACK);
  
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'Screen.println'
  //  or stay on the line is there is room for the text with Screen.print)
  Screen.setCursor    (0, 0, 2);
  Screen.setTextColor (TFT_WHITE, TFT_BLACK);    //White on black
  Screen.setTextSize  (1);
  //Screen.setTextSize  (2);
  Screen << "Sketch: " << szSketchName << ", " << szFileDate << endl;
  Screen << "My MAC address is- " << WiFi.macAddress() << endl;
  
  for (int wTCoupleNum=0; wTCoupleNum < wNumTCouples; wTCoupleNum++) {
    float fFraction= ((float)(wFraction++ % 100) / 100.0);
    fDummyDegF= (float)((wTCoupleNum * 100) + (wCount % 100)) + fFraction;
    stDummyReadings.adTCoupleDegF[wTCoupleNum]= fDummyDegF;
  } //for(int wTCoupleNum=0;wTCoupleNum<wNumTCouples;wTCoupleNum++)

  UpdateScreen(stDummyReadings);

/*
  // Set the font colour to be yellow with no background, set to font 7
  Screen.setTextColor (TFT_YELLOW); Screen.setTextFont(7);
  double dNumber= 1234.56;
  Screen << dNumber << endl;
  
  // Set the font colour to be red with black background, set to font 4
  Screen.setTextColor (TFT_RED, TFT_BLACK);      //Red on black
  Screen.setTextFont  (4);
  Screen.println(3735928559L, HEX); // Should print DEADBEEF

  // Set the font colour to be green with black background, set to font 4
  Screen.setTextColor (TFT_GREEN, TFT_BLACK);     //Green on black
  Screen.setTextFont  (4);
  Screen << "Hey Dude- " << endl << "I implore thee," << endl;

  Screen.setTextFont  (2);
  Screen << "my foonting turlingdromes." << endl;
  Screen << "And hooptiously drangle me" << endl;
  Screen << "with crinkly bindlewurdles," << endl;
  // This next line is deliberately made too long for the display width to test
  // automatic text wrapping onto the next line
  Screen << "Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!" << endl;

  // Test some print formatting functions
  float fnumber = 123.45;
   // Set the font colour to be blue with no background, set to font 4
  Screen.setTextColor(TFT_BLUE);
  Screen.setTextFont(4);
  Screen.print("Float = ");
  Screen.println(fnumber);           // Print floating point number
  Screen.print("Binary = ");
  Screen.println((int)fnumber, BIN); // Print as integer value in binary
  Screen.print("Hexadecimal = ");
  Screen.println((int)fnumber, HEX); // Print as integer number in Hexadecimal
*/
  delay(3000);
  return;
}//loop
//Last line.
