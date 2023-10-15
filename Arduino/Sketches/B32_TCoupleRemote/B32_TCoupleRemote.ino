const char szSketchName[]  = "B32_TCoupleRemote.ino";
const char szFileDate[]    = "10/15/23h";
/*
 MAX31855 library example sketch

 This sample code is designed to be used on the MAX31855x8 breakout board.
 Look for the MAX31855 breakout boards on www.whizoo.com.

 This sample code corresponds to the MAX31855x8 board.
 The board has a single MAX31855 IC on it, and uses a multiplexer
 to select the correct thermoucouple.  The MAX31855 takes around 100ms to take
 an accurate temperature reading, so the best sample rate one can expect is to
 sample all 8 channels once-per-second.  If you are only sampling 2 channels
 then you can do it 4 times-per-second, and so on.

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

 readJunction() and readThermocouple() will return the temperature, or one of these errors:
 #define FAULT_OPEN        10000  // No thermocouple
 #define FAULT_SHORT_GND   10001  // Thermocouple short to ground
 #define FAULT_SHORT_VCC   10002  // Thermocouple short to VCC
 #define NO_MAX31855       10003  // MAX31855 not communicating

 Note:  If you connect the thermocouple probe the wrong way around, the temperature will go up
 instead of down (and vice versa).  No problem, just reverse the terminals.

 Released under WTFPL license.
 27 October 2014 by Peter Easton
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

//Create the temperature object, defining the pins used for communication
MAX31855 TCoupleObject = MAX31855(MISO, CS, SCK);

//Red pin TTGO to be connected to 8x TCouple board and transmit to black pin TTGO
//Black pin TTGO is the display module. From B32_GetMACAddress.ino
uint8_t aucRedPinMAC[]    = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //RedPin MAC
uint8_t aucBlackPinMAC[]  = {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC}; //BlackPin MAC

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

// Variable to store if sending data was successful
String szSuccess;

//Function prototypes
void  setup             (void);
void  loop              (void);
void  SetupPins         (void);
void  SetupESP_NOW      (void);
void  OnDataRecv        (const uint8_t *pucMACAddress,
                         const uint8_t *pucIncomingData, int wNumBytes);
void  OnDataSent        (const uint8_t *pucMACAddress, esp_now_send_status_t wStatus);
void  ReadTCouples      (void);
void  printTemperature  (double dDegF);

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  SetupPins();
  SetupESP_NOW();

  return;
}   //setup


void loop() {
  // Display the junction temperature
  double  dJunctionDegF = TCoupleObject.readJunction(FAHRENHEIT);

  //float temperature = temp.readJunction(FAHRENHEIT);
  Serial.print("Ambiant=");
  printTemperature(dJunctionDegF);

/*
  // Display the temperatures of the 8 thermocouples
  for (int therm=0; therm<8; therm++) {
    // Select the thermocouple
    digitalWrite(T0, therm & 1? HIGH: LOW);
    digitalWrite(T1, therm & 2? HIGH: LOW);
    digitalWrite(T2, therm & 4? HIGH: LOW);
    // The MAX31855 takes 100ms to sample the thermocouple.
    // Wait a bit longer to be safe.  We'll wait 0.125 seconds
    delay(125);

    dTCoupleDegF = TCoupleObject.readThermocouple(FAHRENHEIT);
    if (dTCoupleDegF == FAULT_OPEN){
        continue;
    } //if(temperature==FAULT_OPEN)
    Serial.print(" T");
    Serial.print(therm);
    Serial.print("=");
    printTemperature(dTCoupleDegF);
    } //for(int therm=0;therm<8;therm++)
  Serial.println();
*/
  ReadTCouples();
  return;
}   //loop


