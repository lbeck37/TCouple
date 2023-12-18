const char szSketchName[]  = "B32_TFTPrintTest.ino";
const char szFileDate[]    = "12/17/23b";

//Make sure the pin connections for the larger displays are correct by editing:
//  Sketches/libraries/TFT_eSPI/User_Setups/Setup1_ILI9341.h
//    (included from Sketches/libraries/TFT_eSPI/User_Setup_Select.h)

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <WiFi.h>
#include <B32_TCoupleLib.h>
#include <Streaming.h>

#define DO_ORIGINAL_DEMO    true

#define TFT_GREY 0x5AEB // New colour

#if false
  #define LB_BLACK    TFT_WHITE
  #define LB_WHITE    TFT_BLACK
  #define LB_YELLOW   TFT_RED
  #define LB_RED      TFT_YELLOW
  #define LB_BLUE     TFT_BLUE
  #define LB_GREEN    TFT_GREEN
#else
  #define LB_BLACK    TFT_BLACK
  #define LB_WHITE    TFT_WHITE
  #define LB_YELLOW   TFT_YELLOW
  #define LB_RED      TFT_RED
  #define LB_BLUE     TFT_BLUE
  #define LB_GREEN    TFT_GREEN
#endif
//TFT_eSPI Screen= TFT_eSPI();  // Invoke library

stMessageStructure   stDummyReadings;

void setup				      (void);
void loop				        (void);


void setup(void) {
	Serial.begin(115200);
	delay(100);
	Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
	WiFi.mode(WIFI_MODE_STA);

  Serial << "loop(): Call Screen.init()" << endl;
  Screen.init         ();
  Screen.invertDisplay(1);
	return;
}//setup


void loop() {
  static int  wCount= 0;
  float       fDummyDegF;
  static int  wFraction    = 0;

  Screen.setRotation  (0);                    //ED: Portrait USB at upper-right, WM: Landscape USB on left
  //Screen.setRotation  (1);                    //Landscape, USB at upper-left, WM: Portrait, USB on top
  //Screen.setRotation  (2);                    //Portrait USB at lower-left
  //Screen.setRotation  (3);                    //Landscape, USB at lower-right
  //Screen.setRotation  (4);                    //Portrait USB at lower-left, text right-to-left
  //Screen.fillScreen   (LB_BLACK);
  
#if !DO_ORIGINAL_DEMO
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
#else
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'Screen.println'
  //  or stay on the line is there is room for the text with Screen.print)
  Screen.setCursor    (0, 0, 2);
  Screen.setTextColor (LB_WHITE, LB_BLACK);    //White on black
  Screen.setTextSize  (1);
  //Screen.setTextSize  (2);

/*
  Serial << "setup(): Fill screen with WHITE for 2 seconds" << endl;
  Screen.fillScreen(LB_WHITE);
  delay (2000);

  Serial << "setup(): Fill screen with red for 2 seconds" << endl;
  Screen.fillScreen(LB_RED);
  delay (2000);

  Serial << "setup(): Fill screen with YELLOW for 2 seconds" << endl;
  Screen.fillScreen(LB_YELLOW);
  delay (2000);
*/

  Serial << "setup(): Fill screen with TFT_BLACK for 2 seconds" << endl;
  Screen.fillScreen(LB_BLACK);
  delay (2000);

  // Set the font colour to be yellow with no background, set to font 7
  Screen.setTextColor (LB_YELLOW);
  Screen.setTextFont  (7);
  double dNumber= 1234.56;
  Screen << dNumber << endl;
  
  Screen.setTextColor (LB_GREEN, LB_BLACK);     //Green on black
  Screen.setTextFont  (4);
  Screen << "Above is Font7 TFT_YELLOW" << endl;

  // Set the font color to be green with black background, set to font 4
  Screen.setTextColor (LB_GREEN, LB_BLACK);     //Green on black
  Screen.setTextFont  (4);
  Screen << "Font4 TFT_GREEN" << endl;

  // Set the font colour to be red with black background, set to font 4
  Screen.setTextColor (LB_RED, LB_BLACK);      //Red on black
  Screen.setTextFont  (4);
  Screen << "Font4 TFT_RED" << endl;
  //Screen.println(3735928559L, HEX); // Should print DEADBEEF
  Screen << _HEX(3735928559L) << endl; // Should print DEADBEEF

  Screen.setTextFont  (2);
  Screen << "Font2 TFT_RED" << endl;
  // This next line is deliberately made too long for the display width to test
  // automatic text wrapping onto the next line
  //Screen << "Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!" << endl;

  // Test some print formatting functions
  float fNumber     = 123.45;
  int   wInteger    =  512 + 2;
  //Screen.setTextColor(LB_BLUE);
  Screen.setTextColor (LB_RED);
  Screen.setTextFont  (4);
  Screen << "Font4 LB_RED" << endl;
  Screen << "Float   " << fNumber         << endl;
  Screen << "Integer " << wInteger        << endl;
  Screen << "Binary  " << _BIN(wInteger)  << endl;
  Screen << "Hex     " << _HEX(wInteger)  << endl;
#endif
  delay(4000);
  return;
}//loop
//Last line.
