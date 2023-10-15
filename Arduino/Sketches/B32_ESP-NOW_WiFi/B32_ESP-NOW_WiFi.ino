const char szSketchName[]  = "B32_ESP-NOW_WiFi.ino";
const char szFileDate[]    = "10/15/23h, BlackPin Receiver";
// Beck 10/13/23, B32_ESP-NOW_WiFi.ino
/*Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Streaming.h>
#include <esp_now.h>
#include <WiFi.h>

#define WITH_SENSOR_AND_DISPLAY    false

#if WITH_SENSOR_AND_DISPLAY
  #include <Wire.h>
  #include <Adafruit_Sensor.h>
  #include <Adafruit_BME280.h>

  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
#endif  //WITH_SENSOR_AND_DISPLAY

#define ONE_DOT_RECEIVER    false       //ESP32 w/o USB-C, returned to Amazon
#define TWO_DOT_RECEIVER    false        //ESP32 w/o USB-C, returned to Amazon

#define RED_PIN_RECEIVER    false       //TTGO with red header pins
#define BLACK_PIN_RECEIVER  true        //TTGO with black header pins

//Red pin TTGO to be connected to 8x tcouple board and transmit to black pin TTGO
//Black pin TTGO is the display module
//  Receives (3) tcouple readings from red pin, readings are printed as T, H and P

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#if WITH_SENSOR_AND_DISPLAY
  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

  Adafruit_BME280 bme;
#endif

//Red pin TTGO to be connected to 8x TCouple board and transmit to black pin TTGO
//Black pin TTGO is the display module. From B32_GetMACAddress.ino
//uint8_t aucRedPinMAC[]    = {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C}; //RedPin MAC
//uint8_t aucBlackPinMAC[]  = {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC}; //BlackPin MAC

#if TWO_DOT_RECEIVER
  //TwoDot ESP32 MAC Address for receiver
  uint8_t broadcastAddress[]= {0x48, 0xE7, 0x29, 0xB6, 0xC3,0xA0};
#endif
#if ONE_DOT_RECEIVER
  //OneDot ESP32 MAC Address for receiver
  uint8_t broadcastAddress[]= {0x48, 0xE7, 0x29, 0xAF, 0x7B,0xDC};
#endif  //TWO_DOT_RECEIVER

#if RED_PIN_RECEIVER
  //Running on BlackPin TTGO, sends data to RedPin TTGO
  //uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C};
  uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC};
#endif
#if BLACK_PIN_RECEIVER
  //Running on RedPin TTGO, sends data to BlackPin TTGO
  //uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x32, 0xCC};
  uint8_t broadcastAddress[]= {0xB0, 0xB2, 0x1C, 0x4F, 0x28, 0x0C};
#endif  //TWO_DOT_RECEIVER


// Define variables to store BME280 readings to be sent
float temperature;
float humidity;
float pressure;

// Define variables to store incoming readings
float incomingTemp;
float incomingHum;
float incomingPres;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    float temp;
    float hum;
    float pres;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_message BME280Readings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

esp_now_peer_info_t peerInfo;


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


// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingTemp = incomingReadings.temp;
  incomingHum = incomingReadings.hum;
  incomingPres = incomingReadings.pres;
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
  Serial << endl << "My MAC Address is- " << WiFi.macAddress() << endl;

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
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
  BME280Readings.temp = temperature;
  BME280Readings.hum = humidity;
  BME280Readings.pres = pressure;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &BME280Readings, sizeof(BME280Readings));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  updateDisplay();
  delay(3000);
  return;
}//loop


void getReadings(){
#if WITH_SENSOR_AND_DISPLAY
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = (bme.readPressure() / 100.0F);
#else
  static int  wCount= 0;
  temperature   = wCount++  * 1.0;
  humidity      = wCount    * 10.0;
  pressure      = wCount    * 100.0;
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
  display.print(incomingTemp);
  display.cp437(true);
  display.write(248);
  display.print("C");
  display.setCursor(0, 25);
  display.print("Humidity: ");
  display.print(incomingHum);
  display.print("%");
  display.setCursor(0, 35);
  display.print("Pressure: ");
  display.print(incomingPres);
  display.print("hPa");
  display.setCursor(0, 56);
  display.print(success);
  display.display();
#endif  //WITH_SENSOR_AND_DISPLAY

  // Display Readings in Serial Monitor
  Serial.println("INCOMING READINGS");
  Serial.print("Temperature: ");
  Serial.print(incomingReadings.temp);
  Serial.println(" �C");
  Serial.print("Humidity: ");
  Serial.print(incomingReadings.hum);
  Serial.println(" %");
  Serial.print("Pressure: ");
  Serial.print(incomingReadings.pres);
  Serial.println(" hPa");
  Serial.println();
}   //updateDisplay
//Last line.
