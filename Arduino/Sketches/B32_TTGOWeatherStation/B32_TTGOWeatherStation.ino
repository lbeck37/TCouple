const char szSketchName[]  = "B32_TTGOWeather.ino";
const char szFileDate[]    = "11/18/23k";

#include "Animation.h"
#include <SPI.h>
#include <TFT_eSPI.h>           // Hardware-specific library
#include <ArduinoJson.h>        //https://github.com/bblanchon/ArduinoJson.git
#include <NTPClient.h>          //https://github.com/taranais/NTPClient
#include "Orbitron_Medium_20.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <Streaming.h>

#define DO_OTA      true
#if DO_OTA
  #include <BeckE32_OTALib.h>
#endif

#define TFT_GREY    0x5AEB
#define lightblue   0x01E9
#define darkred     0xA041
#define blue        0x5D9B

const double    dPWMFreq            = 5000.0;
const uint8_t   ucPWMResolution     =  8;
const uint8_t   ucPWMLedChannelTFT  =  0;
const uint8_t   ucLeftButtonPin     =  0;
const uint8_t   ucRightButtonPin    = 35;

const char*     szRouterName        = "Aspot24b";
const char*     szRouterPW          = "Qazqaz11";
const char*     szWebHostName       = "WeatherStation";

const String town             = "Paris";
const String Country          = "FR";
const String endpoint         = "http://api.openweathermap.org/data/2.5/weather?q=" +
                                 town + "," + Country + "&units=metric&APPID=";

const String key              = "d0d0bf1bb7xxxx2e5dce67c95f4fd0800";  //EDDITT

const uint32_t    uwBufSize     = 1000;
char acBuffer[uwBufSize];

String payload                = ""; //whole json
String tmp                    = "" ; //temperatur
String hum                    = "" ; //humidity

StaticJsonDocument<1000>  doc;

TFT_eSPI    tft             = TFT_eSPI();       // Invoke graphics library

// Define NTP Client to get time
WiFiUDP     ntpUDP;
NTPClient   timeClient(ntpUDP);

// Variables to save date and time
String      formattedDate;
String      dayStamp;
String      timeStamp;

int         awBacklight[5]  = {10, 30, 60, 120, 220};
uint32_t    uwBacklightDuty = 1;

// Set offset time in seconds to adjust for your timezone, for example:
const int   wGMTplus1       =   3600;
const int   wGMTminus1      =  -3600;
const int   wGMTplus8       =  28800;
const int   wGMTminus8      = -28800;

//Set up millisecond wait routine
long        lPrintInterval  = 5000;
long        lCurrentMsec    = 0;
long        lNextMsec       = 0;

//Func protos
void setup      (void);
void loop       (void);
void GetData    (void);


