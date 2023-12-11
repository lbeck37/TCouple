const char szSketchName[]  = "B32_TTGOWeatherStation.ino";
const char szFileDate[]    = "12/11/23e";

#define DO_OTA            true
#define DO_OPENWEATHER    true
#define DO_TASKING        true 

#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Time.h>     //Just using this library for unix time conversion

#include <TFT_eSPI.h>           // Hardware-specific library
#include <NTPClient.h>          //https://github.com/taranais/NTPClient
#include <Streaming.h>

#include "Animation10x.h"
#include "Orbitron_Medium_20.h"

#if false && DO_TASKING
  #include <FreeRTOS.h>
  #include <Arduino_FreeRTOS.h>
  #include "freertos/FreeRTOS.h"
#endif  //DO_TASKING

#if DO_OTA
  #include "BeckE32_OTALib.h"
#endif

#if DO_OPENWEATHER
  #include <OpenWeather.h>
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

const String szCity           = "SLO";
const String szCountry        = "US";

float         fTemperature    = 0.00;
uint8_t       ucHumidity      = 0;

const String  szAPIKey        = "82c0b2df6c96557fa90c5f42d705ca0f";

//Set both longitude and latitude to at least 4 decimal places
//  90.0000 to  -90.0000 negative for Southern hemisphere/*
// 180.0000 to -180.0000 negative for West
const String  szBoiseLatitude   =   "43.6150";
const String  szBoiseLongitude  = "-116.2023";

const String  szSLOLatitude     =   "35.2828";
const String  szSLOLongitude    = "-120.6596";

const int     wSecPerHour       = 3600;

const int     wBoiseTimeZone    = -7;
const int     wSLOTimeZone      = -8;
const time_t  ulBoiseSecOffset  = (wBoiseTimeZone * wSecPerHour);
const time_t  ulSLOSecOffset    = (wSLOTimeZone   * wSecPerHour);

//time_t        ulTimeZoneOffset;
uint32_t      uwTimeZoneOffset;

const int     wReadWeatherTaskPeriodSec = 3;

String        szLatitude;
String        szLongitude;
String        szUnits          = "imperial";  //"metric" or "imperial"
String        szLanguage       = "en";

OW_Weather    OpenWeather;                    //Weather pForecast library instance

TFT_eSPI      tft              = TFT_eSPI();  // Invoke graphics library

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
void    setup               (void);
void    loop                (void);
void    HandleRightButton   (void);
void    HandleLeftButton    (void);
void    ReadOpenWeather     (void);
void    ReadWeatherTask     (void *pvParameter);
void    DisplayTempHumidity (void);
void    DisplayTimeStamp    (void);
String  strTime             (time_t unixTime);


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

  tft.print         ("setup(): Sketch: ");
  tft.println       (szSketchName);
  tft.println       (szFileDate);
  delay(3000);    //Delay long enough to read screen

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

//Initialize a NTPClient to get time
  NTPTimeClient.begin();
  NTPTimeClient.setTimeOffset(wGMTminus8);

#if DO_OTA
  Serial << "setup(): Call SetupWebServer(" << szWebHostName << ") for OTA updates" << endl;
  SetupWebserver(szWebHostName);
#endif

#if DO_TASKING
	//xTaskCreate(&ReadWeatherTask, "ReadWeather Task", 2048, NULL, 5, NULL);
	xTaskCreate(&ReadWeatherTask, "ReadWeather Task", (4 * 2048), NULL, 5, NULL);
#endif
  return;
}   //setup


void loop(void){
  static int     wFrame          = 0;

  //Display the next frame of 10 of animation
  wFrame= ((wFrame + 1) % 10);
  tft.pushImage  (0, 88,  135, 65, ausAnimation[wFrame]);

  HandleRightButton();
  HandleLeftButton();
  
  #if !DO_TASKING
    ReadOpenWeather();
  #endif
  DisplayTempHumidity();
  DisplayTimeStamp();

#if DO_OTA
  HandleOTAWebserver();
#endif
  //delay(1000);
  return;
}   //loop


