const char szSketchName[]  = "B32_TFTPrintTest.ino";
const char szFileDate[]    = "10/29/23e";

//#define DO_ESP_LCD32
//Make sure the pin connections are correct by
//editing TFT_eSPI_ESP32.h (included from User_Setup_Select.h)

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Streaming.h>

#define TFT_GREY 0x5AEB // New colour

extern int   wChipSelectPin;

TFT_eSPI Screen= TFT_eSPI();  // Invoke library

void setup				      (void);
void loop				        (void);
void TurnOnBacklight    (void);
void LookForBacklight	  (void);

void setup(void) {
	Serial.begin(115200);
	delay(100);
	Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  TurnOnBacklight();
/*
	Serial << "setup(): Call Screen.init()" << endl;
	wChipSelectPin= 15;
	Screen.init();
	Serial << "setup(): Call setRotation(2)" << endl;
	Screen.setRotation(2);

	//LookForBacklight();
	Serial << "setup(): Call fillScreen(TFT_GREY)" << endl;
  Screen.fillScreen(TFT_GREY);

  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'Screen.println'
  //  or stay on the line is there is room for the text with Screen.print)
  Serial << "setup(): Call setCursor(0, 0, 2)" << endl;
  Screen.setCursor(0, 0, 2);

  // Set the font colour to be white with a black background, set text size multiplier to 1
  Serial << "setup(): Call setTextColor(TFT_WHITE,TFT_BLACK) and setTextSize(1)" << endl;
  Screen.setTextColor(TFT_WHITE,TFT_BLACK);
  Screen.setTextSize(1);

  // We can now print text on screen using the "print" class
  Serial << "setup(): Call Screen.println(Hello World!)" << endl;
  Screen.println("Hello World!");
*/
	return;
}//setup


void loop() {
/*
  static int wLoopCount= 0;
  //Set Chip Select pin for this time thru loop()
  if (((++wLoopCount) % 2) == 1){
    wChipSelectPin= 5;
  }
  else{
    wChipSelectPin= 15;
  }
  Serial << "loop(): wChipSelectPin set equal to " << wChipSelectPin << endl;
*/
  Serial << "loop(): Call Screen.init()" << endl;
  Screen.init();

  //Set chip select so we are sure
  Serial << "loop():  Call pinMode(TFT_CS, OUTPUT) and digitalWrite(TFT_CS, HIGH)" << endl;
  Serial << "loop():  TFT_CS= " << TFT_CS << endl;
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH); // Chip select high (inactive)

  // Fill screen with grey so we can see the effect of printing with and without 
  // a background colour defined
  Screen.fillScreen(TFT_GREY);
  
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'Screen.println'
  //  or stay on the line is there is room for the text with Screen.print)
  Screen.setCursor(0, 0, 2);
  // Set the font colour to be white with a black background, set text size multiplier to 1
  Screen.setTextColor(TFT_WHITE,TFT_BLACK);  Screen.setTextSize(1);
  // We can now plot text on screen using the "print" class
  Screen.println("Hello World!");
  
  // Set the font colour to be yellow with no background, set to font 7
  Screen.setTextColor(TFT_YELLOW); Screen.setTextFont(7);
  Screen.println(1234.56);
  
  // Set the font colour to be red with black background, set to font 4
  Screen.setTextColor(TFT_RED,TFT_BLACK);    Screen.setTextFont(4);
  //Screen.println(3735928559L, HEX); // Should print DEADBEEF

  // Set the font colour to be green with black background, set to font 4
  Screen.setTextColor(TFT_GREEN,TFT_BLACK);
  Screen.setTextFont(4);
  Screen.println("Groop");
  Screen.println("I implore thee,");

  // Change to font 2
  Screen.setTextFont(2);
  Screen.println("my foonting turlingdromes.");
  Screen.println("And hooptiously drangle me");
  Screen.println("with crinkly bindlewurdles,");
  // This next line is deliberately made too long for the display width to test
  // automatic text wrapping onto the next line
  Screen.println("Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!");
  
  // Test some print formatting functions
  float fnumber = 123.45;
   // Set the font colour to be blue with no background, set to font 4
  Screen.setTextColor(TFT_BLUE);    Screen.setTextFont(4);
  Screen.print("Float = "); Screen.println(fnumber);           // Print floating point number
  Screen.print("Binary = "); Screen.println((int)fnumber, BIN); // Print as integer value in binary
  Screen.print("Hexadecimal = "); Screen.println((int)fnumber, HEX); // Print as integer number in Hexadecimal
  delay(10000);
  return;
}//loop


void TurnOnBacklight(void){
  pinMode(TFT_BL, OUTPUT);
  Serial << "TurnOnBacklight(): Set pin " << TFT_BL << " HIGH" << endl;
  digitalWrite(TFT_BL, HIGH);
  return;
} //TurnOnBacklight


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
//Last line.



