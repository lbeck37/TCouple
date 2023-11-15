const char szSketchName[]  = "B32_LCDWIKI_DisplayStringESP32.ino";
const char szFileDate[]    = "11/5/23b";
//This program is a demo of displaying string

#include <Streaming.h>
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_SPI.h> //Hardware-specific library

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

const uint16_t  usLCDInfoIndex    =  6;   //Model index into current_lcd_info array for hex model (0x9486), width and height

const int8_t    cCommandDataPin   =  2;
const int8_t    cResetPin         =  4;
const int8_t    cChipSelectPin    = 16;
const int8_t    cSCLKPin          = 18;
const int8_t    cMISOPin          = 19;
const int8_t    cBacklightPin     = 22;
const int8_t    cMOSIPin          = 23;

//Construct with software SPI constructor w/firts param being the index into current_lcd_info[] array to get 0x9486 as model
LCDWIKI_SPI mylcd(usLCDInfoIndex, cChipSelectPin, cCommandDataPin, cMISOPin, cMOSIPin, cResetPin, cSCLKPin, cBacklightPin);

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  mylcd.Init_LCD();
  Serial << "setup(): Call mylcd.Fill_Screen(BLACK)" << endl;
  mylcd.Fill_Screen(BLACK);
  return;
} //setup


void loop() {
  mylcd.Set_Text_Mode(0);

  Serial << "loop(): Display Hello World! in Red on Black" << endl;
  mylcd.Fill_Screen(0x0000);
  mylcd.Set_Text_colour(RED);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_Size(1);
  mylcd.Print_String("Hello World!", 0, 0);
  mylcd.Print_Number_Float(01234.56789, 2, 0, 8, '.', 0, ' ');
  mylcd.Print_Number_Int(0xDEADBEF, 0, 16, 0, ' ',16);

  Serial << "loop(): Display Hello in Green on Black" << endl;
  mylcd.Set_Text_colour(GREEN);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String("Hello", 0, 32);
  mylcd.Print_Number_Float(01234.56789, 2, 0, 48, '.', 0, ' ');
  mylcd.Print_Number_Int(0xDEADBEF, 0, 64, 0, ' ',16);

  Serial << "loop(): Display Hello in Blue on Black" << endl;
  mylcd.Set_Text_colour(BLUE);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Hello", 0, 86);
  mylcd.Print_Number_Float(01234.56789, 2, 0, 110, '.', 0, ' ');
  mylcd.Print_Number_Int(0xDEADBEF, 0, 134, 0, ' ',16);

  delay(3000);
  return;
} //loop
//Last line.
