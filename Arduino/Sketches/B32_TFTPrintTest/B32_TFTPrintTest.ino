const char szSketchName[]  = "B32_TFTPrintTest.ino";
const char szFileDate[]    = "11/1/23f";

//#define DO_ESP_LCD32
//Make sure the pin connections are correct by
//editing TFT_eSPI_ESP32.h (included from User_Setup_Select.h)

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Streaming.h>

#define TFT_GREY 0x5AEB // New colour

//extern int   wChipSelectPin;

TFT_eSPI Screen= TFT_eSPI();  // Invoke library

void setup				      (void);
void loop				        (void);
void TryNextCSPin       (void);
void SetChipSelectHIGH  (void);
void TurnOnBacklight    (void);
void LookForBacklight	  (void);

void setup(void) {
	Serial.begin(115200);
	delay(100);
	Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
	//return;   //Testing by skipping everything

  //LookForBacklight();
  TurnOnBacklight();

  //Set which ESP32 pin is chip select
  wChipSelectPin= 15;
  //wChipSelectPin= 5;
  //wChipSelectPin= 13;
	Serial << "setup(): Call Screen.init()" << endl;
	Screen.init();

	Serial << "setup(): Call setRotation(2)" << endl;
	Screen.setRotation(2);

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
	return;
}//setup


void loop() {
  //Return w/o doing anything
  //return;

  //TryNextCSPin();
  Serial << "loop(): Call Screen.init()" << endl;
  Screen.init();

/*
  //Set chip select again for good measure
  Serial << "loop(): Call SetChipSelectHIGH() for good measure" << endl;
  SetChipSelectHIGH();
*/

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
  delay(3000);
  return;
}//loop


void TryNextCSPin(void){
  bool                bPinOK;
  bool                bSkipPin      = false;
  static const int    wFirstPin     =  0;
  static const int    wLastPin      = 33;
  static int          wNextPin      = wFirstPin;
  static int          awPinsToSkip[]= {3, 6, 7, 8, 9, 10, 11,
      //TFT_MISO, TFT_MOSI, TFT_SCLK, TFT_DC, TFT_RST};
        TFT_MISO, TFT_MOSI, TFT_SCLK};

  //Number of elements in an array is sizeof array divided by sizeof an element
  int wNumPinsToSkip= (sizeof(awPinsToSkip) / sizeof(awPinsToSkip[0]));

  //See if wNextPin is in list of pins to skip
  bPinOK= false;
  while (!bPinOK){
    bSkipPin= false;
    Serial << "TryNextCSPin(): See if pin " << wNextPin << " is OK to try" << endl;
    for(int wArrayIndex= 0; (!bSkipPin && (wArrayIndex < wNumPinsToSkip)); wArrayIndex++){
      if (wNextPin == awPinsToSkip[wArrayIndex]) {
        bSkipPin= true;
      } //if(wNextPin== ...
    } //for(intwArrayIndex=0;...

    if (bSkipPin){
      wNextPin++;
      //If past last pin to try, then roll back to first pin
      if (wNextPin > wLastPin){
        wNextPin= wFirstPin;
      } //if(wNextPin>wLastPin)
    } //if(bSkipPin)
    else {
      bPinOK= true;
      Serial << "TryNextCSPin(): Pin " << wNextPin << " is OK to try for chip select" << endl;
      //Set global value, wChipSelectPin, so TFT_CS returns new pin number
      wChipSelectPin= wNextPin;
    } //if(bSkipPin)else
  } //while

  Serial << "TryNextCSPin(): Call SetChipSelectHIGH() " << endl;
  SetChipSelectHIGH();
  return;
} //TryNextCSPin


void SetChipSelectHIGH(void){
  Serial << "SetChipSelectHIGH():  Call pinMode(TFT_CS, OUTPUT) and digitalWrite(TFT_CS, HIGH)" << endl;
  Serial << "SetChipSelectHIGH():  TFT_CS= " << TFT_CS << endl;
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH); // Chip select high (inactive)
  return;
} //SetChipSelectHIGH


void TurnOnBacklight(void){
  pinMode(TFT_BL, OUTPUT);
  if (TFT_BACKLIGHT_ON == HIGH){
    Serial << "TurnOnBacklight(): Set pin " << TFT_BL << " HIGH" << endl;
    digitalWrite(TFT_BL, HIGH);
  }
  else{
    Serial << "TurnOnBacklight(): Set pin " << TFT_BL << " LOW" << endl;
    digitalWrite(TFT_BL, LOW);
  }
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



