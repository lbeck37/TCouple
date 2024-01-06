#define INCLUDE_ALL true
#if INCLUDE_ALL
  const char szSketchName[]  = "B32_HelloWorld.ino";
  const char szFileDate[]    = "1/4/24E";

  #include <Streaming.h>
  #define BLOG          millis()
  //Function protos
  void setup                    (void);
  void loop                     (void);
  void PrintRAMSizes            (void);
#endif

#include "Arduino.h"


void setup(void){
  Serial.begin(115200);
  //delay(500);
  Serial.println("\nBeckE32_HelloWorld.ino: setup(): Hello World! 1/5/24e");

#if INCLUDE_ALL
  PrintRAMSizes();
#endif
  return;
} //setup


void loop(void){
  //Do nothing.
  return;
} //loop


#if INCLUDE_ALL
  void PrintRAMSizes(){
    int wTotalHeapBytes= ESP.getHeapSize();
    int wFreeHeapBytes = ESP.getFreeHeap();
    int wUsedHeapBytes = (wTotalHeapBytes - wFreeHeapBytes);

    int wTotalPSRAMBytes= ESP.getPsramSize();
    int wFreePSRAMBytes = ESP.getFreePsram();
    int wUsedPSRAMBytes = (wTotalPSRAMBytes - wFreePSRAMBytes);

    Serial << BLOG << " PrintRAMSizes(): Total Heap  Bytes= " << wTotalHeapBytes << endl;
    Serial << BLOG << " PrintRAMSizes(): Free Heap   Bytes= " << wFreeHeapBytes  << endl;
    Serial << BLOG << " PrintRAMSizes(): Used Heap   Bytes= " << wUsedHeapBytes  << endl;

    Serial << BLOG << " PrintRAMSizes(): Total PSRAM Bytes= " << wTotalPSRAMBytes << endl;
    Serial << BLOG << " PrintRAMSizes(): Free PSRAM  Bytes= " << wFreePSRAMBytes  << endl;
    Serial << BLOG << " PrintRAMSizes(): Used PSRAM  Bytes= " << wUsedPSRAMBytes  << endl;
    return;
  } //PrintRAMSizes
#endif
//Last line.
