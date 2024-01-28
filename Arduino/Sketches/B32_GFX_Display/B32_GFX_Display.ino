const char szSketchName[]  = "B32_GFX_Display.ino";
const char szFileDate[]    = "1/27/24P";

#include <B32_GFXLib.h>
#include <B32_ESPNowLib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

eBoardPinColor  eReceiverBoardPinColor  {eBoardPinColor::eWhitePin};  //Display sends nothing

RGBScreen       *pScreen;

//Function prototypes for compiler
void  setup           (void);
void  loop            (void);

void setup(void){
  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  SetupESPNow();

  pScreen= new RGBScreen(800, 480);
  pScreen->SetupDisplay();
  pScreen->ShowMyMAC();

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void loop(){
  //Loop doesn't do anything to get data but check the no-data timer, it's all driven by receiving data
  if (millis() > lNextMsec){
    Serial << "loop(): We haven't seen any data for over " <<
              (lAliveMsec/1000) << " seconds" << endl;
    ResetTimer();
    pScreen->DisplayLabels();
    pScreen->DisplayErrorReadings(stErrorReadings);
  } //if (millis()>lNextMsec)
  return;
} //loop


//Callback when data is received. Used by Display that receives from TCouple Module
void HandleDataReceived(void) {
  ResetTimer();

  pScreen->DisplayLabels();
  pScreen->DisplayReadings(stIncomingReadings);
  return;
} //OnDataRecv
//Last line.
