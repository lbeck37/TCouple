const char szSketchName[]  = "B32_TCoupleRemote.ino";
const char szFileDate[]    = "10/15/23w";
/* MAX31855 library example sketch
 This sample code is designed to be used on the MAX31855x8 breakout board.
 The board has a single MAX31855 IC on it, and uses a multiplexer
 to select the correct thermoucouple.  The MAX31855 takes around 100ms to take
 an accurate temperature reading, so the best sample rate one can expect is to
 sample all 8 channels once-per-second.
 There are 2 versions of the MAX31855x8 board: 3.3V and 5V.
 There is a solder jumper on the board that can be changed to go from 5V or 3.3V.
 This sample code shows how to take a temperature reading:
 1. Set the multiplexer to the correct thermocouple channel
 2. Wait 125ms (0.125 seconds) for the MAX31855 to take an accurate reading
 3. Get the temperature from the MAX31855
 In the MAX31855 library, there are 2 functions:
 1. float readJunction([CELCUIS|FAHRENHEIT])
      Returns the internal temperature of the MAX31855 IC
 2. float readThermocouple([CELCUIS|FAHRENHEIT])
      Returns the temperature of the probe connected to the MAX31855
 readJunction() and readThermocouple() will return the temperature,
 or one of these errors:
 #define FAULT_OPEN        10000  // No thermocouple
 #define FAULT_SHORT_GND   10001  // Thermocouple short to ground
 #define FAULT_SHORT_VCC   10002  // Thermocouple short to VCC
 #define NO_MAX31855       10003  // MAX31855 not communicating
 Note:  If you connect the thermocouple probe the wrong way around, the temperature
 will go up instead of down (and vice versa).  No problem, just reverse the terminals.
 Released under WTFPL license, 27 October 2014 by Peter Easton
*/
#include <Arduino.h>
#include <MAX31855.h>
#include <Streaming.h>
#include <esp_now.h>
#include <WiFi.h>

//ESP32 GPIO pin numbers (range from 0 to 39)
#define T0   27
#define T1   26
#define T2   25
#define MISO 32
#define CS   17
#define SCK  33

#define ONE_DOT_RECEIVER    false       //ESP32 w/o USB-C, returned to Amazon
#define TWO_DOT_RECEIVER    false        //ESP32 w/o USB-C, returned to Amazon

#define RED_PIN_RECEIVER    false       //TTGO with red header pins, Remote tcouple reader
#define BLACK_PIN_RECEIVER  true        //TTGO with black header pins, tcouple display

#if RED_PIN_RECEIVER
  //Running on BlackPin TTGO, sends data to RedPin TTGO
  uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC};
#endif
#if BLACK_PIN_RECEIVER
  //Running on RedPin TTGO, sends data to BlackPin TTGO
  uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C};
#endif  //TWO_DOT_RECEIVER

//Create the temperature object, defining the pins used for communication
MAX31855 TCoupleObject = MAX31855(MISO, CS, SCK);

/*
//Red pin TTGO to be connected to 8x TCouple board and transmit to black pin TTGO
//Black pin TTGO is the display module. From B32_GetMACAddress.ino
//uint8_t aucRedPinMAC[]    = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //RedPin MAC
//uint8_t aucBlackPinMAC[]  = {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC}; //BlackPin MAC
uint8_t aucRedPinMAC[]    = {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC}; //RedPin MAC
*/
uint8_t aucBlackPinMAC[]  = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //BlackPin MAC

const int   wNumTCouples= 3;
double      adTCoupleDegF[wNumTCouples];

//Message Structure that is used to pass data back and forth
typedef struct stMessageStructure {
  double dTCouple0_DegF;
  double dTCouple1_DegF;
  double dTCouple2_DegF;
} stMessageStructure;

//Create an stMessageStructure to hold  sensor readings
stMessageStructure      stIncomingReadings;
stMessageStructure      stOutgoingReadings;
esp_now_peer_info_t     stPeerInfo;

double  dJunctionDegF;

//Variable to store if sending data was successful
String szSuccess;

//Function prototypes
void  setup             (void);
void  loop              (void);
void  OnDataRecv        (const uint8_t *pucMACAddress,
                         const uint8_t *pucIncomingData, int wNumBytes);
void  OnDataSent        (const uint8_t *pucMACAddress, esp_now_send_status_t wStatus);
void  SendDataToDisplay (void);
void  SetupPins         (void);
void  SetupESP_NOW      (void);
void  ReadAmbiant       (void);
void  ReadTCouples      (void);
void  PrintTemperatures (void);
void  PrintTemperature  (double dDegF);

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  SetupPins();
  SetupESP_NOW();

  return;
}   //setup


void loop() {
  ReadAmbiant();
  ReadTCouples();
  PrintTemperatures();
  SendDataToDisplay();

  delay(3000);
  return;
}   //loop


//Callback when data is received.
void OnDataRecv(const uint8_t *pucMACAddress, const uint8_t *pucIncomingData, int wNumBytes) {
  memcpy(&stIncomingReadings, pucIncomingData, sizeof(stIncomingReadings));
  Serial << "OnDataRecv():  Bytes received= " << wNumBytes << endl;
  return;
} //OnDataRecv


