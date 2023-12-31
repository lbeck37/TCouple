const char szSketchName[]  = "BeckE32_TTGO_PrintTest.ino";
const char szFileDate[]    = "11/16/23d";
//Was BeckE32_TTGO_PrintTest.ino 3/23/21d
/*Test the tft.print() viz embedded tft.write() function
 This sketch used font 2, 4, 7
 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.
 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */
#include <Streaming.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

#define TFT_GREY 0x5AEB // New colour

TFT_eSPI tft = TFT_eSPI();  // Invoke library

//Func protos
void setup  (void);
void loop   (void);

void setup(void) {
  bool bLandscape= false;
	Serial.begin(115200);
	delay(500);
	//Serial.println("\nBeckE32_TTGO_PrintTest, 3/23/21d: setup(): Begin");
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  // 0 is USB top     Portrait (Default)
  // 1 is USB right   Landscape
  // 2 is USB bottom  Portrait
  // 3 is USB left    Landscape
	tft.init();
	if(bLandscape){
    tft.setRotation(1);
	}
	else{
    tft.setRotation(2);
	}
	return;
}	//setup


void loop() {
  // Fill screen with grey so we can see the effect of printing with and without 
  // a background colour defined
  tft.fillScreen(TFT_GREY);
  
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  tft.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
  // We can now plot text on screen using the "print" class
  tft.println("Hello World!");
  
  // Set the font colour to be yellow with no background, set to font 7
  tft.setTextColor(TFT_YELLOW); tft.setTextFont(7);
  tft.println(1234.56);
  
  // Set the font colour to be red with black background, set to font 4
  tft.setTextColor(TFT_RED,TFT_BLACK);
  tft.setTextFont(4);
  //tft.println(3735928559L, HEX); // Should print DEADBEEF

  // Set the font colour to be green with black background, set to font 4
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextFont(4);
  //tft.println("Dude!"); 
  tft.println(szFileDate); 
  tft.println("I implore thee,");

  // Change to font 2
  tft.setTextFont(2);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  // This next line is deliberately made too long for the display width to test
  // automatic text wrapping onto the next line
  tft.println("Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!");
  
  // Test some print formatting functions
  float fnumber = 123.45;
   // Set the font colour to be blue with no background, set to font 4
  tft.setTextColor(TFT_BLUE);    tft.setTextFont(4);
  tft.print("Float = "); tft.println(fnumber);           // Print floating point number
  tft.print("Binary = "); tft.println((int)fnumber, BIN); // Print as integer value in binary
  tft.print("Hexadecimal = "); tft.println((int)fnumber, HEX); // Print as integer number in Hexadecimal
  delay(10000);
  return;
}	//loop
