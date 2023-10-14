const char szSketchName[]  = "B32_TCoupleDisplay.ino";
const char szFileDate[]    = "10/14/23e";
//Thanks to Rui Santos, https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32

//This sketch, B32_TCoupleDisplay.ino), and B32_TCoupleModule.ino share WiFi
//communication code from the esp_now.h library. The example used to generate this code
//was an example with a Adafruit_BME280 temp, humidity pressure chip and
//Adafruit_SSD1306 display libraries.
//Previously it was tested on ESP32 chips idenfiied by either one or two dots on top.
#include <Streaming.h>
#include <esp_now.h>
#include <WiFi.h>

#define WITH_SENSOR_AND_DISPLAY     false

#define BUILD_AS_TCOUPLE_DISPLAY    true
#define BUILD_AS_TCOUPLE_READER     false

#if WITH_SENSOR_AND_DISPLAY
  #include <Wire.h>
  #include <Adafruit_Sensor.h>
  #include <Adafruit_BME280.h>

  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
#endif  //WITH_SENSOR_AND_DISPLAY

//Red pin TTGO to be connected to 8x TCouple board and transmit to black pin TTGO
//Black pin TTGO is the display module
//  Receives (3) TCouple readings from red pin, readings are printed as T, H and P

#if WITH_SENSOR_AND_DISPLAY
#define ONE_DOT_RECEVIER    false       //ESP32 w/o USB-C, returned to Amazon
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

  Adafruit_BME280 bme;
#endif

// REPLACE WITH THE MAC Address of your receiver
//uint8_t RemoteMAC[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
/*
#if TWO_DOT_RECEVIER
  //TwoDot ESP32 MAC Address for receiver
  uint8_t RemoteMAC[]    = {0x48, 0xE7, 0x29, 0xB6, 0xC3,0xA0};
#else
  //OneDot ESP32 MAC Address for receiver
  uint8_t RemoteMAC[]    = {0x48, 0xE7, 0x29, 0xAF, 0x7B,0xDC};
#endif  //TWO_DOT_RECEVIER
*/

//From B32_GetMACAddress.ino
  //Red pin TT-GO T-Display   - B0:B2:1C:4F:28:0C
  //Black pin TT-GO T-Display - B0:B2:1C:4F:32:CC
#if BUILD_AS_TCOUPLE_DISPLAY
  //We are running on BlackPin TTGO as the Display and we receive from RedPin
  uint8_t RemoteMAC[]   = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //RedPin MAC
#endif

#if BUILD_AS_TCOUPLE_READER
  //We are running on RedPin TTGO and we send TCouple readings to BlackPin Display
  uint8_t RemoteMAC[]   = {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC}; //BlackPin MAC
#endif  //TWO_DOT_RECEVIER

// Define variables to store BME280 readings to be sent
/*
float temperature;
float humidity;
float pressure;
*/
double dTCouple0_DegF;
double dTCouple1_DegF;
double dTCouple2_DegF;

// Define variables to store incoming readings
/*
float incomingTemp;
float incomingHum;
float incomingPres;
*/
double dIncomingTCouple0_DegF;
double dIncomingTCouple1_DegF;
double dIncomingTCouple2_DegF;

// Variable to store if sending data was successful
String success;

/*
//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    float temp;
    float hum;
    float pres;
} struct_message;
*/
//Message Structure that is used to pass data back an forth
typedef struct stMessageStructure {
    double dTCouple0_DegF;
    double dTCouple1_DegF;
    double dTCouple2_DegF;
} stMessageStructure;

// Create a stMessageStructure called BME280Readings to hold sensor readings
stMessageStructure BME280Readings;

// Create a stMessageStructure to hold incoming sensor readings
stMessageStructure incomingReadings;

esp_now_peer_info_t peerInfo;


//Callback when data is sent. Used by TCouple Module that sends to Display Module
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
  return;
} //OnDataSent


//Callback when data is received. Used by Display that receives from TCouple Module
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  dIncomingTCouple0_DegF = incomingReadings.dTCouple0_DegF;
  dIncomingTCouple1_DegF = incomingReadings.dTCouple1_DegF;
  dIncomingTCouple2_DegF = incomingReadings.dTCouple2_DegF;
  return;
} //OnDataRecv


void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  delay(100);
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

#if WITH_SENSOR_AND_DISPLAY
  // Init BME280 sensor
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // Init OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
#endif  //WITH_SENSOR_AND_DISPLAY

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, RemoteMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  return;
} //setup


void loop() {
  getReadings();

  // Set values to send
  BME280Readings.dTCouple0_DegF = dTCouple0_DegF;
  BME280Readings.dTCouple1_DegF = dTCouple1_DegF;
  BME280Readings.dTCouple2_DegF = dTCouple2_DegF;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(RemoteMAC, (uint8_t *) &BME280Readings, sizeof(BME280Readings));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  updateDisplay();
  delay(10000);
  return;
}//loop


void getReadings(){
#if WITH_SENSOR_AND_DISPLAY
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = (bme.readPressure() / 100.0F);
#else
  static int  wCount= 0;
  dTCouple0_DegF   = wCount++  * 1.0;
  dTCouple1_DegF      = wCount    * 10.0;
  dTCouple2_DegF      = wCount    * 100.0;
#endif  //WITH_SENSOR_AND_DISPLAY
  return;
}//getReadings


void updateDisplay(){
#if WITH_SENSOR_AND_DISPLAY
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
  display.print(success);
  display.display();
#endif  //WITH_SENSOR_AND_DISPLAY

  // Display Readings in Serial Monitor
  Serial.println("INCOMING READINGS");
  Serial.print("Temperature: ");
  Serial.print(incomingReadings.dTCouple0_DegF);
  Serial.println(" �C");
  Serial.print("Humidity: ");
  Serial.print(incomingReadings.dTCouple1_DegF);
  Serial.println(" %");
  Serial.print("Pressure: ");
  Serial.print(incomingReadings.dTCouple2_DegF);
  Serial.println(" hPa");
  Serial.println();
  return;
}   //updateDisplay
//Last line.
