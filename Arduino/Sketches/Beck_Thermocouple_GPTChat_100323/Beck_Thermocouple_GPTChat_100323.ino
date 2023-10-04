const char szSketchName[]  = "Beck_Thermocouple_GPTChat_100323.ino";
const char szFileDate[]    = "10/3/23a";
//The is the code that Chat GPT gave Kevin when he queried about using
//the Lattepanda arduino/win 10 board with a lcd panel, keypad and ssr control.

#include <Adafruit_MAX31855.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Define pins for Max31855
#define MAX_CS_PIN 10 // Define your Chip Select (CS) pin for Max31855
Adafruit_MAX31855 thermocouple[MAX_CHANNELS] = {
  Adafruit_MAX31855(MAX_CS_PIN),  // Create instances for each channel (MAX_CHANNELS should be defined as 8)
  Adafruit_MAX31855(MAX_CS_PIN),
  // ... Repeat for all 8 channels
};

// Define pins for SSR control
int ssrPins[MAX_CHANNELS] = {2, 3, 4, 5, 6, 7, 8, 9}; // SSR control pins

// Define LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// Define Keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3}; // Connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A5, 2, 3}; // Connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  // Initialize LCD and print a welcome message
  lcd.begin(16, 2);
  lcd.print("Temperature Ctrl");

  // Initialize SSR control pins
  for (int i = 0; i < MAX_CHANNELS; i++) {
    pinMode(ssrPins[i], OUTPUT);
  }
}

void loop() {
  char key = keypad.getKey();

  // Check if a key is pressed
  if (key) {
    // Perform actions based on the key pressed
    switch(key) {
      case '1':
        // Code to control SSR 1
        break;
      case '2':
        // Code to control SSR 2
        break;
      // ... Repeat for all keys (1 to 8) for SSR control

      case 'A':
        // Code to read temperature from channel 1 and display on LCD
        float temp1 = thermocouple[0].readCelsius();
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print(temp1);
        lcd.print(" C  ");
        break;
      // ... Repeat for other channels (B, C, D for channels 2 to 4)
    }
  }
}
