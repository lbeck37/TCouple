//Beck, SquarelineSetup.cpp 2/22/24
//My B32_LVGL_DriverLib.cpp code

Arduino_ESP32RGBPanel   *pRGBPanel;
Arduino_RGB_Display     *pDisplay;

lv_color_t              *pDisplayBuffer;
lv_disp_drv_t           stDisplayDriver;
lv_disp_draw_buf_t      stDrawBuffer;

void FlushDataToDisplay(lv_disp_drv_t *pDisplayDriver, const lv_area_t *pArea, lv_color_t *color_p){
  pDisplay->draw16bitBeRGBBitmap();
  lv_disp_flush_ready(pDisplayDriver);
}   //FlushDataToDisplay

SetupDisplay(){
  pRGBPanel = new Arduino_ESP32RGBPanel(pins,...);
  pDisplay  = new Arduino_RGB_Display(Width, Height, pRGBPanel);
  pDisplay->begin();
}   //SetupDisplay

C:\_Repos\TCouple\SquareLine\MyProjects\ThermoArduino\Template_Export\ThermoArduino\ui\ui.ino

lv_disp_draw_buf_t      draw_buf;                   //My stDrawBuffer
lv_color_t              buf[Width*Height / 10];     //My pDisplayBuffer
