const char szSketchName[]  = "B32_GFX_Display.ino";
const char szFileDate[]    = "1/27/24E";

//#include <B32_RGBDisplayLib.h>
#include <B32_GFXLib.h>
#include <B32_ESPNowLib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

eBoardPinColor    eReceiverBoardPinColor    {eBoardPinColor::eWhitePin};  //Display does not send anything

RGBScreen             *pScreen;

//Function prototypes for compiler
void  setup           (void);
void  loop            (void);

void setup(void){
  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
/*
  Serial << BLOG << " setup(): Hello World!" << endl;
  Serial << BLOG << " setup(): Display Hello World! in random sizes, colors and positions. (Looks crazy :)" << endl;
*/

  SetupESPNow();

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


//Callback when data is received. Used by Display that receives from TCouple Module
void HandleDataReceived(void) {
  ResetTimer();

  //PrintTemperatures(stIncomingReadings);
  //DisplayData(stIncomingReadings);
  return;
} //OnDataRecv

//Last line.
