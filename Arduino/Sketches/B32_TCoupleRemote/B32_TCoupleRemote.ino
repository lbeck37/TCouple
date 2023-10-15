const char szSketchName[]  = "B32_TCoupleRemote.ino";
const char szFileDate[]    = "10/14/23b";
/*
 MAX31855 library example sketch
 
 This sample code is designed to be used on the MAX31855x8 breakout board.
 Look for the MAX31855 breakout boards on www.whizoo.com.
 
 This sample code corresponds to the MAX31855x8 board.  The board has a single MAX31855 IC on it, and uses a multiplexer
 to select the correct thermoucouple.  The MAX31855 takes around 100ms to take an accurate temperature reading, so 
 the best sample rate one can expect is to sample all 8 channels once-per-second.  If you are only sampling 2 channels
 then you can do it 4 times-per-second, and so on.
 
 There are 2 versions of the MAX31855x8 board: 3.3V and 5V.  There is a solder jumper on the board
 that can be changed to go from one voltage to the other.
 
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
#include <Streaming.h>
#include <MAX31855.h>

#define VccGndOnOutputPins    false
// Pin connections to the MAX31855x8 board 
// The power requirement for the board is less than 2mA.  Most microcontrollers can source or sink a lot more
// than that one each I/O pin.  For example, the ATmega328 supports up to 20mA.  For convenience, the board
// is placed directly on top of a row of I/O pins on the microcontroller.  Power is supplied to the board by
// holding the GND pin low and the VIN pin high
/*
#define GND  3
#define T0   4
#define T1   5
#define T2   6
#define VIN  7
#define MISO 8
#define CS   9
#define SCK  10
*/
#define T0   27
#define T1   26
#define T2   25
#define MISO 32
#define CS   17
#define SCK  33
#if VccGndOnOutputPins
  #define VIN  12
  #define GND  13
#endif
/*
#define RELAY_ON 1
#define RELAY_OFF 0
#define RELAY_1  12
#define RELAY_2  11
#define RELAY_3  13
#define RELAY_4  2
*/

// Create the temperature object, defining the pins used for communication
MAX31855 temp = MAX31855(MISO, CS, SCK);

//Function prototypes
void  setup             (void);
void  loop              (void);
void  SetupPins         (void);
void  SetupESP_NOW      (void);
void  OnDataSent        (const uint8_t *mac_addr, esp_now_send_status_t status);
void  printTemperature  (double dDegF);

void setup() {
  // Display temperatures using the serial port
  //Serial.begin(9600);
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  
  SetupPins();
  SetupESP_NOW();
  
  return;
}   //setup


void loop () {
  // Display the junction temperature
  double  dJunctionDegF = temp.readJunction(FAHRENHEIT);
  double  dTCoupleDegF;

  //float temperature = temp.readJunction(FAHRENHEIT);
  Serial.print("Ambiant=");
  printTemperature(dJunctionDegF);
    
  // Display the temperatures of the 8 thermocouples
  for (int therm=0; therm<8; therm++) {
    // Select the thermocouple
    digitalWrite(T0, therm & 1? HIGH: LOW);
    digitalWrite(T1, therm & 2? HIGH: LOW);
    digitalWrite(T2, therm & 4? HIGH: LOW);
    // The MAX31855 takes 100ms to sample the thermocouple.
    // Wait a bit longer to be safe.  We'll wait 0.125 seconds
    delay(125);
    
    dTCoupleDegF = temp.readThermocouple(FAHRENHEIT);
    if (dTCoupleDegF == FAULT_OPEN){
        continue;
    } //if(temperature==FAULT_OPEN)
    Serial.print(" T");
    Serial.print(therm);
    Serial.print("=");
    printTemperature(dTCoupleDegF);
/*
    // Right here is where I think the line of code should go to turn on and off Relay_1
    // which is the relay I want to activate when Thermocouple0 is above 70 degrees F
    // the other three relays I waant to try to make work with the other 6 thermocouples...
    // these thermos will activate the relay/s when the temperature gets lower than a set point
    if (????? >= 70){
       digitalWrite(RELAY_1, RELAY_ON);
       }
    else (????? >= 71); {
       digitalWrite(RELAY_1, RELAY_OFF);
      }
*/
    } //for(int therm=0;therm<8;therm++)
  Serial.println();
}//loop


void SetupPins(void(){
  // Initialize pins
  pinMode(T0,   OUTPUT);
  pinMode(T1,   OUTPUT);
  pinMode(T2,   OUTPUT);

  pinMode(MISO, INPUT);
  pinMode(CS,   OUTPUT);
  pinMode(SCK,  OUTPUT);

/*
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
*/

  // Power up the board
/*
  digitalWrite(RELAY_1, RELAY_OFF);
  digitalWrite(RELAY_2, RELAY_OFF);
  digitalWrite(RELAY_3, RELAY_OFF);
  digitalWrite(RELAY_4, RELAY_OFF);
*/
#if VccGndOnOutputPins
  pinMode(VIN, OUTPUT);
  pinMode(GND, OUTPUT);

  digitalWrite(GND, LOW);
  digitalWrite(VIN, HIGH);
#endif
  delay(200);
  return;
} //SetupPins


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


// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
} //OnDataSent


// Print the temperature, or the type of fault
void printTemperature(double dDegF) {
  switch ((int) dDegF) {
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
}
//Last line.
