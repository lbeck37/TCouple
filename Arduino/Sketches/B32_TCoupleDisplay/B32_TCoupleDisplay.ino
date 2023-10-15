const char szSketchName[]  = "B32_TCoupleDisplay.ino";
const char szFileDate[]    = "10/15/23b";
//Thanks to Rui Santos, https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32

//This sketch, B32_TCoupleDisplay.ino), and B32_TCoupleModule.ino share WiFi
//communication code from the esp_now.h library.
//Previously it was tested on two ESP32 chips idenfiied by either one or two dots on top.
#include <Streaming.h>
#include <esp_now.h>
#include <WiFi.h>

#define WITH_DISPLAY     false

#if WITH_DISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#endif


//Red pin TTGO to be connected to 8x TCouple board and transmit to black pin TTGO
//Black pin TTGO is the display module
//From B32_GetMACAddress.ino
//uint8_t OneDotMAC[]     = {0x48, 0xE7, 0x29, 0xAF, 0x7B,0xDC};  //Returned to Amazon
//uint8_t TwoDotMAC[]     = {0x48, 0xE7, 0x29, 0xB6, 0xC3,0xA0};  //Returned to Amazon
uint8_t aucRedPinMAC[]    = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //RedPin MAC
uint8_t aucBlackPinMAC[]  = {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC}; //BlackPin MAC

//Define variables to store BME280 readings to be sent
double dTCouple0_DegF;
double dTCouple1_DegF;
double dTCouple2_DegF;

// Define variables to store incoming readings
double dIncomingTCouple0_DegF;
double dIncomingTCouple1_DegF;
double dIncomingTCouple2_DegF;

// Variable to store if sending data was successful
String szSuccess;

//Message Structure that is used to pass data back an forth
typedef struct stMessageStructure {
  double dTCouple0_DegF;
  double dTCouple1_DegF;
  double dTCouple2_DegF;
} stMessageStructure;

// Create a stMessageStructure to hold incoming sensor readings
stMessageStructure      stIncomingReadings;
stMessageStructure      stOutgoingReadings;
esp_now_peer_info_t     stPeerInfo;

//Function prototypes
void  setup           (void);
void  loop            (void);
void  SetupDisplay    (void);
void  SetupESP_NOW    (void);
void  OnDataRecv      (const uint8_t *pucMACAddress,
                       const uint8_t *pucIncomingData, int wNumBytes);
void  OnDataSent      (const uint8_t *mac_addr, esp_now_send_status_t status);
void  UpdateDisplay   (void);

void setup(){
  // Init Serial Monitor
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  SetupDisplay();
  SetupESP_NOW();
  return;
} //setup


void loop(){
  UpdateDisplay();
  delay(10000);
  return;
}//loop


void SetupDisplay(){
#if WITH_DISPLAY
  // Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
#endif  //WITH_DISPLAY
  return;
}//SetupDisplay


void SetupESP_NOW(void){
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("SetupESP_NOW(): Error initializing ESP-NOW");
    return;
  } // if(esp_now_init()!=ESP_OK)

  //Register remote module
  memcpy(stPeerInfo.peer_addr, aucRedPinMAC, 6);
  stPeerInfo.channel = 0;
  stPeerInfo.encrypt = false;

  //Add peer
  if (esp_now_add_peer(&stPeerInfo) != ESP_OK){
    Serial.println("SetupESP_NOW(): Failed to add peer");
    return;
  }   //if(esp_now_add_peer(&stPeerInfo)!=ESP_OK)

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  return;
}   //SetupESP_NOW


//Callback when data is received. Used by Display that receives from TCouple Module
void OnDataRecv(const uint8_t *pucMACAddress, const uint8_t *pucIncomingData, int wNumBytes) {
  memcpy(&stIncomingReadings, pucIncomingData, sizeof(stIncomingReadings));
  Serial.print("OnDataRecv(): Bytes received: ");
  Serial.println(wNumBytes);
  dIncomingTCouple0_DegF = stIncomingReadings.dTCouple0_DegF;
  dIncomingTCouple1_DegF = stIncomingReadings.dTCouple1_DegF;
  dIncomingTCouple2_DegF = stIncomingReadings.dTCouple2_DegF;
  return;
} //OnDataRecv


void UpdateDisplay(){
#if WITH_DISPLAY
  // Display Readings on OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("INCOMING READINGS");
  display.setCursor(0, 15);
  display.print("Temperature: ");
  display.print(dIncomingTCouple0_DegF);
  display.cp437(true);
  display.write(248);
  display.print("C");
  display.setCursor(0, 25);
  display.print("Humidity: ");
  display.print(dIncomingTCouple1_DegF);
  display.print("%");
  display.setCursor(0, 35);
  display.print("Pressure: ");
  display.print(dIncomingTCouple2_DegF);
  display.print("hPa");
  display.setCursor(0, 56);
  display.print(szSuccess);
  display.display();
#endif  //WITH_DISPLAY

  // Display Readings in Serial Monitor
  Serial.println("UpdateDisplay(): INCOMING READINGS");
  Serial.print("TCouple 0: ");
  Serial.print(stIncomingReadings.dTCouple0_DegF);
  Serial.println(" F");
  Serial.print("TCouple 1: ");
  Serial.print(stIncomingReadings.dTCouple1_DegF);
  Serial.println(" F");
  Serial.print("TCouple 2: ");
  Serial.print(stIncomingReadings.dTCouple2_DegF);
  Serial.println(" F");
  Serial.println();
  return;
}   //UpdateDisplay
//Last line.
