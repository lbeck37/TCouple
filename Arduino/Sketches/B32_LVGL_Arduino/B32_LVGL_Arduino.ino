const char szSketchName[]  = "B32_LVGL_Arduino.ino";
const char szFileDate[]    = "12/29/23D";
/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Streaming.h>

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 *Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 *as the examples and demos are now part of the main LVGL library. */
//#include <examples/lv_examples.h>
//#include <demos/lv_demos.h>

#define DO_TOUCH      false
#define DO_LOGGING    false

/*Change to your screen resolution*/
static const uint16_t         usScreenWidth  = 480;
static const uint16_t         usScreenHeight = 320;

static lv_disp_draw_buf_t     stDispDrawBuffer;     //See src\hal\lv_hal_disp.h
//static lv_disp_buf_t         draw_buf;
//static lv_draw_buf_t         draw_buf;

static lv_color_t             stColorPixelsBuffer[ (usScreenWidth * usScreenHeight / 10)];

TFT_eSPI TFTScreen = TFT_eSPI(usScreenWidth, usScreenHeight); /* TFT instance */

//Function protos
void setup              (void);
void loop               (void);
//void my_print           (lv_log_level_t   level         , const char *stColorPixelsBuffer);
void MyDispFlush        (lv_disp_drv_t    *pstDispDrv     , const lv_area_t  *pstAreaCoords, lv_color_t *stPixels );
void my_touchpad_read   (lv_indev_drv_t   *pstIndevDrv    , lv_indev_data_t  *pstIndevData);

#if DO_LOGGING
  #if LV_USE_LOG != 0   //Serial debugging
    void my_print(lv_log_level_t level, const char *stColorPixelsBuffer){
      LV_UNUSED(level);
      Serial.flush();
      return;
    } //my_print
  #endif  //LV_USE_LOG!=0
#endif
/* Display flushing */
//void my_disp_flush(lv_disp_t *disp, const lv_area_t *area, lv_color_t *color_p)
void MyDispFlush(lv_disp_drv_t *pstDispDrv, const lv_area_t *pstAreaCoords, lv_color_t *stPixels){
  uint32_t w = ((pstAreaCoords->x2 - pstAreaCoords->x1) + 1);
  uint32_t h = ((pstAreaCoords->y2 - pstAreaCoords->y1) + 1 );

  TFTScreen.startWrite();
  TFTScreen.setAddrWindow(pstAreaCoords->x1, pstAreaCoords->y1, w, h );
  //tft.pushColors((uint16_t *)&color_p->full, (w * h), true);
  TFTScreen.pushColors((uint16_t *)&stPixels->full, (w * h), true);
  TFTScreen.endWrite();

  lv_disp_flush_ready(pstDispDrv);
  return;
} //MyDispFlush


//void my_touchpad_read(lv_indev_t * indev_driver, lv_indev_data_t * data){ ///*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *pstIndevDrv, lv_indev_data_t *pstIndevData){ ///*Read the touchpad*/
  uint16_t touchX, touchY;

#if DO_TOUCH
  bool touched = TFTScreen.getTouch(&touchX, &touchY, 600);
#else
  bool touched= false;
#endif
  if(!touched){
      pstIndevData->state = LV_INDEV_STATE_REL;
  } //if(!touched)
  else{
    pstIndevData->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    pstIndevData->point.x = touchX;
    pstIndevData->point.y = touchY;

    Serial.print( "Data x " );
    Serial.println( touchX );

    Serial.print( "Data y " );
    Serial.println( touchY );
  } //if(!touched)else
  return;
} //my_touchpad_read


void setup(){
  Serial.begin( 115200 ); /* prepare for possible serial debug */
  Serial << endl << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println( LVGL_Arduino );
  Serial.println( "I am LVGL_Arduino" );

  lv_init();

#if DO_LOGGING
  #if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
  #endif
#endif
  TFTScreen.begin();          /* TFT init */
  TFTScreen.setRotation( 3 ); /* Landscape orientation, flipped */

#if DO_TOUCH
  /*Set the touchscreen calibration data,
   the actual data for your display can be acquired using
   the Generic -> Touch_calibrate example from the TFT_eSPI library*/
  uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  TFTScreen.setTouch( calData );
#endif
  lv_disp_draw_buf_init( &stDispDrawBuffer, stColorPixelsBuffer, NULL, usScreenWidth * usScreenHeight / 10 );

  /*Initialize the display*/
  //static lv_disp_t    disp_drv;
  static lv_disp_drv_t    disp_drv;

  lv_disp_drv_init( &disp_drv );

  /*Change the following line to your display resolution*/
  disp_drv.hor_res    = usScreenWidth;
  disp_drv.ver_res    = usScreenHeight;
  disp_drv.flush_cb   = MyDispFlush;
  disp_drv.draw_buf   = &stDispDrawBuffer;
  lv_disp_drv_register( &disp_drv );

  /*Initialize the (dummy) input device driver*/
  //static lv_indev_t     indev_drv;
  static lv_indev_drv_t     indev_drv;

  lv_indev_drv_init(&indev_drv);
  indev_drv.type      = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb   = my_touchpad_read;
  lv_indev_drv_register( &indev_drv );

  /* Create simple label */
  //lv_obj_t *label = lv_label_create( lv_scr_act() );
  //lv_label_set_text( label, LVGL_Arduino.c_str() );
  //lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

  /* Try an example. See all the examples
   * online: https://docs.lvgl.io/master/examples.html
   * source codes: https://github.com/lvgl/lvgl/tree/e7f88efa5853128bf871dde335c0ca8da9eb7731/examples */
   //lv_example_btn_1();

   /*Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMOS_WIDGETS*/
  //lv_demo_widgets();
  // lv_demo_benchmark();
  // lv_demo_keypad_encoder();
  // lv_demo_music();
  // lv_demo_printer();
  // lv_demo_stress();

  Serial.println( "Setup done" );
  return;
} //setup


void loop(){
  lv_timer_handler(); /* let the GUI do its work */
  delay( 5 );
  return;
} //loop
//Last line.
