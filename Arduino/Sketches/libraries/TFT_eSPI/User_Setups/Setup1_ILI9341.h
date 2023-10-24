// Setup1_ILI9341 Beck 10/24/23f
//Beck BADCODE put in to fail compile if this file is even used!

// See SetupX_Template.h for all options available
#define ILI9341_DRIVER

#if 0
	//Original from 10/24/23
	#define TFT_CS   PIN_D8  // Chip select control pin D8
	#define TFT_DC   PIN_D3  // Data Command control pin
	#define TFT_RST  PIN_D4  // Reset pin (could connect to NodeMCU RST, see next line)
	//#define TFT_RST  -1    // Set TFT_RST to -1 if the display RESET is connected to NodeMCU RST or 3.3V
#endif

#if 1
//From ElectroDragon Wiki "ESP-LCD32
	#define TFT_MOSI            15
	//#define TFT_MOSI            14
	#define TFT_SCLK            18
	#define TFT_CS              27
	#define TFT_DC              32
	#define TFT_RST              5
	//#define TFT_BL          	 4  	// Display backlight control pin
	#define TFT_BL          	32  	// Display backlight control pin
	//#define TFT_BACKLIGHT_ON 	HIGH  	// HIGH or LOW are options
	#define TFT_BACKLIGHT_ON 	LOW  	// HIGH or LOW are options
#endif

#if 0
//From https://forum.arduino.cc/t/esp32-2432s028r-all-in-one-display-touch-spi-problems/1059746
	#define TFT_MOSI            13
	#define TFT_SCLK            14
	#define TFT_CS              15
	#define TFT_DC               2
	#define TFT_RST             12
	#define TFT_BL          	 4  	// Display backlight control pin
	#define TFT_BACKLIGHT_ON 	HIGH  	// HIGH or LOW are options
#endif

#if 0
//From https://github.com/mstrens/grbl_controller_esp32
	#define TFT_MOSI            23
	#define TFT_SCLK            18
	#define TFT_CS              13
	#define TFT_DC              14
	#define TFT_RST             12
	#define TFT_BL          	 4  	// Display backlight control pin
	#define TFT_BACKLIGHT_ON 	HIGH  	// HIGH or LOW are options
#endif

#if 0
//From User_setup.h for M5 Stack
	#define TFT_MOSI            23
	#define TFT_SCLK            18
	#define TFT_CS              14
	#define TFT_DC              27
	#define TFT_RST             33
	#define TFT_BL          	32  	// Display backlight control pin
	#define TFT_BACKLIGHT_ON 	HIGH  	// HIGH or LOW are options
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
