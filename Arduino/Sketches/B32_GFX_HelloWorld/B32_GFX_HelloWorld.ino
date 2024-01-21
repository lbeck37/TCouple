const char szSketchName[]  = "B32_GFX_HelloWorld.ino";
const char szFileDate[]    = "1/20/24k";

#include <B32_RGBDisplayLib.h>
#include <Streaming.h>

#define BLOG          millis()

void setup(void){
  Serial.begin(115200);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  if (!pRGBDisplay->begin()){
    Serial.println("pRGBDisplay->begin() failed!");
  }
  pRGBDisplay->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  pRGBDisplay->setCursor(10, 10);
  pRGBDisplay->setTextColor(RED);
  pRGBDisplay->println("Hello World!");

  delay(1000);
  return;
} //setup


void loop(){
  pRGBDisplay->setCursor      (random(pRGBDisplay->width()), random(pRGBDisplay->height()));
  pRGBDisplay->setTextColor   (random(0xffff), random(0xffff));
  pRGBDisplay->setTextSize    (random(6) /* x scale */, random(6) /* y scale */, random(2) /* pixel_margin */);

  pRGBDisplay->println("Hello Dude!");

  //delay(1000); // 1 second
  return;
} //loop
//Last line.
