const char szSketchName[]  = "B32_LVGL_Arduino.ino";
const char szFileDate[]    = "1/1/24X";

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Streaming.h>

#define DO_TOUCH      false
#define DO_LOGGING    false
#define BLOG          millis()

static const uint16_t         usPanelWidth  = 480;
static const uint16_t         usPanelHeight = 320;

const uint8_t                 ucRotation    = 3;     //DIYmall 3.5", Landscape, USB on the left


//Light Google Material Design colors
const lv_color_t              stColorWhite        = lv_color_hex(0xffffff);
const lv_color_t              stColorYellow200    = lv_color_hex(0xFFF59D);
const lv_color_t              stColorYellowA100   = lv_color_hex(0xFFFF8D);
const lv_color_t              stColorOrange200    = lv_color_hex(0xFFCC80);
const lv_color_t              stColorOrange600    = lv_color_hex(0xFB8C00);

//Dark Google Material Design colors
const lv_color_t              stColorBlack        = lv_color_hex(0x000000);
const lv_color_t              stColorRed900       = lv_color_hex(0xB71C1C);
const lv_color_t              stColorPurple900    = lv_color_hex(0x4A148C);
const lv_color_t              stColorIndigo900    = lv_color_hex(0x1A237E);
const lv_color_t              stColorTeal900      = lv_color_hex(0x004D40);
const lv_color_t              stColorGreen900     = lv_color_hex(0x1B5E20);
const lv_color_t              stColorBlueGray900  = lv_color_hex(0x263238);


//Darks tried: stColorRed900, stColorPurple900, stColorBlueGray900
//Lights tried:stColorWhite,stColorYellow100,stColorYellowA100,stColorOrange600,stColorGreen900
static lv_color_t             stDefaultTextColor  = stColorYellowA100;
static lv_color_t             stDefaultBGColor    = stColorBlack;

static lv_style_t             stMyStyle;
static lv_disp_drv_t          stDisplay;
static lv_indev_drv_t         stTouchPad;
static lv_disp_draw_buf_t     stDisplayBuffer;     //See src\hal\lv_hal_disp.h
static lv_color_t             stColorPixelsBuffer[(usPanelWidth * usPanelHeight / 10)];

TFT_eSPI                      TFTPanel      = TFT_eSPI(usPanelWidth, usPanelHeight);  //TFT_eSPI acts as display driver

//Function protos
void setup                    (void);
void loop                     (void);
static void CreateStyle       (void);
void SetupLVGL                (void);
void DisplayText              (const char *szText);
void MyDispFlush              (lv_disp_drv_t  *pstDispDrv , const lv_area_t *pstAreaCoords, lv_color_t *stPixels );
void my_touchpad_read         (lv_indev_drv_t *pstIndevDrv, lv_indev_data_t *pstIndevData);
#if DO_LOGGING
  void my_print               (lv_log_level_t level, const char *stColorPixelsBuffer);
#endif
void DisplayTFTHeading        (void);