void ReadTCouples(void){
  double  dTCoupleDegF;
  // Read the temperatures of the 8 thermocouples
  for (int therm=0; therm<8; therm++) {
    // Select the thermocouple
    digitalWrite(T0, therm & 1? HIGH: LOW);
    digitalWrite(T1, therm & 2? HIGH: LOW);
    digitalWrite(T2, therm & 4? HIGH: LOW);
    // The MAX31855 takes 100ms to sample the thermocouple.
    // Wait a bit longer to be safe.  We'll wait 0.125 seconds
    delay(125);

    dTCoupleDegF = TCoupleObject.readThermocouple(FAHRENHEIT);
    if (dTCoupleDegF == FAULT_OPEN){
        continue;
    } //if(temperature==FAULT_OPEN)
/*
    Serial.print(" T");
    Serial.print(therm);
    Serial.print("=");
*/
    Serial << "ReadTCouples(): T" << therm << "= ";
    printTemperature(dTCoupleDegF);
    } //for(int therm=0;therm<8;therm++)
  //Serial.println();
  Serial << endl;
  return;
}   //ReadTCouples


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
    //Serial.println("SetupESP_NOW(): Error initializing ESP-NOW");
    Serial << "SetupESP_NOW(): Error initializing ESP-NOW" << endl;
    return;
  } // if(esp_now_init()!=ESP_OK)

  //Register remote module
  memcpy(stPeerInfo.peer_addr, aucRedPinMAC, 6);
  stPeerInfo.channel = 0;
  stPeerInfo.encrypt = false;

  //Add peer
  if (esp_now_add_peer(&stPeerInfo) != ESP_OK){
    //Serial.println("SetupESP_NOW(): Failed to add peer");
    Serial << "SetupESP_NOW(): Failed to add peer" << endl;
    return;
  }   //if(esp_now_add_peer(&stPeerInfo)!=ESP_OK)

  //Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  return;
}   //SetupESP_NOW


//Callback when data is received.
void OnDataRecv(const uint8_t *pucMACAddress, const uint8_t *pucIncomingData, int wNumBytes) {
  memcpy(&stIncomingReadings, pucIncomingData, sizeof(stIncomingReadings));
/*
  Serial.print("OnDataRecv(): Bytes received: ");
  Serial.println(wNumBytes);
*/
  Serial << "OnDataRecv():  Bytes received= " << wNumBytes << endl;
  return;
} //OnDataRecv


// Callback when data is sent
void OnDataSent(const uint8_t *pucMACAddress, esp_now_send_status_t wStatus) {
/*
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(wStatus == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
*/
  //Serial << endl << "OnDataSent(): Last Packet Send Status: Delivery Success" << endl;
      if (wStatus == ESP_NOW_SEND_SUCCESS){
        Serial << endl << "OnDataSent(): Last Packet Send Status: FAIL" << endl;
      }
      else {
        Serial << endl << "OnDataSent(): Last Packet Send Status: Success" << endl;
      }
/*
  if (wStatus == 0){
    szSuccess = "Delivery Success :)";
  }
  else{
    szSuccess = "Delivery Fail :(";
  }
*/
  return;
} //OnDataSent


// Print the temperature, or the type of fault
void printTemperature(double dDegF) {
  //switch statement takes an integer value and executes the case that corresponds
  //Cast the double dDegF to be an int so it becomes an error code
/*
  switch ((int)dDegF) {
    case FAULT_OPEN:
      Serial.print("FAULT_OPEN");
      break;
    case FAULT_SHORT_GND:
      Serial.print("FAULT_SHORT_GND");
      break;
    case FAULT_SHORT_VCC:
      Serial.print("FAULT_SHORT_VCC");
      break;
    case NO_MAX31855:
      Serial.print("NO_MAX31855");
      break;
    default:
      Serial.print(dDegF);
      break;
  }
  Serial.print(" ");
*/
  switch ((int)dDegF){
    case FAULT_OPEN:
      //Serial.print("FAULT_OPEN");
      Serial << "FAULT_OPEN";
      break;
    case FAULT_SHORT_GND:
      //Serial.print("FAULT_SHORT_GND");
      Serial << "FAULT_SHORT_GND";
      break;
    case FAULT_SHORT_VCC:
      //Serial.print("FAULT_SHORT_VCC");
      Serial << "FAULT_SHORT_VCC";
      break;
    case NO_MAX31855:
      //Serial.print("NO_MAX31855");
      Serial << "NO_MAX31855";
      break;
    default:
      //Serial.print(dDegF);
      Serial << dDegF;
      break;
  } //switch
  //Serial.print(" ");
  Serial << " ";
}//printTemperature
//Last line.