void setup(void) {
  Serial.begin(115200);
  delay(500);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  pinMode(0, INPUT_PULLUP);
  pinMode(ucRightButtonPin, INPUT);
  Serial << "setup(): Sketch: Call tft.init()" << endl;
  tft.init            ();
  tft.setRotation     (0);
  tft.fillScreen      (TFT_BLACK);
  tft.setTextColor    (TFT_WHITE,TFT_BLACK);
  tft.setTextSize     (1);

  Serial << "setup(): Sketch: Call ledcSetup()" << endl;
  ledcSetup           (ucPWMLedChannelTFT, dPWMFreq, ucPWMResolution);
  ledcAttachPin       (TFT_BL, ucPWMLedChannelTFT);
  ledcWrite           (ucPWMLedChannelTFT, awBacklight[uwBacklightDuty]);

  tft.print           ("Connecting to ");    //This goes to the display
  tft.println         (szRouterName);

  Serial << "setup(): Sketch: Call WiFi.begin(" << szRouterName << ", "
         << szRouterPW << ")" << endl;
  WiFi.begin(szRouterName, szRouterPW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    tft.print(".");
  } //while
  Serial << endl << "setup(): Connected to " << szRouterName
         << ", IP address to connect to is " << WiFi.localIP() << endl;

  tft.println       ("");
  tft.println       ("WiFi connected.");
  tft.println       ("IP address: ");
  tft.println       (WiFi.localIP());
  delay(3000);

  tft.setTextColor  (TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);
  tft.fillScreen    (TFT_BLACK);
  tft.setSwapBytes  (true);
  tft.setCursor     (2, 232, 1);
  tft.println       (WiFi.localIP());
  tft.setCursor     (80, 204, 1);
  tft.println       ("BRIGHT:");

  tft.setCursor     (80, 152, 2);
  tft.println       ("SEC:");
  tft.setTextColor  (TFT_WHITE,lightblue);
  tft.setCursor     (4, 152, 2);
  tft.println       ("TEMP:");

  tft.setCursor     (4, 192, 2);
  tft.println       ("HUM: ");
  tft.setTextColor  (TFT_WHITE,TFT_BLACK);

  tft.setFreeFont   (&Orbitron_Medium_20);
  tft.setCursor     (6, 82);
  tft.println       (town);

  tft.fillRect      (68, 152, 1, 74, TFT_GREY);

  for(int i= 0;i < (uwBacklightDuty + 1);i++){
    tft.fillRect((78 + (i*7)), 216, 3, 10, blue);
  } //for

// Initialize a NTPClient to get time
  timeClient.begin();

  timeClient.setTimeOffset(wGMTplus1);
  GetData();
  delay(500);

#if DO_OTA
  Serial << "setup(): Call SetupWebServer(" << szWebHostName << ")" << endl;
  SetupWebserver(szWebHostName);
#endif
  return;
}   //setup


/*
int     i           = 0;
String  tt          = "";
int     count       = 0;
bool    inv         = 1;
int     press1      = 0;
int     press2      = 0;
*/

//String  curSeconds  = "";

