//Beck, Setup1_ILI9341.h, 12/20/23c
// See SetupX_Template.h for all options available
#pragma once
#define USER_SETUP_ID           1
#pragma message("Setup1_ILI9341.h, Begin")

#define ILI9341_DRIVER

#define DO_WALMART_DISPLAY      true
#define DO_ESP32_DISPLAY        false

//If colors are inverted (white shows as black) then use one of these
//#define   TFT_INVERSION_ON
//#define   TFT_INVERSION_OFF

//If the blue & red are swapped on the display, then use one of these
//#define   TFT_RGB_ORDER     TFT_RGB     // Color order Red-Green-Blue
//#define   TFT_RGB_ORDER     TFT_BGR     // Color order Blue-Green-Red

#if DO_ESP32_DISPLAY
//From ElectroDragon Wiki "ESP-LCD32
  #define TFT_MOSI            23
  #define TFT_SCLK            18
  #define TFT_CS              27
  #define TFT_DC              32
  #define TFT_RST              5
  #define TFT_BL              19    // Display backlight control pin, not sure if this even works
  #define TFT_BACKLIGHT_ON    LOW   // HIGH or LOW are options
#endif  //DO_ESP32_DISPLAY

#if DO_WALMART_DISPLAY
  #pragma message("Setup1_ILI9341.h, defining SPI pins and display characteristics")
  #define TFT_WIDTH          320
  #define TFT_HEIGHT         240
  #define TFT_MOSI            13
  #define TFT_SCLK            14
  #define TFT_CS              15
  #define TFT_DC               2
  #define TFT_RST             12
  #define TFT_BL              27          // Display backlight control pin for Walmart ESP32-2432S0 32
  #define TFT_BACKLIGHT_ON    HIGH        // HIGH or LOW are options
  #define TFT_RGB_ORDER       TFT_RGB     // Color order Blue-Green-Red
  #define TFT_INVERSION_ON
#endif  //DO_WALMART_DISPLAY

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

#define SPI_FREQUENCY           40000000
#define SPI_READ_FREQUENCY      20000000
#define SPI_TOUCH_FREQUENCY      2500000
//Last line.
