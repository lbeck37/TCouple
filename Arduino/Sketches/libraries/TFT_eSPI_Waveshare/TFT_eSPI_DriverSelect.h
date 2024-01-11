#pragma once
#pragma message("TFT_eSPI_WaveShare/TFT_eSPI_DriverSelect.h, 1/10/24c: Begin")

#ifdef TFT_PARALLEL_8_BIT
  //Waveshare 4.3",800x480,ESP32-S3, 8M PSRAM,4MB usable
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup50_SSD1963_Parallel.h")
  #include <User_Setups/Setup50_SSD1963_Parallel.h>
#endif

#ifdef B32_ILI9341
  //ElectroDragon ESP32 on 3.2" display with ILI9341 chip
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup1_ILI9341.h")
  #include <User_Setups/Setup1_ILI9341.h>
#endif

#ifdef B32_ILI9488_ESP32
  //DIYMall ESP32 on 3.5" with cap touch
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup21_ILI9488.h")
  #include <User_Setups/Setup21_ILI9488.h>
#endif

#ifdef B32_SSD1963_PARALLEL
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup50_SSD1963_Parallel.h")
  #include <User_Setups/Setup50_SSD1963_Parallel.h>
#endif

#ifdef B32_TTGO_T_DISPLAY
  //ESP32 TTGO T-Display ST7789V SPI controller
  #pragma message("TFT_eSPI_DriverSelect.h, Include User_Setups/Setup25_TTGO_T_Display.h")
  #include <User_Setups/Setup25_TTGO_T_Display.h>
#endif
//Last line.