void loop(void){
   static int     wFrame          = 0;
   int            i               = 0;
   String         tt              = "";
   int            count           = 0;
   static bool    bInvertDisplay  = true;
   static int     wPress1         = 0;
   //bool    bPress1                = false;
   static int     wPress2         = 0;
   static String  szCurrentSecs   = "";

   tft.pushImage  (0, 88,  135, 65, ausAnimation[wFrame]);

   //Cycle through the 10 frames of animation
   wFrame= ((wFrame + 1) % 10);

   //Handle TTGO button on the right of the USB connector
   //Cycles through 5 levels of the LED backlight to the LCD screen
   if(digitalRead(ucRightButtonPin) == 0){
     if(wPress2 == 0){
       wPress2= 1;
       tft.fillRect   (78,216,44,12,TFT_BLACK);

       uwBacklightDuty++;
       if(uwBacklightDuty >= 5){
         uwBacklightDuty= 0;
       }  //if(uwBacklightDuty>=5)
       wFrame= ((wFrame + 1) % 10);

       for(int i= 0;i < (uwBacklightDuty + 1);i++){
         tft.fillRect (78 + (i*7), 216, 3, 10, blue);
       }  //for

       //uwBacklightDuty ranges from 10 to 220
       ledcWrite(ucPWMLedChannelTFT, awBacklight[uwBacklightDuty]);
     }  //if(wPress2==0)
   }  //if(digitalRead(ucRightButtonPin)==0)
   else{
     wPress2= 0;
   }  //if(digitalRead(ucRightButtonPin)==0)else

   //Handle TTGO button on the left of the USB connector
   //Left button toggles between black background and inverted
   if(digitalRead(ucLeftButtonPin) == 0){
     if(wPress1 == 0){
       wPress1 = 1;
       bInvertDisplay= !bInvertDisplay;
       tft.invertDisplay(bInvertDisplay);
     }  //if(wPress1==0)
   }  //if(digitalRead(ucLeftButtonPin)==0)
   else{
     wPress1= 0;
   }  //if(digitalRead(ucLeftButtonPin)==0)else
/*
   if(digitalRead(ucLeftButtonPin) == 0){
     if(!bPress1){
       bPress1= true;
       bInvertDisplay= !bInvertDisplay;
       tft.invertDisplay(bInvertDisplay);
     }  //if(!bPress1)
   }  //if(digitalRead(ucLeftButtonPin)==0)
   else{
     bPress1= false;
   }  //if(digitalRead(ucLeftButtonPin)==0)else
*/

   if(count == 0){
     Serial << "loop(): Skip call to GetData() for debugging stutter" << endl;
     //GetData();
   }  //if(count==0)

   count++;
   if(count > 2000){
     count= 0;
   }  //if(count>2000)

  tft.setFreeFont   (&Orbitron_Medium_20);
  tft.setCursor     (2, 187);
  tft.println       (tmp.substring(0, 3));

  tft.setCursor     (2, 227);
  tft.println       (hum + "%");

  tft.setTextColor  (TFT_ORANGE, TFT_BLACK);
  tft.setTextFont   (2);
  tft.setCursor     (6, 44);
  tft.println       (dayStamp);
  tft.setTextColor  (TFT_WHITE, TFT_BLACK);

  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }   //while

  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  if (millis() > lNextMsec){
    //Serial.println(formattedDate);
    Serial << "loop(): Formatted Date= " << formattedDate << endl;
    lNextMsec= (millis() + lPrintInterval);
  }

  int splitT  = formattedDate.indexOf("T");
  dayStamp    = formattedDate.substring(0, splitT);

  timeStamp   = formattedDate.substring(splitT+1, formattedDate.length()-1);

  if(szCurrentSecs != timeStamp.substring(6, 8)){
    tft.fillRect    (78,170,48,28,darkred);
    tft.setFreeFont (&Orbitron_Light_24);
    tft.setCursor   (81, 192);
    tft.println     (timeStamp.substring(6,8));
    szCurrentSecs= timeStamp.substring(6,8);
  }   //if(szCurrentSecs!=timeStamp.substring(6,8))

  tft.setFreeFont   (&Orbitron_Light_32);
  String current= timeStamp.substring(0,5);

  if(current != tt){
    tft.fillRect    (3,8,120,30,TFT_BLACK);
    tft.setCursor   (5, 34);
    tft.println     (timeStamp.substring(0,5));
    tt= timeStamp.substring(0,5);
  }  //if(current!=tt)

  delay(80);
#if DO_OTA
  HandleOTAWebserver();
#endif
  return;
}   //loop


void GetData(){
  tft.fillRect    (1, 170, 64,20, TFT_BLACK);
  tft.fillRect    (1, 210, 64,20, TFT_BLACK);
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    HTTPClient    http;
    http.begin(endpoint + key); //Specify the URL
    int httpCode= http.GET();  //Make the request

    if (httpCode > 0){ //Check for the returning code
         payload= http.getString();
       // Serial.println(httpCode);
        Serial.println(payload);
      } //if(httpCode>0)
    else {
      Serial.println("Error on HTTP request");
    } //if(httpCode>0)else

    http.end(); //Free the resources
  } //if((WiFi.status()==WL_CONNECTED))

/*
  char inp[1000];
  payload.toCharArray(inp, 1000);
  deserializeJson(doc, inp);
*/
  payload.toCharArray(acBuffer, uwBufSize);
  deserializeJson(doc, acBuffer);

  String tmp2  = doc["main"]["temp"];
  String hum2  = doc["main"]["humidity"];
  String town2 = doc["name"];
  tmp= tmp2;
  hum= hum2;

  Serial.println("Temperature" + String(tmp));
  Serial.println("Humidity" + hum);
  Serial.println(town);
  return;
 }  //GetData
 //Last line.