void HandleRightButton(void){
  //Handle TTGO button on the right of the USB connector
  //Cycles through 5 levels of the LED backlight to the LCD screen
  static bool    bPress2         = false;
  //static int     wFrame          = 0;

  if(digitalRead(ucRightButtonPin) == 0){
    if(!bPress2){
      bPress2= true;
      tft.fillRect   (78,216,44,12,TFT_BLACK);

      uwBacklightDuty++;
      if(uwBacklightDuty >= 5){
        uwBacklightDuty= 0;
      }  //if(uwBacklightDuty>=5)

      //wFrame cycles from 0 to 9, why?
      //wFrame= ((wFrame + 1) % 10);

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
  return;
} //HandleRightButton


void HandleLeftButton(void){
  //Handle TTGO button on the left of the USB connector
  //Left button toggles between black background and inverted
  static bool    bPress1          = false;
  static bool    bInvertDisplay   = true;

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
  return;
} //HandleLeftButton


void DisplayTempHumidity(void){
  tft.fillRect    (1, 170, 64,20, TFT_BLACK);   //Was in ReadOpenWeather()
  tft.fillRect    (1, 210, 64,20, TFT_BLACK);

  tft.setFreeFont   (&Orbitron_Medium_20);
  tft.setCursor     (2, 187);
  //tft.println       (szTemperature.substring(0, 3));
  tft.println       (fTemperature);

  tft.setCursor     (2, 227);
  tft.print         (ucHumidity);
  tft.println       ("%");

  tft.setTextColor  (TFT_ORANGE, TFT_BLACK);
  tft.setTextFont   (2);
  tft.setCursor     (6, 44);
  tft.println       (szDayStamp);
  tft.setTextColor  (TFT_WHITE, TFT_BLACK);
  return;
} //DisplayTempHumidity


void DisplayTimeStamp(void){
  String         szPreviousTime  = "";
  static String  szCurrentSecs   = "";
  while(!NTPTimeClient.update()) {
    NTPTimeClient.forceUpdate();
  }   //while

  // The szFormattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  szFormattedDate = NTPTimeClient.getFormattedDate();
  if (millis() > lNextPrintMsec){
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
  return;
} //DisplayTimeStamp


void ReadOpenWeather(void){
  if (millis() < lNextReadMsec){
    return;
  }
  else{
    //Set next time to execute this routine.
    //After setting next time, run this routine.
    lNextReadMsec= (millis() + lReadInterval);
  }  //if(millis()>lNextReadMsec)

  Serial << endl << "ReadOpenWeather(): Begin" << endl;
  tft.fillRect    (1, 170, 64,20, TFT_BLACK);
  tft.fillRect    (1, 210, 64,20, TFT_BLACK);
  if ((WiFi.status() != WL_CONNECTED)) { //Check the current connection status
    //Return without doing anything.
    Serial << "ReadOpenWeather(): ERROR: WiFi is not connected" << endl;
  } //if((WiFi.status()==WL_CONNECTED))

  //Create the structures that hold the retrieved weather
  //OW_current, OW_hourly, OW_daily and OW_forecast are structs defined in Data_Point_Set.h
  OW_current    *pCurrent     = new OW_current;
  OW_hourly     *pHourly      = new OW_hourly;
  OW_daily      *pDaily       = new OW_daily;

  uwTimeZoneOffset= ulSLOSecOffset;
  Serial << "ReadOpenWeather(): Call OneCall OpenWeather.getForecast()" << endl;
  OpenWeather.getForecast(pCurrent, pHourly, pDaily, szAPIKey, szSLOLatitude, szSLOLongitude,
                          szUnits, szLanguage);
  fTemperature  = pCurrent->temp;
  ucHumidity    = pCurrent->humidity;

  // Delete to free up space and prevent fragmentation as strings change in length
  delete pCurrent;
  delete pHourly;
  delete pDaily;

  return;
} //ReadOpenWeather


void ReadWeatherTask(void *pvParameter){
  while(true){
    //Create the structures that hold the retrieved weather
    //OW_current, OW_hourly, OW_daily and OW_forecast are structs defined in Data_Point_Set.h
    OW_current    *pCurrent     = new OW_current;
    OW_hourly     *pHourly      = new OW_hourly;
    OW_daily      *pDaily       = new OW_daily;

    uwTimeZoneOffset= ulSLOSecOffset;
    OpenWeather.getForecast(pCurrent, pHourly, pDaily, szAPIKey, szSLOLatitude, 
                            szSLOLongitude, szUnits, szLanguage);
    fTemperature  = pCurrent->temp;
    ucHumidity    = pCurrent->humidity;

    // Delete to free up space and prevent fragmentation as strings change in length
    delete pCurrent;
    delete pHourly;
    delete pDaily;

    vTaskDelay(wReadWeatherTaskPeriodSec/portTICK_PERIOD_MS);
  } //while(true)

  //Never returns
} //ReadWeatherTask


String strTime(time_t unixTime){
  //Convert unix time to a time string
  unixTime += uwTimeZoneOffset;
  return ctime(&unixTime);
} //strTime

#if false   //Here just as documentation
void PrintOpenWeather(OW_current *pCurrent){
  Serial << "ReadOpenWeather(): dt          : " << (strTime(pCurrent->dt));
  Serial << "ReadOpenWeather(): Sunrise     : " << (strTime(pCurrent->sunrise));
  Serial << "ReadOpenWeather(): Sunset      : " << (strTime(pCurrent->sunset));
  Serial << "ReadOpenWeather(): main        : " << pCurrent->main << endl;
  Serial << "ReadOpenWeather(): description : " << pCurrent->description << endl;
  Serial << "ReadOpenWeather(): icon        : " << pCurrent->icon << endl;

  Serial << "ReadOpenWeather(): Latitude    :   " << (OpenWeather.lat) << endl;
  Serial << "ReadOpenWeather(): Longitude   : " << (OpenWeather.lon) << endl;
  //Serial << "ReadOpenWeather(): Temperature : " << fTemperature << " DegF" << endl;
  Serial << "ReadOpenWeather(): Temperature : " << fTemperature << "\370" << "F" << endl;
  Serial << "ReadOpenWeather(): Humidity    : " << ucHumidity << "%" << endl;
} //PrintOpenWeather
#endif

//Last line.
