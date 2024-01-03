const char szSketchName[]  = "B32_LVGL_TCoupleDisplay.ino";
const char szFileDate[]    = "1/2/24C";

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <B32_TCoupleLib.h>
#include <Streaming.h>

#define DO_TOUCH      false
#define DO_LOGGING    false
#define BLOG          millis()

eBoardPinColor    eReceiverBoardPinColor    {eBoardPinColor::eBluePin};  //Display currently does not send anything

char     aucLeftLabel [][wMaxLabelChars]  = {"Cyl 1", "Cyl 2", "Cyl 3" , "Cyl 4"};
char     aucRightLabel[][wMaxLabelChars]  = {"Oil In" , "Oil Out" , "Heater" , "Air"};

static const uint16_t         usPanelWidth  = 480;
static const uint16_t         usPanelHeight = 320;

const uint8_t                 ucRotation    = 3;     //DIYmall 3.5", Landscape, USB on the left

//Light Google Material Design colors
const lv_color_t              stColorBlack        = lv_color_hex(0x000000);
const lv_color_t              stColorWhite        = lv_color_hex(0xFFFFFF);
const lv_color_t              stColorRed          = lv_color_hex(0xFF0000);
const lv_color_t              stColorGreen        = lv_color_hex(0x00FF00);
const lv_color_t              stColorBlue         = lv_color_hex(0x0000FF);
const lv_color_t              stColorLightBlue    = lv_color_hex(0x7F7FFF);
const lv_color_t              stColorLightYellow  = lv_color_hex(0xFFFFA0);
const lv_color_t              stColorYellow200    = lv_color_hex(0xFFF59D);
const lv_color_t              stColorYellowA100   = lv_color_hex(0xFFFF8D);
const lv_color_t              stColorOrange200    = lv_color_hex(0xFFCC80);
const lv_color_t              stColorOrange600    = lv_color_hex(0xFB8C00);

//Dark Google Material Design colors
const lv_color_t              stColorRed900       = lv_color_hex(0xB71C1C);
const lv_color_t              stColorPurple900    = lv_color_hex(0x4A148C);
const lv_color_t              stColorIndigo900    = lv_color_hex(0x1A237E);
const lv_color_t              stColorTeal900      = lv_color_hex(0x004D40);
const lv_color_t              stColorGreen900     = lv_color_hex(0x1B5E20);
const lv_color_t              stColorBlueGray900  = lv_color_hex(0x263238);

//Text color
//Tried: stColorWhite,stColorYellow100,stColorYellowA100,stColorOrange600,
//  stColorGreen900,stColorBlue,stColorLightBlue,stColorLightYellow
static lv_color_t             stDefaultTextColor  = stColorWhite;
static lv_color_t             stTinyTextColor     = stColorLightBlue;
static lv_color_t             stTitleTextColor    = stColorOrange600;
static lv_color_t             stDataTextColor     = stColorRed;
static lv_color_t             stLocationTextColor = stColorGreen;

//Background color
//Tried: stColorBlack,stColorBlueGray900,stColorPurple900,stColorRed900,stColorRed
static lv_color_t             stDefaultBGColor    = stColorBlack;

const lv_font_t               *pTinyTextFont      = &lv_font_montserrat_12;
const lv_font_t               *pTitleTextFont     = &lv_font_montserrat_30;
const lv_font_t               *pDataTextFont      = &lv_font_montserrat_24;
const lv_font_t               *pLocationTextFont  = &lv_font_montserrat_30;

static lv_style_t             stTinyTextStyle;
static lv_style_t             stTitleTextStyle;
static lv_style_t             stDataTextStyle;
static lv_style_t             stLocationTextStyle;

static lv_disp_drv_t          stDisplay;
static lv_indev_drv_t         stTouchPad;
static lv_disp_draw_buf_t     stDisplayBuffer;     //See src\hal\lv_hal_disp.h
static lv_color_t             stColorPixelsBuffer[(usPanelWidth * usPanelHeight / 10)];

TFT_eSPI                      TFTPanel      = TFT_eSPI(usPanelWidth, usPanelHeight);  //TFT_eSPI acts as display driver

//Function protos
void setup                    (void);
void loop                     (void);
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
  Serial << endl << endl << BLOG << " setup(): Sketch: " << szSketchName << ", " << szFileDate << endl;

  Serial << BLOG << " setup(): Call SetupESP_NOW()" << endl;
  SetupESP_NOW();

  Serial << BLOG << " setup(): Call SetupLVGL(), SetupLogging, SetupLogging()" << endl;
  SetupLVGL();
  SetupTouchscreen();
  SetupLogging();
  SetupStyles();

  DisplayText();

  Serial << BLOG << " setup(): Done" << endl;
  return;
} //setup


void loop(){
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
  return;
} //loop


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

  Serial << BLOG << " SetupLVGL(): Call TFTPanel.init()" << endl;
  TFTPanel.init();

  Serial << BLOG << " SetupLVGL(): Call TFTPanel.setRotation(ucRotation)" << endl;
  TFTPanel.setRotation(ucRotation);

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
  /*Set the touchscreen calibration data,
   the actual data for your display can be acquired using
   the Generic -> Touch_calibrate example from the TFT_eSPI library*/
  uint16_t calData[5] = { 275, 3620, 264, 3532, 1 };
  Serial << BLOG << " SetupLVGL(): Call TFTPanel.setTouch(calData)" << endl;
  TFTPanel.setTouch(calData);
#endif

  /*Initialize the (dummy) input device driver*/
  //static lv_indev_drv_t     indev_drv;
  Serial << BLOG << " SetupLVGL(): Call lv_indev_drv_init(&stTouchPad)" << endl;
  lv_indev_drv_init(&stTouchPad);

  stTouchPad.type      = LV_INDEV_TYPE_POINTER;
  stTouchPad.read_cb   = my_touchpad_read;

  Serial << BLOG << " SetupLVGL(): Call lv_indev_drv_register(&stTouchPad)" << endl;
  lv_indev_drv_register(&stTouchPad);

  return;
} //SetupTouchscreen


void SetupLogging(void){
#if DO_LOGGING && (LV_USE_LOG != 0)
  Serial << BLOG << " SetupLVGL(): Call lv_log_register_print_cb(my_print)" << endl;
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


void my_print(lv_log_level_t cLevel, const char *stColorPixelsBuffer){
  LV_UNUSED(cLevel);
  Serial.flush();
  return;
} //my_print
//Last line.
