const char szSketchName[]  = "B32_FirstExamples.ino";
const char szFileDate[]    = "12/3/23c";
#include <Streaming.h>

void setup() {
  Serial.begin(115200);
  delay(100);   //Give Serial a chance to stabilize

  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  Serial << "setup(): Hello World! (from Larry Beck)"<< endl;

  return;
} //setup

void loop() {
  //We're currently not doing anything when loop() is called
  return;
} //loop

/*
  If you create a new sketch in c:\_Repos\TCouple\Arduino\Sketches\KCB_FirstExamples\KCB_FirstExamples.ino, then
  you could then check it in to TCouple repository. I installed "Git for Windows" from https://git-scm.com/download/win 
  and I was then able to open a terminal window , cd to c:\_Repos\TCouple\, and type "git gui" to bring up the Git Gui window
  where I type in a "Commit Message" describing what is being Comitted to my local repository and Pushed up to github.
  It may be easier, especially for the future, to use your own github account and create your own repository. This can be done later

  Put the text between the quotes in File>Preferences>Additional boards manager URL's:
    "https://arduino.esp8266.com/stable/package_esp8266com_index.json,https://dl.espressif.com/dl/package_esp32_index.json"

  Use ESP32 Dev Module for the TTGO boards

  In Preferences turn on "All" Compiler warnings and Show verbose output during both Compile and Upload
  I also had Show files inside Sketches and Auto save both checked.

  In File>Preferences>Sketchbook put the location of your sketches. It will look for libraries in the library
  directory in that directory. My location is currently: "c:\_Repos\TCouple\Arduino\Sketches" with my "personal" libraries
  in "c:\_Repos\TCouple\Arduino\Sketches\libraries". This is where it finds the Streaming.h library mentioned in
  the "#include <Streaming.h>" line

  Even though I selected the ESP32 Dev Module in the window in AIDE2, I found I still had to go into 
    Tools>Board: ESP32 Dev Module>esp32 and select the ESP32 Dev Module there, too.
*/
//Last line