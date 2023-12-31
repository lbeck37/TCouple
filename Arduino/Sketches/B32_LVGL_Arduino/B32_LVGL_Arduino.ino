const char szSketchName[]  = "B32_LVGL_Arduino.ino";
const char szFileDate[]    = "12/30/23J";

#include <lvgl.h>
#include <TFT_eSPI.h>
//#include <B32_TCoupleLib.h>
#include <Streaming.h>

#define DO_TOUCH      false
#define DO_LOGGING    false

#define BLOG          millis()

const uint8_t                 ucRotation    = 3;     //DIYmall 3.5" touchscreen, Landscape, USB on the left

/*Change to your screen resolution*/
static const uint16_t         usScreenWidth  = 480;
static const uint16_t         usScreenHeight = 320;

static lv_disp_draw_buf_t     stDispDrawBuffer;     //See src\hal\lv_hal_disp.h

static lv_color_t             stColorPixelsBuffer[ (usScreenWidth * usScreenHeight / 10)];

TFT_eSPI TFTScreen = TFT_eSPI(usScreenWidth, usScreenHeight); /* TFT instance */

//Function protos
void setup              (void);
void DisplayTFTHeading  (void);
void SetupLVGL          (void);
void loop               (void);
void DisplayText        (const char *szText);
#if DO_LOGGING
  void my_print         (lv_log_level_t   level         , const char *stColorPixelsBuffer);
#endif
void MyDispFlush        (lv_disp_drv_t    *pstDispDrv     , const lv_area_t  *pstAreaCoords, lv_color_t *stPixels );
void my_touchpad_read   (lv_indev_drv_t   *pstIndevDrv    , lv_indev_data_t  *pstIndevData);


void setup(){
  Serial.begin( 115200 ); /* prepare for possible serial debug */
  Serial << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  //Set up LVGL graphics library
  SetupLVGL();

  DisplayTFTHeading();

  //Display a couple lines
  //DisplayText("Hello world, 12/30/23C");
  DisplayText(szFileDate);

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void DisplayTFTHeading(void){
  //TFTScreen.init            ();
  Serial << BLOG << " DisplayTFTHeading(): Write Sketch name and date to screen using TFTScreen calls" << endl;
  TFTScreen.setRotation     (ucRotation);
  TFTScreen.fillScreen      (TFT_BLACK);

  TFTScreen.setTextColor    (TFT_GREEN,TFT_BLACK);
  TFTScreen.setTextSize     (1);
  TFTScreen.setCursor       (0, 0);
  TFTScreen.setTextFont     (2);

  TFTScreen << szSketchName << " " << szFileDate << endl;
  //ShowMyMAC(true);

  return;
} //DisplayTFTHeading


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

  Serial << BLOG << " SetupLVGL(): Call TFTScreen.setRotation(ucRotation)" << endl;
  TFTScreen.setRotation(ucRotation);

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


void DisplayText(const char *szText){
  static lv_style_t     stMyStyle;
  lv_obj_t              *pstSketchNameLabel   = lv_label_create(lv_scr_act());
  lv_obj_t              *pstFileDateLabel     = lv_label_create(lv_scr_act());

  //Serial << BLOG << " DisplayText(): Display |"  << szText << "|" << endl;
  Serial << BLOG << " DisplayText(): Display Sketch name and date" << endl;

  Serial << BLOG << " DisplayText(): Call lv_style_init(&stMyStyle)" << endl;
  lv_style_init                 (&stMyStyle);

  Serial << BLOG << " DisplayText(): Call lv_style_set_text_font(&stMyStyle, &lv_font_montserrat_48)" << endl;
  lv_style_set_text_font        (&stMyStyle, &lv_font_montserrat_48);

  /*Create a label, set its text and align it to the center*/
/*
  Serial << BLOG << " DisplayText(): Call lv_label_create(lv_scr_act())" << endl;
  lv_obj_t *pstLabelObj= lv_label_create(lv_scr_act());
*/

  /*Change the active screen's background color*/
  Serial << BLOG << " DisplayText(): Call lv_obj_set_style_bg_color(lv_scr_act(),lv_color_hex(0x000000),LV_PART_MAIN)" << endl;
  //lv_obj_set_style_bg_color     (lv_scr_act(), lv_color_hex(0x003a57), LV_PART_MAIN);
  lv_obj_set_style_bg_color     (lv_scr_act(), lv_color_hex(0x000000), LV_PART_MAIN);

  Serial << BLOG << " DisplayText(): Call lv_obj_set_style_text_color(lv_scr_act(),lv_color_hex(0xffffff),LV_PART_MAIN)" << endl;
  lv_obj_set_style_text_color   (lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);

  Serial << BLOG << " DisplayText(): Call lv_label_set_text(pstLabelObj, szSketchName)" << endl;
  lv_label_set_text             (pstSketchNameLabel, szSketchName);

  Serial << BLOG << " DisplayText(): Call lv_label_set_text(pstFileDateLabel, szFileDate)" << endl;
  lv_label_set_text             (pstFileDateLabel, szFileDate);

  Serial << BLOG << " DisplayText(): Call lv_style_set_text_font(&stMyStyle, &lv_font_montserrat_28)" << endl;
  lv_style_set_text_font        (&stMyStyle, &lv_font_montserrat_28);

  Serial << BLOG << " DisplayText(): Call lv_obj_align(pstSketchNameLabel, LV_ALIGN_IN_TOP_MID,0,0)" << endl;
  //lv_obj_align                  (pstSketchNameLabel, LV_ALIGN_CENTER, 0, 0);
  lv_obj_align                  (pstSketchNameLabel, LV_ALIGN_TOP_MID, 0, 0);

  Serial << BLOG << " DisplayText(): Call lv_obj_align(pstFileDateLabel, LV_ALIGN_IN_BOTTOM_MID,0,0)" << endl;
  lv_obj_align                  (pstFileDateLabel, LV_ALIGN_BOTTOM_MID, 0, 0);

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
