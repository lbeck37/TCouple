const char szSketchName[]  = "B32_LVGL_Waveshare.ino";
const char szFileDate[]    = "1/10/24n";
#pragma message("B32_LVGL_Arduino.ino, Begin compile")

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <B32_LVGL_GraphicsLib.h>
#include <Streaming.h>

#define DO_TOUCH      false
#define DO_LOGGING    false
#define BLOG          millis()

static const uint16_t         usPanelWidth  = B32_TFT_WIDTH;
static const uint16_t         usPanelHeight = B32_TFT_HEIGHT;

const uint8_t                 ucRotation    = 3;     //DIYmall 3.5", Landscape, USB on the left

static lv_disp_drv_t          stDisplay;
static lv_indev_drv_t         stTouchPad;
static lv_disp_draw_buf_t     stDisplayBuffer;     //See src\hal\lv_hal_disp.h

static lv_color_t             stColorPixelsBuffer[(usPanelWidth * usPanelHeight / 10)];

TFT_eSPI                      TFTPanel      = TFT_eSPI(usPanelWidth, usPanelHeight);  //TFT_eSPI is display driver

//Function protos
void setup                    (void);
void loop                     (void);
void PrintRAMSizes            (void);
void SetupStyles              (void);
void SetupTextStyle           (lv_style_t *pTextStyle, const lv_font_t *pTextFont, lv_color_t stTextColor);
void DisplayText              (void);
void SetupLVGL                (void);
void SetupTouchscreen         (void);
void SetupLogging             (void);
void MyDispFlush              (lv_disp_drv_t  *pstDispDrv, const lv_area_t *pstArea, lv_color_t *stPixels);
void my_touchpad_read         (lv_indev_drv_t *pstIndevDrv, lv_indev_data_t *pstIndevData);
void my_print                 (lv_log_level_t cLevel, const char *stColorPixelsBuffer);


void setup(){
  Serial.begin(115200);
  delay(500);
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;
  Serial << endl << BLOG << " setup(): Sketch: Print RAM sizes at beginning of Sketch" << endl;
  PrintRAMSizes();

#if true
  Serial << BLOG << " setup():Call SetupLVGL(),SetupLogging,SetupLogging,SetupStyles" << endl;
  SetupLVGL();
  SetupTouchscreen();
  SetupLogging();
  SetupStyles();

  DisplayText();
#endif
  Serial << endl << BLOG << " setup(): Sketch: Print RAM sizes at end of Sketch" << endl;
  PrintRAMSizes();
  Serial << BLOG << " setup(): " << szSketchName << ", " << szFileDate << ", is done." << endl;
  return;
} //setup

void loop(){
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
  return;
} //loop

void PrintRAMSizes(){
  int wTotalHeapBytes= ESP.getHeapSize();
  int wFreeHeapBytes = ESP.getFreeHeap();
  int wUsedHeapBytes = (wTotalHeapBytes - wFreeHeapBytes);

  int wTotalPSRAMBytes= ESP.getPsramSize();
  int wFreePSRAMBytes = ESP.getFreePsram();
  int wUsedPSRAMBytes = (wTotalPSRAMBytes - wFreePSRAMBytes);

  Serial << BLOG << " PrintRAMSizes(): Total Heap  Bytes= " << wTotalHeapBytes << endl;
  Serial << BLOG << " PrintRAMSizes(): Free Heap   Bytes= " << wFreeHeapBytes  << endl;
  Serial << BLOG << " PrintRAMSizes(): Used Heap   Bytes= " << wUsedHeapBytes  << endl;

  Serial << BLOG << " PrintRAMSizes(): Total PSRAM Bytes= " << wTotalPSRAMBytes << endl;
  Serial << BLOG << " PrintRAMSizes(): Free PSRAM  Bytes= " << wFreePSRAMBytes  << endl;
  Serial << BLOG << " PrintRAMSizes(): Used PSRAM  Bytes= " << wUsedPSRAMBytes  << endl;
  return;
} //PrintRAMSizes


void SetupStyles(void){
  SetupTextStyle  (&stTinyTextStyle    , pTinyTextFont    , stTinyTextColor);
  SetupTextStyle  (&stTitleTextStyle   , pTitleTextFont   , stTitleTextColor);
  SetupTextStyle  (&stDataTextStyle    , pDataTextFont    , stDataTextColor);
  SetupTextStyle  (&stLocationTextStyle, pLocationTextFont, stLocationTextColor);
  return;
} //SetupStyles


void SetupTextStyle(lv_style_t *pTextStyle, const lv_font_t *pTextFont, lv_color_t stTextColor){
  Serial << BLOG << " SetupTextStyle(): Call lv_style_init(pTextStyle)" << endl;
  lv_style_init                 (pTextStyle);

  Serial << BLOG << " SetupTextStyle(): Call lv_style_set_text_font(pTextStyle,pTextFont)" << endl;
  lv_style_set_text_font        (pTextStyle, pTextFont);

  Serial << BLOG << " SetupTextStyle(): Call lv_style_set__text_color(pTextStyle,stTextColor)" << endl;
  lv_style_set_text_color       (pTextStyle, stTextColor);

  return;
} //SetupTextStyle


