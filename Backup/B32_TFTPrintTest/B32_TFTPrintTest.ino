const char szSketchName[]  = "B32_TFTPrintTest.ino";
const char szFileDate[]    = "11/14/23g";

//Make sure the pin connections for the larger displays are correct by editing:
//  Sketches/libraries/TFT_eSPI/User_Setups/Setup1_ILI9341.h
//    (included from Sketches/libraries/TFT_eSPI/User_Setup_Select.h)

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Streaming.h>

#define TFT_GREY 0x5AEB // New colour

TFT_eSPI Screen= TFT_eSPI();  // Invoke library

void setup				      (void);
void loop				        (void);
/*
void TryNextCSPin       (void);
void SetChipSelectHIGH  (void);
void TurnOnBacklight    (void);
void LookForBacklight	  (void);
*/


void setup(void) {
	Serial.begin(115200);
	delay(100);
	Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
	return;
}//setup


void loop() {
  static int wRotation= 0;
  Serial << "loop(): Call Screen.init()" << endl;
  Screen.init();
  //Screen.setRotation(wRotation++ % 4);    //Roll through the rotations
  Screen.setRotation  (3);                    //USB at lower right
  Screen.fillScreen   (TFT_BLACK);
  
  // Set "cursor" at top left corner of display (0,0) and select font 2
  // (cursor will move to next line automatically during printing with 'Screen.println'
  //  or stay on the line is there is room for the text with Screen.print)
  Screen.setCursor    (0, 0, 2);
  Screen.setTextColor (TFT_WHITE, TFT_BLACK);    //White on black
  Screen.setTextSize  (1);
  Screen << "Sketch: " << szSketchName << ", " << szFileDate << endl;
  
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
  delay(3000);
  return;
}//loop


/*
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
*/
//Last line.
