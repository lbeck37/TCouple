const char szSketchName[]  = "B32_GFX_HelloWorld.ino";
const char szFileDate[]    = "1/23/24d";

#include <B32_RGBDisplayLib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

RGBScreen             *pScreen;

//Function prototypes for compiler
void  setup           (void);
void  loop            (void);
/*
void  SetupDisplay    (void);
void  FillDisplay     (void);
*/

void setup(void){
  //RGBScreen             *pScreen;
  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  Serial << BLOG << " setup(): Hello World!" << endl;
  Serial << BLOG << " setup(): Display Hello World! in random sizes, colors and positions. (Looks crazy :)" << endl;

  pScreen= new RGBScreen(800, 480);
  pScreen->SetupDisplay();

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void loop(){
/*
  uint16_t  usDisplayWidth   = random(pRGBDisplay->width());
  uint16_t  usDisplayHeight  = random(pRGBDisplay->height());
  uint16_t  usTextColor      = random(0xffff);
  uint16_t  usTextBGColor    = random(0xffff);

  uint8_t   ucTextXscale     = random(6);
  uint8_t   ucTextYscale     = random(6);
  uint8_t   ucPixelMargin    = random(2);

  Serial << BLOG << " loop():"
      " usDisplayWidth= " << usDisplayWidth  << ", usDisplayHeight= " << usDisplayHeight <<
      ", ucTextXscale= "  << ucTextXscale    << ", ucTextYscale= "    << ucTextYscale <<
      ", ucPixelMargin= " << ucPixelMargin   << endl;

  pRGBDisplay->setCursor      (usDisplayWidth, usDisplayHeight);
  pRGBDisplay->setTextColor   (usTextColor   , usTextBGColor);
  pRGBDisplay->setTextSize    (ucTextXscale  , ucTextYscale, ucPixelMargin);

  pRGBDisplay->println        ("Hello World!");
*/
  pScreen->FillDisplay();
  delay(1000); //Milliseconds
  return;
} //loop


/*
void SetupDisplay(void){
  Serial << BLOG << " SetupDisplay(): Call pRGBDisplay->begin()" << endl;
  if (!pRGBDisplay->begin()){
    Serial << BLOG << " SetupDisplay(): pRGBDisplay->begin() failed" << endl;
  }
  else{
    Serial << BLOG << " SetupDisplay(): Clear screen with call to pRGBDisplay->fillScreen(BLACK)" << endl;
    pRGBDisplay->fillScreen(BLACK);
  }

  delay(1000);
  return;
} //SetupDisplay


void FillDisplay(void){
  uint16_t  usDisplayWidth   = random(pRGBDisplay->width());
  uint16_t  usDisplayHeight  = random(pRGBDisplay->height());
  uint16_t  usTextColor      = random(0xffff);
  uint16_t  usTextBGColor    = random(0xffff);

  uint8_t   ucTextXscale     = random(6);
  uint8_t   ucTextYscale     = random(6);
  uint8_t   ucPixelMargin    = random(2);

  Serial << BLOG << " loop():"
      " usDisplayWidth= " << usDisplayWidth  << ", usDisplayHeight= " << usDisplayHeight <<
      ", ucTextXscale= "  << ucTextXscale    << ", ucTextYscale= "    << ucTextYscale <<
      ", ucPixelMargin= " << ucPixelMargin   << endl;

  pRGBDisplay->setCursor      (usDisplayWidth, usDisplayHeight);
  pRGBDisplay->setTextColor   (usTextColor   , usTextBGColor);
  pRGBDisplay->setTextSize    (ucTextXscale  , ucTextYscale, ucPixelMargin);

  pRGBDisplay->println        ("Hello World!");
  return;
} //FillDisplay
*/
//Last line.
