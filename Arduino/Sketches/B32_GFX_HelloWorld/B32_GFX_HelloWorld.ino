const char szSketchName[]  = "B32_GFX_HelloWorld.ino";
const char szFileDate[]    = "1/25/24B";

#include <B32_RGBDisplayLib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

RGBScreen             *pScreen;

//Function prototypes for compiler
void  setup           (void);
void  loop            (void);

void setup(void){
  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  Serial << BLOG << " setup(): Hello World!" << endl;
  Serial << BLOG << " setup(): Display Hello World! in random sizes, colors and positions. (Looks crazy :)" << endl;

  pScreen= new RGBScreen(800, 480);
  pScreen->SetupDisplay();
  pScreen->ShowMyMAC();

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void loop(){
  //pScreen->RandomDisplay();
  pScreen->DisplayLabels();
  pScreen->CreateData();
  pScreen->DisplayData();
  delay(1000); //Milliseconds
  return;
} //loop
//Last line.
