//Beck, SquarelineSetup.cpp 2/22/24b
//_Repos\TCouple\Arduino\Sketches\libraries\B32_LVGL_DriverLib\
Arduino_ESP32RGBPanel   *pRGBPanel;
Arduino_RGB_Display     *pDisplay;

lv_color_t              *pDisplayBuffer;
lv_disp_drv_t           stDisplayDriver;
lv_disp_draw_buf_t      stDrawBuffer;
lv_indev_drv_t          stInputDeviceDriver;

void FlushDataToDisplay(lv_disp_drv_t *pDisplayDriver,
                        const lv_area_t *pArea, lv_color_t *color_p){
  pDisplay->draw16bitBeRGBBitmap();
  lv_disp_flush_ready(pDisplayDriver);
}   //FlushDataToDisplay

SetupDisplay(){
  pRGBPanel = new Arduino_ESP32RGBPanel(pins,...);
  pDisplay  = new Arduino_RGB_Display(Width, Height, pRGBPanel);
  pDisplay->begin();
}   //SetupDisplay

SetupLVGL(){
  lv_init();
  pDisplayBuffer= heap_caps_malloc();
  lv_disp_draw_buf_init     (&stDrawBuffer, pDisplayBuffer, NULL, (sScreenWidth * 40));

  lv_disp_drv_init(&stDisplayDriver);
  stDisplayDriver.flush_cb     = FlushDataToDisplay;
  stDisplayDriver.draw_buf     = &stDrawBuffer;
  lv_disp_drv_register(&stDisplayDriver);

  lv_indev_drv_init         (&stInputDeviceDriver);
  lv_indev_drv_register     (&stInputDeviceDriver);
}   //SetupLVGL


//C:\_Repos\TCouple\SquareLine\MyProjects\ThermoArduino\
//   Template_Export\ThermoArduino\ui\ui.ino
lv_disp_draw_buf_t      draw_buf;                   //My stDrawBuffer
lv_color_t              buf[Width*Height / 10];     //My pDisplayBuffer
lv_disp_drv_t           disp_drv;
lv_indev_drv_t          indev_drv;

my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p ){
  tft.pushColors(&color_p->full,...);
  lv_disp_flush_ready(disp);
}   //my_disp_flush

setup(){
  lv_init();
  lv_disp_draw_buf_init     (&draw_buf, buf, NULL, (Width*Height / 10));
  lv_disp_drv_init          (&disp_drv);
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register      (&disp_drv);

  lv_indev_drv_init         (&indev_drv);
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register     (&indev_drv);
  ui_init();
}   //setup