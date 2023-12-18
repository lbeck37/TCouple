//Beck, 12/17/23, was 11/15/23b
// See SetupX_Template.h for all options available
#define USER_SETUP_ID 1

#define ILI9341_DRIVER

#define DO_WALMART_DISPLAY      true
#define DO_ESP32_DISPLAY        false

#if DO_ESP32_DISPLAY
//From ElectroDragon Wiki "ESP-LCD32
  #define TFT_MOSI            23
  #define TFT_SCLK            18
  #define TFT_CS              27
  #define TFT_DC              32
  #define TFT_RST              5
  #define TFT_BL              19    // Display backlight control pin, not sure if this even works
  //#define TFT_BACKLIGHT_ON    HIGH    // HIGH or LOW are options
  #define TFT_BACKLIGHT_ON    LOW   // HIGH or LOW are options
#endif  //DO_ESP32_DISPLAY

#if DO_WALMART_DISPLAY
  #pragma message("Setup1_ILI9341.h, defining TFT_MOSI, SCLK, CS, DC, RST, BL")
  //Start with https://forum.arduino.cc/t/esp32-2432s028r-all-in-one-display-touch-spi-problems/1059746
  //Also found in esp32-smartdisplay.h for ESP32-2432S024N, 24R, 24C 28R
  //24C is only one with backlight on 27 like our Walmart, others are 21
  #define TFT_MOSI            13
  #define TFT_SCLK            14
  #define TFT_CS              15
  #define TFT_DC               2
  #define TFT_RST             12
  #define TFT_BL              27    // Display backlight control pin for Walmart ESP32-2432S0 32
  #define TFT_BACKLIGHT_ON  HIGH    // HIGH or LOW are options
#endif  //DO_WALMART_DISPLAY

#if 0   //From original code, 11/15/23 Beck
  #define TFT_CS   PIN_D8  // Chip select control pin D8
  #define TFT_DC   PIN_D3  // Data Command control pin
  #define TFT_RST  PIN_D4  // Reset pin (could connect to NodeMCU RST, see next line)
  //#define TFT_RST  -1    // Set TFT_RST to -1 if the display RESET is connected to NodeMCU RST or 3.3V
#endif

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts


#define SMOOTH_FONT


// #define SPI_FREQUENCY  27000000
#define SPI_FREQUENCY  40000000
// #define SPI_FREQUENCY  80000000

#define SPI_READ_FREQUENCY  20000000

#define SPI_TOUCH_FREQUENCY  2500000


// #define SUPPORT_TRANSACTIONS
