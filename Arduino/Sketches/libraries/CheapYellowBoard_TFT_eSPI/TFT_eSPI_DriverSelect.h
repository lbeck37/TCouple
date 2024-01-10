//Larry Beck, TFT_eSPI_DriverSelect.h
#pragma once
#pragma message("TFT_eSPI_DriverSelect.h, 1/9/24c: Begin")

/*
#ifdef B32_ILI9341
  #pragma message("TFT_eSPI_DriverSelect.h, Include TFT_eSPI_DriverSelect.h, B32_ILI9341 is defined")
#else
  #pragma message("TFT_eSPI_DriverSelect.h, Include TFT_eSPI_DriverSelect.h, B32_ILI9341 is not defined")
#endif
#ifdef B32_TTGO_T_DISPLAY
  #pragma message("TFT_eSPI_DriverSelect.h, B32_TTGO_T_DISPLAY is defined")
#endif
*/

#ifdef B32_RAW_8BIT_PARALLEL    //1/9/24 I think this has changed
  //Waveshare 4.3",800x480,ESP32-S3, 8M PSRAM,4MB usable
  #pragma message("TFT_eSPI_DriverSelect.h, Driver is B32_RAW_8BIT_PARALLEL, don't include anything")
#endif

#ifdef B32_ILI9341
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup1_ILI9341.h")
  #include <User_Setups/Setup1_ILI9341.h>  // ElectroDragon ESP32 on 3.2" display with ILI9341 chip
  //#include <CheapYellowBoard_TFT_eSPI/User_Setups/Setup1_ILI9341.h> //ElectroDragon 3.2",320x240,ILI9341,no touch
#endif

#ifdef B32_ILI9488_ESP32
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup21_ILI9488.h")
  #include <User_Setups/Setup21_ILI9488.h>  // DIYMall ESP32 on 3.5" display with capacitive touchscreen
  //#include <CheapYellowBoard_TFT_eSPI/User_Setups/Setup21_ILI9488.h> //CheapYellowDisplay,DIYMall 3.5",480x320,ILI9488,cap touch
#endif

#ifdef B32_SSD1963_PARALLEL
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup50_SSD1963_Parallel.h")
  #include <User_Setups/Setup50_SSD1963_Parallel.h>
#endif

#ifdef B32_TTGO_T_DISPLAY
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup25_TTGO_T_Display.h")
  #include <User_Setups/Setup25_TTGO_T_Display.h>
  //#include <CheapYellowBoard_TFT_eSPI/User_Setups/Setup25_TTGO_T_Display.h> //ESP32 TTGO T-Display ST7789V SPI controller
#endif
//Last line.
