const char szSketchName[]  = "B32_GFX_HelloWorld.ino";
const char szFileDate[]    = "1/21/24e";

#include <B32_RGBDisplayLib.h>
#include <Streaming.h>

#define BLOG          millis()    //Ued in logging

//Function protos
void  setup           (void);
void  loop            (void);
void  SetupDisplay    (void);

void setup(void){
  Serial.begin(115200);
  Serial << BLOG << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  Serial << BLOG << " setup(): Hello World!" << endl;
  Serial << BLOG << " setup(): Display Hello World! in random sizes, colors and positions. (Looks crazy :)" << endl;

  SetupDisplay();

  return;
} //setup


void loop(){
  pRGBDisplay->setCursor      (random(pRGBDisplay->width()), random(pRGBDisplay->height()));
  pRGBDisplay->setTextColor   (random(0xffff), random(0xffff));
  pRGBDisplay->setTextSize    (random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);

  pRGBDisplay->println("Hello World!");

  delay(1000); //Milliseconds
  return;
} //loop


void SetupDisplay(void){
#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!pRGBDisplay->begin()){
    Serial.println("pRGBDisplay->begin() failed!");
  }
  pRGBDisplay->fillScreen(BLACK);

#ifdef GFX_BL   //Turn on LCD backlight
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif
  return;
} //SetupDisplay
//Last line.
