//Beck, TFT_eSPI_DriverSelect.h, 12/18/23c
#pragma once
  #pragma message("TFT_eSPI_DriverSelect.h, Begin")

#ifdef B32_ILI9341
  #pragma message("TFT_eSPI_DriverSelect.h, B32_ILI9341 is defined")
#endif

#ifdef B32_TTGO_T_DISPLAY
  #pragma message("TFT_eSPI_DriverSelect.h, B32_TTGO_T_DISPLAY is defined")
#endif

#if true
  #pragma message("TFT_eSPI_DriverSelect.h, including User_Setups/Setup1_ILI9341.h")
  #include <User_Setups/Setup1_ILI9341.h>  // ElectroDragon ESP32 on 3.2" display with ILI9341 chip
#endif

#ifdef B32_TTGO_T_DISPLAY
  #include <User_Setups/Setup25_TTGO_T_Display.h>    // Setup file for ESP32 and TTGO T-Display ST7789V SPI bus TFT
#endif
//Last line.