void setup(){
  Serial.begin(115200);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << BLOG << " setup(): Call SetupLVGL()" << endl;
  SetupLVGL();

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void loop(){
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
  return;
} //loop


static void CreateStyle(void){
  //static lv_style_t     stMyStyle;
  Serial << BLOG << " CreateStyle(): Call lv_style_init(&stMyStyle)" << endl;
  lv_style_init                 (&stMyStyle);

  Serial << BLOG << " CreateStyle(): Call lv_obj_set_style_bg_color(lv_scr_act(),stDefaultBGColor,LV_PART_MAIN)" << endl;
  lv_obj_set_style_bg_color     (lv_scr_act(), stDefaultBGColor, LV_PART_MAIN);

  Serial << BLOG << " CreateStyle(): Call lv_style_set_text_font(&stMyStyle, &lv_font_montserrat_48)" << endl;
  lv_style_set_text_font        (&stMyStyle, &lv_font_montserrat_48);

  Serial << BLOG << " CreateStyle(): Call lv_obj_set_style_text_color(lv_scr_act(),stDefaultTextColor,LV_PART_MAIN)" << endl;
  lv_obj_set_style_text_color   (lv_scr_act(), stDefaultTextColor, LV_PART_MAIN);

  return;
} //CreateStyle


void SetupLVGL(void){
  Serial << BLOG << " SetupLVGL(): LVGL V" << lv_version_major() << "." << lv_version_minor() << "." << lv_version_patch() << endl;
  Serial << BLOG << " SetupLVGL(): Call lv_init()" << endl;
  lv_init();

#if DO_LOGGING && (LV_USE_LOG != 0)
  Serial << BLOG << " SetupLVGL(): Call lv_log_register_print_cb(my_print)" << endl;
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif  //DO_LOGGING&&(LV_USE_LOG!=0)
  Serial << BLOG << " SetupLVGL(): Call TFTPanel.init()" << endl;
  TFTPanel.init();

  Serial << BLOG << " SetupLVGL(): Call TFTPanel.setRotation(ucRotation)" << endl;
  TFTPanel.setRotation(ucRotation);

#if DO_TOUCH
  /*Set the touchscreen calibration data,
   the actual data for your display can be acquired using
   the Generic -> Touch_calibrate example from the TFT_eSPI library*/
  uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  Serial << BLOG << " SetupLVGL(): Call TFTPanel.setTouch(calData)" << endl;
  TFTPanel.setTouch(calData);
#endif

  Serial << BLOG << " SetupLVGL(): Call lv_disp_draw_buf_init(&stDisplayBuffer, stColorPixelsBuffer,...)" << endl;
  lv_disp_draw_buf_init(&stDisplayBuffer, stColorPixelsBuffer, NULL, usPanelWidth * usPanelHeight / 10 );

  /*Initialize the display*/
  //static lv_disp_drv_t    disp_drv;

  Serial << BLOG << " SetupLVGL(): Call lv_disp_drv_init(calData)" << endl;
  lv_disp_drv_init(&stDisplay);

  stDisplay.hor_res    = usPanelWidth;
  stDisplay.ver_res    = usPanelHeight;
  stDisplay.flush_cb   = MyDispFlush;
  stDisplay.draw_buf   = &stDisplayBuffer;

  Serial << BLOG << " SetupLVGL(): Call lv_disp_drv_register(&stDisplay)" << endl;
  lv_disp_drv_register(&stDisplay);

  /*Initialize the (dummy) input device driver*/
  //static lv_indev_drv_t     indev_drv;

  Serial << BLOG << " SetupLVGL(): Call lv_indev_drv_init(&stTouchPad)" << endl;
  lv_indev_drv_init(&stTouchPad);

  stTouchPad.type      = LV_INDEV_TYPE_POINTER;
  stTouchPad.read_cb   = my_touchpad_read;

  Serial << BLOG << " SetupLVGL(): Call lv_indev_drv_register(&stTouchPad)" << endl;
  lv_indev_drv_register(&stTouchPad);

  Serial << BLOG << " SetupLVGL(): Call CreateStyle()" << endl;
  CreateStyle();

  Serial << BLOG << " SetupLVGL(): Use lv_label_create(),lv_label_set_text(),lv_obj_align()" << endl;
  static lv_obj_t    *pstSketchNameLabel   = lv_label_create(lv_scr_act());
  static lv_obj_t    *pstFileDateLabel     = lv_label_create(lv_scr_act());
  static lv_obj_t    *pstCenterLabel       = lv_label_create(lv_scr_act());

  lv_label_set_text             (pstSketchNameLabel , szSketchName);
  lv_label_set_text             (pstFileDateLabel   , szFileDate);
  lv_label_set_text             (pstCenterLabel     , "Hello K&R, from The Dude");

  lv_obj_add_style              (pstSketchNameLabel, &stMyStyle, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_align                  (pstSketchNameLabel , LV_ALIGN_TOP_MID    , 0, 0);
  lv_obj_align                  (pstFileDateLabel   , LV_ALIGN_BOTTOM_MID , 0, 0);

  //lv_obj_set_style_local_text_font(pstCenterLabel, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_48);
  lv_obj_align                  (pstCenterLabel     , LV_ALIGN_CENTER     , 0, 0);

  Serial << BLOG << " SetupLVGL(): Done" << endl;
  return;
} //SetupLVGL


void DisplayText(const char *szText){
  Serial << BLOG << " DisplayText(): Begin" << endl;
  Serial << BLOG << " DisplayText(): Done" << endl;
  return;
} //DisplayText


void MyDispFlush(lv_disp_drv_t *pstDispDrv, const lv_area_t *pstAreaCoords, lv_color_t *stPixels){
  Serial << BLOG << " MyDispFlush(): Begin" << endl;
  uint32_t w = ((pstAreaCoords->x2 - pstAreaCoords->x1) + 1);
  uint32_t h = ((pstAreaCoords->y2 - pstAreaCoords->y1) + 1 );

  TFTPanel.startWrite    ();
  TFTPanel.setAddrWindow (pstAreaCoords->x1, pstAreaCoords->y1, w, h );
  TFTPanel.pushColors    ((uint16_t *)&stPixels->full, (w * h), true);
  TFTPanel.endWrite      ();

  lv_disp_flush_ready    (pstDispDrv);
  return;
} //MyDispFlush


void my_touchpad_read(lv_indev_drv_t *pstIndevDrv, lv_indev_data_t *pstIndevData){ ///*Read the touchpad*/
  uint16_t touchX, touchY;
#if DO_TOUCH
  bool touched = TFTPanel.getTouch(&touchX, &touchY, 600);
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


#if DO_LOGGING && (LV_USE_LOG != 0)
void my_print(lv_log_level_t level, const char *stColorPixelsBuffer){
  LV_UNUSED(level);
  Serial.flush();
  return;
} //my_print
#endif  //DO_LOGGING&&(LV_USE_LOG!=0)


void DisplayTFTHeading(void){
  //I think LVGL blanks the screen before writing so this won't stay on the screen
  Serial << BLOG << " DisplayTFTHeading(): Write Sketch name and date to panel using TFTPanel object calls" << endl;
  TFTPanel.setRotation     (ucRotation);
  TFTPanel.fillScreen      (TFT_BLACK);

  TFTPanel.setTextColor    (TFT_GREEN,TFT_BLACK);
  TFTPanel.setTextSize     (1);
  TFTPanel.setCursor       (0, 0);
  TFTPanel.setTextFont     (2);

  TFTPanel << szSketchName << " " << szFileDate << endl;
  //ShowMyMAC(true);
  return;
} //DisplayTFTHeading
//Last line.
