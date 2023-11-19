const char szSketchName[]  = "B32_TTGOWeather.ino";
const char szFileDate[]    = "11/18/23z";

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

#define DO_OTA  true
#if DO_OTA
  #include <BeckE32_OTALib.h>
#endif

const double    dPWMFreq          = 5000.0;
const uint8_t   ucPWMResolution   =  8;
const uint8_t   ucPWMLedChannel   =  0;
const uint8_t   ucLeftButtonPin   =  0;
const uint8_t   ucRightButtonPin  = 35;

const uint16_t  usTFT_Grey    = 0x5AEB;
const uint16_t  usLightBlue   = 0x01E9;
const uint16_t  usDarkRed     = 0xA041;
const uint16_t  usBlue        = 0x5D9B;

const char*     szRouterName  = "Aspot24b";
const char*     szRouterPW    = "Qazqaz11";
const char*     szWebHostName = "WeatherStation";

const String szCity           = "Paris";
const String szCountry        = "FR";
/*
const String szEndpoint       = "http://api.openweathermap.org/data/2.5/weather?q=" +
                                 szCity + "," + szCountry + "&units=metric&APPID=";
*/
const String szEndpoint       = "http://api.openweathermap.org/data/3.0/weather?q=" +
                                 szCity + "," + szCountry + "&units=metric&APPID=";

//const String szKey            = "d0d0bf1bb7xxxx2e5dce67c95f4fd0800";
const String szKey            = "82c0b2df6c96557fa90c5f42d705ca0f";

const uint32_t    uwBufSize   = 1000;
char acBuffer[uwBufSize];

String szPayload              = ""; //whole json
String szTemperature          = "" ;
String szHumidity             = "" ;

const int   wDocCapacity      = 1000;
StaticJsonDocument<wDocCapacity>  JsonDoc;

TFT_eSPI    tft               = TFT_eSPI();       // Invoke graphics library

// Define NTP Client to get time
WiFiUDP     ntpUDP;
NTPClient   NTPTimeClient(ntpUDP);

// Variables to save date and time
String      szFormattedDate;
String      szDayStamp;
String      szTimeStamp;

int         awBacklight[5]  = {10, 30, 60, 120, 220};
uint32_t    uwBacklightDuty = 1;

// Set offset time in seconds to adjust for your timezone, for example:
const int   wGMTplus1       =   3600;
const int   wGMTminus1      =  -3600;
const int   wGMTplus8       =  28800;
const int   wGMTminus8      = -28800;

//Set up millisecond wait routine
long        lPrintInterval  = 5000;
long        lReadInterval   = 4000;
long        lNextPrintMsec  = 0;
long        lNextReadMsec   = 0;

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
  tft.setTextColor    (TFT_WHITE, TFT_BLACK);
  tft.setTextSize     (1);

  Serial << "setup(): Sketch: Call ledcSetup()" << endl;
  ledcSetup           (ucPWMLedChannel, dPWMFreq, ucPWMResolution);
  ledcAttachPin       (TFT_BL, ucPWMLedChannel);
  ledcWrite           (ucPWMLedChannel, awBacklight[uwBacklightDuty]);

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
  tft.setTextColor  (TFT_WHITE,usLightBlue);
  tft.setCursor     (4, 152, 2);
  tft.println       ("TEMP:");

  tft.setCursor     (4, 192, 2);
  tft.println       ("HUM: ");
  tft.setTextColor  (TFT_WHITE,TFT_BLACK);

  tft.setFreeFont   (&Orbitron_Medium_20);
  tft.setCursor     (6, 82);
  tft.println       (szCity);

  tft.fillRect      (68, 152, 1, 74, usTFT_Grey);

  for(int i= 0;i < (uwBacklightDuty + 1);i++){
    tft.fillRect((78 + (i*7)), 216, 3, 10, usBlue);
  } //for

// Initialize a NTPClient to get time
  NTPTimeClient.begin();

  NTPTimeClient.setTimeOffset(wGMTminus8);
  GetData();
  delay(500);

#if DO_OTA
  Serial << "setup(): Call SetupWebServer(" << szWebHostName << ")" << endl;
  SetupWebserver(szWebHostName);
#endif
  return;
}   //setup


