const char szSketchName[]  = "B32_TFTPrintTest.ino";
const char szFileDate[]    = "10/27/23b";

#define DO_ESP_LCD32

/*Test the tft.print() viz embedded tft.write() function, uses font 2, 4, 7
*/
 #ifndef DO_ESP_LCD32
	//Make sure all the display driver and pin comnenctions are correct by
	//editting the User_Setup.h file in the TFT_eSPI library folder.
	//DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #else
	//Make sure all the display driver and pin comnenctions are correct by
	//editing the User_Setup_Select.h file in the TFT_eSPI library folder.
 #endif		//DO_ESP_LCD32

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Streaming.h>

#define TFT_GREY 0x5AEB // New colour

TFT_eSPI tft = TFT_eSPI();  // Invoke library

void setup				(void);
void loop				(void);
void LookForBacklight	(void);

void setup(void) {
	Serial.begin(115200);
	delay(100);
	Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

	Serial << "setup(): Call tft.init()" << endl;
	tft.init();
	Serial << "setup(): Call setRotation(2)" << endl;
	tft.setRotation(2);

	//LookForBacklight();
	Serial << "setup(): Call fillScreen(TFT_GREY)" << endl;
  tft.fillScreen(TFT_GREY);

  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'tft.println'
  //  or stay on the line is there is room for the text with tft.print)
  Serial << "setup(): Call setCursor(0, 0, 2)" << endl;
  tft.setCursor(0, 0, 2);

  // Set the font colour to be white with a black background, set text size multiplier to 1
  Serial << "setup(): Call setTextColor(TFT_WHITE,TFT_BLACK) and setTextSize(1)" << endl;
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);

  // We can now print text on screen using the "print" class
  Serial << "setup(): Call tft.println(Hello World!)" << endl;
  tft.println("Hello World!");
	return;
}//setup


void LookForBacklight(void){
	int wPin;
	while(true){
		wPin= 2;
		pinMode(wPin, OUTPUT);
		Serial << "LookForBackLight(): Set pin " << wPin << " HIGH" << endl;
		digitalWrite(wPin, HIGH);
		delay(1000);
		Serial << "LookForBackLight(): Set pin " << wPin << " LOW" << endl;
		digitalWrite(wPin, LOW);
		delay(1000);

		wPin= 4;
		pinMode(wPin, OUTPUT);
		Serial << "LookForBackLight(): Set pin " << wPin << " HIGH" << endl;
		digitalWrite(wPin, HIGH);
		delay(1000);
		Serial << "LookForBackLight(): Set pin " << wPin << " LOW" << endl;
		digitalWrite(wPin, LOW);
		delay(1000);

		wPin= 5;
		pinMode(wPin, OUTPUT);
		Serial << "LookForBackLight(): Set pin " << wPin << " HIGH" << endl;
		digitalWrite(wPin, HIGH);
		delay(1000);
		Serial << "LookForBackLight(): Set pin " << wPin << " LOW" << endl;
		digitalWrite(wPin, LOW);
		delay(1000);

		for(int wPin= 12; wPin <= 33; wPin++){
		//for(int wPin= 22; wPin <= 30; wPin++){
		//for(int wPin= 27; wPin <= 27; wPin++){
			pinMode(wPin, OUTPUT);
			Serial << "LookForBackLight(): Set pin " << wPin << " HIGH" << endl;
			digitalWrite(wPin, HIGH);
			delay(1000);
			Serial << "LookForBackLight(): Set pin " << wPin << " LOW" << endl;
			digitalWrite(wPin, LOW);
			delay(1000);
		}	//for(int wPin=12;wPin<=33;wPin++)
	}	//while
	return;
}//LookForBacklight


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
  tft.setTextColor(TFT_RED,TFT_BLACK);    tft.setTextFont(4);
  //tft.println(3735928559L, HEX); // Should print DEADBEEF

  // Set the font colour to be green with black background, set to font 4
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextFont(4);
  tft.println("Groop");
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
}//loop
//Last line.



