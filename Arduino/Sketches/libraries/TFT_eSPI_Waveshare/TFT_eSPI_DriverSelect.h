//Beck, TFT_eSPI_DriverSelect.h, 1/8/24c
#pragma once
#pragma message("TFT_eSPI_DriverSelect.h, Begin")

#ifdef TFT_PARALLEL_8_BIT
  #pragma message("TFT_eSPI_DriverSelect.h, Driver is B32_WAVESHARE_8_BIT_PARALLEL, don't include anything")
/*
  #pragma message("TFT_eSPI_DriverSelect.h, #define TFT_PARALLEL_8_BIT and CONFIG_TFT_PARALLEL_8_BIT")
  #define TFT_PARALLEL_8_BIT
  #define CONFIG_TFT_PARALLEL_8_BIT       //Used in TFT_config.h
*/
#endif

#ifdef B32_ILI9341
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup1_ILI9341.h")
  #include <User_Setups/Setup1_ILI9341.h>  // ElectroDragon ESP32 on 3.2" display with ILI9341 chip
#endif

#ifdef B32_ILI9488_ESP32
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup21_ILI9488.h")
  #include <User_Setups/Setup21_ILI9488.h>  // DIYMall ESP32 on 3.5" display with capacitive touchscreen
#endif

#ifdef B32_SSD1963_PARALLEL
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup50_SSD1963_Parallel.h")
  #include <User_Setups/Setup50_SSD1963_Parallel.h>    //Not Waveshare 800x480 display w/ESP32-S3, 8/4MB PSRAM
#endif

#ifdef B32_TTGO_T_DISPLAY
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup25_TTGO_T_Display.h")
  #include <User_Setups/Setup25_TTGO_T_Display.h>    // Setup file for ESP32 and TTGO T-Display ST7789V SPI bus TFT
#endif
//Last line.