// Callback when data is sent
void OnDataSent(const uint8_t *pucMACAddress, esp_now_send_status_t wStatus) {
  if (wStatus == ESP_NOW_SEND_SUCCESS){
    Serial << endl << "OnDataSent(): Last Packet Send Status: FAIL" << endl;
  }
  else {
    Serial << endl << "OnDataSent(): Last Packet Send Status: Success" << endl;
  }
  return;
} //OnDataSent


void SendDataToDisplay(void){
  stOutgoingReadings.dTCouple0_DegF= adTCoupleDegF[0];
  stOutgoingReadings.dTCouple1_DegF= adTCoupleDegF[1];
  stOutgoingReadings.dTCouple2_DegF= adTCoupleDegF[2];

  esp_err_t wResult= esp_now_send(aucBlackPinMAC,
                                  (uint8_t *)&stOutgoingReadings,
                                  sizeof(stOutgoingReadings));
/*
  esp_err_t wResult= esp_now_send(broadcastAddress,
                                  (uint8_t *)&stOutgoingReadings,
                                  sizeof(stOutgoingReadings));
*/
  if (wResult == ESP_OK) {
    Serial << "Sent with success" << endl;
  }
  else {
    Serial << "Error sending the data" << endl;
  }
  return;
} //SendDataToDisplay


void SetupPins(void){
  pinMode(T0,   OUTPUT);
  pinMode(T1,   OUTPUT);
  pinMode(T2,   OUTPUT);

  pinMode(MISO, INPUT);
  pinMode(CS,   OUTPUT);
  pinMode(SCK,  OUTPUT);

  delay(200);
  return;
} //SetupPins


void SetupESP_NOW(void){
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial << "SetupESP_NOW(): Error initializing ESP-NOW" << endl;
    return;
  } // if(esp_now_init()!=ESP_OK)

  //Register remote module
  //memcpy(stPeerInfo.peer_addr, aucRedPinMAC, 6);
  memcpy(stPeerInfo.peer_addr, broadcastAddress, 6);
  stPeerInfo.channel = 0;
  stPeerInfo.encrypt = false;

  //Add peer
  if (esp_now_add_peer(&stPeerInfo) != ESP_OK){
    Serial << "SetupESP_NOW(): Failed to add peer" << endl;
    return;
  }   //if(esp_now_add_peer(&stPeerInfo)!=ESP_OK)

  //Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  return;
}   //SetupESP_NOW


void ReadAmbiant(void){
  dJunctionDegF= TCoupleObject.readJunction(FAHRENHEIT);
  return;
} //ReadAmbiant


void ReadTCouples(void){
  //Read the temperatures of the 8 thermocouples
  for (int wTCoupleNum=0; (wTCoupleNum < wNumTCouples); wTCoupleNum++) {
    //Select the thermocouple
    digitalWrite(T0, wTCoupleNum & 1? HIGH: LOW);
    digitalWrite(T1, wTCoupleNum & 2? HIGH: LOW);
    digitalWrite(T2, wTCoupleNum & 4? HIGH: LOW);
    //The MAX31855 takes 100ms to sample the TCouple.
    //Wait a bit longer to be safe.  We'll wait 0.125 seconds
    delay(125);

    adTCoupleDegF[wTCoupleNum]= TCoupleObject.readThermocouple(FAHRENHEIT);
    if (adTCoupleDegF[wTCoupleNum] == FAULT_OPEN){
      //Break out of for loop, go to top of for loop and next TCouple
      continue;
    } //if(temperature==FAULT_OPEN)
  } //for(int wTCoupleNum=0;wTCoupleNum<8;wTCoupleNum++)
  return;
}   //ReadTCouples


void PrintTemperatures(void){
  Serial << "Ambiant= ";
  PrintTemperature(dJunctionDegF);
  for (int wTCoupleNum=0; (wTCoupleNum < wNumTCouples); wTCoupleNum++) {
    Serial << ", ";
    Serial << "T" << wTCoupleNum << "= ";
    PrintTemperature(adTCoupleDegF[wTCoupleNum]);
  } //for(int wTCoupleNum=0;wTCoupleNum<8;wTCoupleNum++)
  Serial << endl;
  return;
} //PrintTemperatures


//Print the temperature, or the type of fault
void PrintTemperature(double dDegF) {
  //switch statement takes an integer value and executes the case that corresponds
  //Cast the double dDegF to be an int so it becomes an error code
  switch ((int)dDegF){
    case FAULT_OPEN:
      Serial << "FAULT_OPEN";
      break;
    case FAULT_SHORT_GND:
      Serial << "FAULT_SHORT_GND";
      break;
    case FAULT_SHORT_VCC:
      Serial << "FAULT_SHORT_VCC";
      break;
    case NO_MAX31855:
      Serial << "NO_MAX31855";
      break;
    default:
      Serial << dDegF;
      break;
  } //switch
}//PrintTemperature
//Last line.
