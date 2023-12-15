//BeckE32_HelloWorld_NEW.ino
#include "Arduino.h"

//Function protos
//void setup    (void);
//void loop     (void);

void setup(void){
  Serial.begin(115200);
  delay(500);
  Serial.println("\nBeckE32_HelloWorld.ino: setup(): Hello World! 12/15/23b");
  return;
} //setup


void loop(void){
  //Do nothing.
  return;
} //loop
//Last line.
