const char szSketchName[]  = "B32_LVGL_Arduino.ino";
const char szFileDate[]    = "12/29/23K";

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Streaming.h>

#define DO_TOUCH      false
#define DO_LOGGING    false

#define BLOG          millis()

/*Change to your screen resolution*/
static const uint16_t         usScreenWidth  = 480;
static const uint16_t         usScreenHeight = 320;

static lv_disp_draw_buf_t     stDispDrawBuffer;     //See src\hal\lv_hal_disp.h

static lv_color_t             stColorPixelsBuffer[ (usScreenWidth * usScreenHeight / 10)];

TFT_eSPI TFTScreen = TFT_eSPI(usScreenWidth, usScreenHeight); /* TFT instance */

//Function protos
void setup              (void);
void SetupLVGL          (void);
void loop               (void);
void DisplayText        (void);
#if DO_LOGGING
  void my_print         (lv_log_level_t   level         , const char *stColorPixelsBuffer);
#endif
void MyDispFlush        (lv_disp_drv_t    *pstDispDrv     , const lv_area_t  *pstAreaCoords, lv_color_t *stPixels );
void my_touchpad_read   (lv_indev_drv_t   *pstIndevDrv    , lv_indev_data_t  *pstIndevData);


void setup(){
  Serial.begin( 115200 ); /* prepare for possible serial debug */
  Serial << endl << BLOG << "setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  //Set up LVGL graphics library
  SetupLVGL();

  /* Create simple label */
  DisplayText();

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void SetupLVGL(void){
  Serial << BLOG << " SetupLVGL(): LVGL V" << lv_version_major() << "." << lv_version_minor() << "." << lv_version_patch() << endl;
  Serial << BLOG << " SetupLVGL(): Call lv_init()" << endl;
  lv_init();

#if DO_LOGGING
  #if LV_USE_LOG != 0
    Serial << BLOG << " SetupLVGL(): Call lv_log_register_print_cb(my_print)" << endl;
    lv_log_register_print_cb(my_print); /* register print function for debugging */
  #endif
#endif
  Serial << BLOG << " SetupLVGL(): Call TFTScreen.begin()" << endl;
  TFTScreen.begin();

  Serial << BLOG << " SetupLVGL(): Call TFTScreen.setRotation(3)" << endl;
  TFTScreen.setRotation(3); /*DIYMall 3.5" touchscreen, Landscape, USB on the left */

#if DO_TOUCH
  /*Set the touchscreen calibration data,
   the actual data for your display can be acquired using
   the Generic -> Touch_calibrate example from the TFT_eSPI library*/
  uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  Serial << BLOG << " SetupLVGL(): Call TFTScreen.setTouch(calData)" << endl;
  TFTScreen.setTouch(calData);
#endif

  Serial << BLOG << " SetupLVGL(): Call lv_disp_draw_buf_init(&stDispDrawBuffer, stColorPixelsBuffer,...)" << endl;
  lv_disp_draw_buf_init(&stDispDrawBuffer, stColorPixelsBuffer, NULL, usScreenWidth * usScreenHeight / 10 );

  /*Initialize the display*/
  static lv_disp_drv_t    disp_drv;

  Serial << BLOG << " SetupLVGL(): Call lv_disp_drv_init(calData)" << endl;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res    = usScreenWidth;
  disp_drv.ver_res    = usScreenHeight;
  disp_drv.flush_cb   = MyDispFlush;
  disp_drv.draw_buf   = &stDispDrawBuffer;

  Serial << BLOG << " SetupLVGL(): Call lv_disp_drv_register(&disp_drv)" << endl;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t     indev_drv;

  Serial << BLOG << " SetupLVGL(): Call lv_indev_drv_init(&indev_drv)" << endl;
  lv_indev_drv_init(&indev_drv);

  indev_drv.type      = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb   = my_touchpad_read;

  Serial << BLOG << " SetupLVGL(): Call lv_indev_drv_register(&indev_drv)" << endl;
  lv_indev_drv_register(&indev_drv);

  Serial << BLOG << " SetupLVGL(): Done" << endl;
  return;
} //SetupLVGL


void loop(){
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
  return;
} //loop


void DisplayText(void){
  Serial << BLOG << " DisplayText(): Begin" << endl;

  /*Create a white label, set its text and align it to the center*/
  Serial << BLOG << " DisplayText(): Call lv_label_create(lv_scr_act())" << endl;
  lv_obj_t *pstLabelObj= lv_label_create(lv_scr_act());

  /*Change the active screen's background color*/
  Serial << BLOG << " DisplayText(): Call lv_obj_set_style_bg_color(lv_scr_act(),...)" << endl;
  lv_obj_set_style_bg_color     (lv_scr_act(), lv_color_hex(0x003a57), LV_PART_MAIN);

  Serial << BLOG << " DisplayText(): Call lv_obj_set_style_text_color(lv_scr_act(),...)" << endl;
  lv_obj_set_style_text_color   (lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);

  Serial << BLOG << " DisplayText(): Call lv_label_set_text(pstLabelObj, Hello world...)" << endl;
  lv_label_set_text             (pstLabelObj, "Hello world, 12/29/23BB");

  Serial << BLOG << " DisplayText(): Call lv_obj_align(pstLabelObj, LV_ALIGN_CENTER,...)" << endl;
  lv_obj_align                  (pstLabelObj, LV_ALIGN_CENTER, 0, 0);

  Serial << BLOG << " DisplayText(): Done" << endl;
  return;
} //DisplayText


#if DO_LOGGING
  #if LV_USE_LOG != 0   //Serial debugging
    void my_print(lv_log_level_t level, const char *stColorPixelsBuffer){
      LV_UNUSED(level);
      Serial.flush();
      return;
    } //my_print
  #endif  //LV_USE_LOG!=0
#endif


void MyDispFlush(lv_disp_drv_t *pstDispDrv, const lv_area_t *pstAreaCoords, lv_color_t *stPixels){
  Serial << BLOG << " MyDispFlush(): Begin" << endl;
  uint32_t w = ((pstAreaCoords->x2 - pstAreaCoords->x1) + 1);
  uint32_t h = ((pstAreaCoords->y2 - pstAreaCoords->y1) + 1 );

  TFTScreen.startWrite    ();
  TFTScreen.setAddrWindow (pstAreaCoords->x1, pstAreaCoords->y1, w, h );
  TFTScreen.pushColors    ((uint16_t *)&stPixels->full, (w * h), true);
  TFTScreen.endWrite      ();

  lv_disp_flush_ready     (pstDispDrv);
  return;
} //MyDispFlush


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
//Last line.
