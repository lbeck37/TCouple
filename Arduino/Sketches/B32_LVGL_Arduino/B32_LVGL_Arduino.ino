const char szSketchName[]  = "B32_LVGL_Arduino.ino";
const char szFileDate[]    = "12/27/23C";
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

#define DO_TOUCH    false

/*Change to your screen resolution*/
static const uint16_t   screenWidth  = 480;
static const uint16_t   screenHeight = 320;

//static lv_disp_draw_buf_t         draw_buf;
//static lv_disp_buf_t         draw_buf;
static lv_draw_buf_t         draw_buf;

static lv_color_t                 buf[ (screenWidth * screenHeight / 10)];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

//Function protos
void setup              (void);
void loop               (void);
void my_print           (lv_log_level_t   level       , const char *buf);
void my_disp_flush      (lv_disp_t        *disp       , const lv_area_t  *area, lv_color_t *color_p );
void my_touchpad_read   (lv_indev_t *     indev_driver, lv_indev_data_t  *data);

#if LV_USE_LOG != 0   //Serial debugging
  void my_print(lv_log_level_t level, const char * buf ){
    LV_UNUSED(level);
    Serial.flush();
    return;
  } //my_print
#endif  //LV_USE_LOG!=0

/* Display flushing */
void my_disp_flush(lv_disp_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  tft.startWrite();
  tft.setAddrWindow( area->x1, area->y1, w, h );
  //tft.pushColors((uint16_t *)&color_p->full, (w * h), true);
  tft.pushColors((uint16_t *)&color_p->full, (w * h), true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
  return;
} //my_disp_flush


void my_touchpad_read(lv_indev_t * indev_driver, lv_indev_data_t * data){ ///*Read the touchpad*/
  uint16_t touchX, touchY;

#if DO_TOUCH
  bool touched = tft.getTouch(&touchX, &touchY, 600);
#else
  bool touched= false;
#endif
  if(!touched){
      data->state = LV_INDEV_STATE_REL;
  } //if(!touched)
  else{
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

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

#if LV_USE_LOG != 0
  lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

  tft.begin();          /* TFT init */
  tft.setRotation( 3 ); /* Landscape orientation, flipped */

#if DO_TOUCH
  /*Set the touchscreen calibration data,
   the actual data for your display can be acquired using
   the Generic -> Touch_calibrate example from the TFT_eSPI library*/
  uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  tft.setTouch( calData );
#endif
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

  /*Initialize the display*/
  static lv_disp_t    disp_drv;

  lv_disp_drv_init( &disp_drv );

  /*Change the following line to your display resolution*/
  disp_drv.hor_res    = screenWidth;
  disp_drv.ver_res    = screenHeight;
  disp_drv.flush_cb   = my_disp_flush;
  disp_drv.draw_buf   = &draw_buf;
  lv_disp_drv_register( &disp_drv );

  /*Initialize the (dummy) input device driver*/
  static lv_indev_t     indev_drv;

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
  lv_demo_widgets();
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