void loop(void){
  static int     wFrame          = 0;
  String         szPreviousTime  = "";
  static bool    bInvertDisplay  = true;
  static bool    bPress1         = false;
  static bool    bPress2         = false;
  static String  szCurrentSecs   = "";

   tft.pushImage  (0, 88,  135, 65, ausAnimation[wFrame]);

   //Cycle through the 10 frames of animation
   wFrame= ((wFrame + 1) % 10);

   //Handle TTGO button on the right of the USB connector
   //Cycles through 5 levels of the LED backlight to the LCD screen
   if(digitalRead(ucRightButtonPin) == 0){
     if(!bPress2){
       bPress2= true;
       tft.fillRect   (78,216,44,12,TFT_BLACK);

       uwBacklightDuty++;
       if(uwBacklightDuty >= 5){
         uwBacklightDuty= 0;
       }  //if(uwBacklightDuty>=5)

       //wFrame cycles from 0 to 9, why?
       wFrame= ((wFrame + 1) % 10);

       //Display number of bars to indicate brightness
       for(int wLevel= 0;wLevel < (uwBacklightDuty + 1);wLevel++){
         tft.fillRect (78 + (wLevel * 7), 216, 3, 10, usBlue);
       }  //for

       //uwBacklightDuty ranges from 10 to 220
       ledcWrite(ucPWMLedChannel, awBacklight[uwBacklightDuty]);
     }  //if(!bPress2)
   }  //if(digitalRead(ucRightButtonPin)==0)
   else{
     bPress2= false;
   }  //if(digitalRead(ucRightButtonPin)==0)else

  //Handle TTGO button on the left of the USB connector
  //Left button toggles between black background and inverted
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

  if (millis() > lNextReadMsec){
     //Serial << "loop(): Skip call to GetData() for debugging stutter" << endl;
     Serial << "loop(): Call GetData()" << endl;
     GetData();
     lNextReadMsec= (millis() + lReadInterval);
  }  //if(millis()>lNextReadMsec)

  tft.setFreeFont   (&Orbitron_Medium_20);
  tft.setCursor     (2, 187);
  tft.println       (szTemperature.substring(0, 3));

  tft.setCursor     (2, 227);
  tft.println       (szHumidity + "%");

  tft.setTextColor  (TFT_ORANGE, TFT_BLACK);
  tft.setTextFont   (2);
  tft.setCursor     (6, 44);
  tft.println       (szDayStamp);
  tft.setTextColor  (TFT_WHITE, TFT_BLACK);

  while(!NTPTimeClient.update()) {
    NTPTimeClient.forceUpdate();
  }   //while

  // The szFormattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  szFormattedDate = NTPTimeClient.getFormattedDate();
  if (millis() > lNextPrintMsec){
    //Serial.println(formattedDate);
    Serial << "loop(): Formatted Date= " << szFormattedDate << endl;
    lNextPrintMsec= (millis() + lPrintInterval);
  }

  int splitT  = szFormattedDate.indexOf("T");
  szDayStamp  = szFormattedDate.substring(0, splitT);

  szTimeStamp = szFormattedDate.substring((splitT + 1), (szFormattedDate.length() - 1));

  if(szCurrentSecs != szTimeStamp.substring(6, 8)){
    tft.fillRect    (78, 170, 48, 28, usDarkRed);
    tft.setFreeFont (&Orbitron_Light_24);
    tft.setCursor   (81, 192);
    tft.println     (szTimeStamp.substring(6, 8));
    szCurrentSecs=   szTimeStamp.substring(6, 8);
  }   //if(szCurrentSecs!=szTimeStamp.substring(6,8))

  tft.setFreeFont   (&Orbitron_Light_32);
  String    szNewTime= szTimeStamp.substring(0, 5);

  if(szNewTime != szPreviousTime){
    tft.fillRect    (3, 8, 120, 30, TFT_BLACK);
    tft.setCursor   (5, 34);
    tft.println     (szTimeStamp.substring(0, 5));
    szPreviousTime= szTimeStamp.substring(0, 5);
  }  //if(szNewTime!=szPreviousTime)

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
    Serial << "GetData(): Call http.begin()" << endl;
    http.begin(szEndpoint + szKey); //Specify the URL
    Serial << "GetData(): Call http.GET()" << endl;
    int httpCode= http.GET();  //Make the request
    Serial << "GetData(): After the call to http.GET()" << endl;

    if (httpCode > 0){ //Check for the returning code
      Serial << "GetData(): Call http.getString()" << endl;
      szPayload= http.getString();
      Serial << "GetData(): After call to http.getString()" << endl;
      // Serial.println(httpCode);
      Serial.println(szPayload);
    } //if(httpCode>0)
    else {
      Serial.println("Error on HTTP request");
    } //if(httpCode>0)else

    Serial << "GetData(): Call http.end()" << endl;
    http.end(); //Free the resources
  } //if((WiFi.status()==WL_CONNECTED))

  szPayload.toCharArray(acBuffer, uwBufSize);
  Serial << "GetData(): Call deserializeJson()" << endl;
  deserializeJson(JsonDoc, acBuffer);

  String szNewTemperature = JsonDoc["main"]["temp"];
  String szNewHumidity    = JsonDoc["main"]["humidity"];
  szTemperature = szNewTemperature;
  szHumidity    = szNewHumidity;

  Serial.println("Temperature" + String(szTemperature));
  Serial.println("Humidity" + szHumidity);
  Serial.println(szCity);
  return;
 }  //GetData
/*Receiving this message:
  {"cod":401, "message": "Invalid API key.
  Please see https://openweathermap.org/faq#error401 for more info."}
  I think this is my new key "82c0b2df6c96557fa90c5f42d705ca0f", 11/18/23
*/
 //Last line.