void DisplayText(void){
  Serial << BLOG << " DisplayText(): Begin" << endl;
  static lv_obj_t     *pstSketchNameLabel   = lv_label_create(lv_scr_act());
  static lv_obj_t     *pstFileDateLabel     = lv_label_create(lv_scr_act());
  static lv_obj_t     *pstLocationLabel     = lv_label_create(lv_scr_act());
  static lv_obj_t     *pstDataLabel         = lv_label_create(lv_scr_act());

  lv_label_set_text   (pstSketchNameLabel, szSketchName);
  lv_label_set_text   (pstFileDateLabel  , szFileDate);
  lv_label_set_text   (pstLocationLabel  , "Left");
  lv_label_set_text   (pstDataLabel      , "Right");

  lv_obj_add_style    (pstSketchNameLabel, &stTitleTextStyle   , LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style    (pstFileDateLabel  , &stTinyTextStyle    , LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style    (pstLocationLabel  , &stLocationTextStyle, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_style    (pstDataLabel      , &stDataTextStyle    , LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_align        (pstSketchNameLabel, LV_ALIGN_TOP_MID   , 0, 0);
  lv_obj_align        (pstFileDateLabel  , LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_align        (pstLocationLabel  , LV_ALIGN_LEFT_MID  , 0, 0);
  lv_obj_align        (pstDataLabel      , LV_ALIGN_RIGHT_MID , 0, 0);

  Serial << BLOG << " DisplayText(): Done" << endl;
  return;
} //DisplayText


void SetupLVGL(void){
  Serial << BLOG << " SetupLVGL(): LVGL V" << lv_version_major() << "." << lv_version_minor() << "." << lv_version_patch() << endl;
  Serial << BLOG << " SetupLVGL(): Call lv_init()" << endl;
  lv_init();

#if false
  Serial << BLOG << " SetupLVGL(): Call TFTPanel.init()" << endl;
  TFTPanel.init();

  Serial << BLOG << " SetupLVGL(): Call TFTPanel.setRotation(ucRotation)" << endl;
  TFTPanel.setRotation(ucRotation);
#endif

  Serial << BLOG << " SetupLVGL(): Call lv_disp_draw_buf_init(&stDisplayBuffer, stColorPixelsBuffer,...)" << endl;
  lv_disp_draw_buf_init(&stDisplayBuffer, stColorPixelsBuffer, NULL, usPanelWidth * usPanelHeight / 10 );

  Serial << BLOG << " SetupLVGL(): Call lv_disp_drv_init(calData)" << endl;
  lv_disp_drv_init(&stDisplay);

  stDisplay.hor_res    = usPanelWidth;
  stDisplay.ver_res    = usPanelHeight;
  stDisplay.flush_cb   = MyDispFlush;
  stDisplay.draw_buf   = &stDisplayBuffer;

  Serial << BLOG << " SetupLVGL(): Call lv_disp_drv_register(&stDisplay)" << endl;
  lv_disp_drv_register(&stDisplay);

  Serial << BLOG << " SetupLVGL(): Call lv_obj_set_style_bg_color(lv_scr_act(),stDefaultBGColor,LV_PART_MAIN)" << endl;
  lv_obj_set_style_bg_color     (lv_scr_act(), stDefaultBGColor, LV_PART_MAIN);

  Serial << BLOG << " SetupLVGL(): Done" << endl;
  return;
} //SetupLVGL


void SetupTouchscreen(void){
#if DO_TOUCH
  uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  Serial << BLOG << " SetupTouchscreen(): Call TFTPanel.setTouch(calData)" << endl;
  TFTPanel.setTouch(calData);
#endif
  /*Initialize the (dummy) input device driver*/
  //static lv_indev_drv_t     indev_drv;
  Serial << BLOG << " SetupTouchscreen(): Call lv_indev_drv_init(&stTouchPad)" << endl;
  lv_indev_drv_init(&stTouchPad);

  stTouchPad.type      = LV_INDEV_TYPE_POINTER;
  stTouchPad.read_cb   = my_touchpad_read;

  Serial << BLOG << " SetupTouchscreen(): Call lv_indev_drv_register(&stTouchPad)" << endl;
  lv_indev_drv_register(&stTouchPad);
  return;
} //SetupTouchscreen


void SetupLogging(void){
#if DO_LOGGING && (LV_USE_LOG != 0)
  Serial << BLOG << " SetupTouchscreen(): Call lv_log_register_print_cb(my_print)" << endl;
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif  //DO_LOGGING&&(LV_USE_LOG!=0)
  return;
} //SetupLogging

void MyDispFlush(lv_disp_drv_t *pstDispDrv, const lv_area_t *pstArea, lv_color_t *stPixels){
  Serial << BLOG << " MyDispFlush(): Begin" << endl;
  uint32_t w = ((pstArea->x2 - pstArea->x1) + 1);
  uint32_t h = ((pstArea->y2 - pstArea->y1) + 1 );

  TFTPanel.startWrite    ();
  TFTPanel.setAddrWindow (pstArea->x1, pstArea->y1, w, h );
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

    pstIndevData->point.x = touchX;
    pstIndevData->point.y = touchY;

    Serial.print( "Data x " );
    Serial.println( touchX );
    Serial.print( "Data y " );
    Serial.println( touchY );
  } //if(!touched)else
  return;
} //my_touchpad_read

void my_print(lv_log_level_t cLevel, const char *stColorPixelsBuffer){
  LV_UNUSED(cLevel);
  Serial.flush();
  return;
} //my_print
//Last line.
