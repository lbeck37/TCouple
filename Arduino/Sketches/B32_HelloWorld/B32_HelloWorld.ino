#define INCLUDE_ALL true
#if INCLUDE_ALL
  const char szSketchName[]  = "B32_HelloWorld.ino";
  const char szFileDate[]    = "7/30/25C";   //Was 1/5/24G

  #include <Streaming.h>
  #define BLOG          millis()
  //Function protos
  void setup                    (void);
  void loop                     (void);
  void PrintChipInfo            (void);
#endif  //INCLUDE_ALL

#include "Arduino.h"


void setup(void){
  Serial.begin(115200);
  //delay(500);
  //Serial.println("\nBeckE32_HelloWorld.ino: setup(): Hello World! 1/5/24e");
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

#if INCLUDE_ALL
  PrintChipInfo();
#endif
  return;
} //setup


void loop(void){
  //Do nothing.
  return;
} //loop


#if INCLUDE_ALL
  void PrintChipInfo(){
    int wTotalHeapBytes= ESP.getHeapSize();
    int wFreeHeapBytes = ESP.getFreeHeap();
    int wUsedHeapBytes = (wTotalHeapBytes - wFreeHeapBytes);

    int wTotalPSRAMBytes= ESP.getPsramSize();
    int wFreePSRAMBytes = ESP.getFreePsram();
    int wUsedPSRAMBytes = (wTotalPSRAMBytes - wFreePSRAMBytes);

    Serial << BLOG << " PrintChipInfo(): Total Heap  Bytes= " << wTotalHeapBytes << endl;
    Serial << BLOG << " PrintChipInfo(): Free Heap   Bytes= " << wFreeHeapBytes  << endl;
    Serial << BLOG << " PrintChipInfo(): Used Heap   Bytes= " << wUsedHeapBytes  << endl;

    Serial << BLOG << " PrintChipInfo(): Total PSRAM Bytes= " << wTotalPSRAMBytes << endl;
    Serial << BLOG << " PrintChipInfo(): Free PSRAM  Bytes= " << wFreePSRAMBytes  << endl;
    Serial << BLOG << " PrintChipInfo(): Used PSRAM  Bytes= " << wUsedPSRAMBytes  << endl;

    Serial << BLOG << " PrintChipInfo(): Chip Model       = " << ESP.getChipModel()       << endl;
    Serial << BLOG << " PrintChipInfo(): Chip version     = " << ESP.getChipRevision()    << endl;
    Serial << BLOG << " PrintChipInfo(): Number of cores  = " << ESP.getChipCores()       << endl;
    Serial << BLOG << " PrintChipInfo(): Flash Chip Size  = " << ESP.getFlashChipSize()   << endl;
    Serial << BLOG << " PrintChipInfo(): Flash Chip Speed = " << ESP.getFlashChipSpeed()  << endl;
    return;
  } //PrintChipInfo
#endif
//Last line.
