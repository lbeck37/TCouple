//B32_RGBDisplayLib.cpp, 1/23/24d
#include <B32_RGBDisplayLib.h>
#include <Streaming.h>

#ifndef BLOG
  #define BLOG          millis()    //Used in logging
#endif

RGBScreen::RGBScreen(uint16_t usWidthPixels, uint16_t usHeightPixels){
  usScreenWidth      = usWidthPixels;
  usScreenHeight     = usHeightPixels;

  Serial << BLOG << " RGBScreen() Constructor: usScreenWidth= " << usScreenWidth << ", usScreenHeight= " << usScreenHeight << endl;
  pRGBPanel = new Arduino_ESP32RGBPanel(
          cDE_Pin      , cVsyncPin    , cHsyncPin    , cPclkPin,
          acRedPin  [0], acRedPin  [1], acRedPin  [2], acRedPin  [3], acRedPin  [4],
          acGreenPin[0], acGreenPin[1], acGreenPin[2], acGreenPin[3], acGreenPin[4], acGreenPin[5],
          acBluePin [0], acBluePin [1], acBluePin [2], acBluePin [3], acBluePin [4],
          usHsyncPolarity, usHsyncFrontPorch, usHsyncPulseWidth, usHsyncBackPorch,
          usVsyncPolarity, usVsyncFrontPorch, usVsyncPulseWidth, usVsyncBackPorch,
          usPclkActiveNeg, uwPreferSpeed    , bUseBigEndian    , usDEIdleHigh    , usPclkIdleHigh);

  pRGBDisplay = new Arduino_RGB_Display(usScreenWidth, usScreenHeight, pRGBPanel);

} //RGBScreen constructor


RGBScreen::~RGBScreen(void){
  Serial << BLOG << " ~RGBScreen desctructor, delete pRGBDisplay and pRGBPanel" << endl;
  delete pRGBDisplay;
  delete pRGBPanel;
} //RGBScreen destructor


void RGBScreen::SetupDisplay(void){
  Serial << BLOG << " SetupDisplay(): Call pRGBDisplay->begin()" << endl;
  if (!pRGBDisplay->begin()){
    Serial << BLOG << " SetupDisplay(): pRGBDisplay->begin() failed" << endl;
  }
  else{
    Serial << BLOG << " SetupDisplay(): Clear screen with call to pRGBDisplay->fillScreen(BLACK)" << endl;
    pRGBDisplay->fillScreen(BLACK);
  }

  delay(1000);
  return;
} //SetupDisplay


void RGBScreen::DisplayLabels(void){
  uint8_t   ucTextXscale     = 5;
  uint8_t   ucTextYscale     = 5;
  uint8_t   ucPixelMargin    = 2;

  pRGBDisplay->setTextColor   (CYAN, BLACK);
  pRGBDisplay->setTextSize    (ucTextXscale, ucTextYscale, ucPixelMargin);

  for (int wLineNum= 0; wLineNum < 4; wLineNum++) {
    pRGBDisplay->setCursor(sLeftLabelX, (sLeftLabelFirstY + (wLineNum * sLineSpacing)));
    *pRGBDisplay << aucLeftLabel[wLineNum];

    pRGBDisplay->setCursor(sRightLabelX, (sRightLabelFirstY + (wLineNum * sLineSpacing)));
    *pRGBDisplay << aucRightLabel[wLineNum];
  } //for(int wLineNum= 0...
  return;
} //DisplayLabels


void RGBScreen::RandomDisplay(void){
  uint16_t  usDisplayWidth   = random(pRGBDisplay->width());
  uint16_t  usDisplayHeight  = random(pRGBDisplay->height());
  uint16_t  usTextColor      = random(0xffff);
  uint16_t  usTextBGColor    = random(0xffff);

  uint8_t   ucTextXscale     = random(6);
  uint8_t   ucTextYscale     = random(6);
  uint8_t   ucPixelMargin    = random(2);

  Serial << BLOG << " loop():"
      " usDisplayWidth= " << usDisplayWidth  << ", usDisplayHeight= " << usDisplayHeight <<
      ", ucTextXscale= "  << ucTextXscale    << ", ucTextYscale= "    << ucTextYscale <<
      ", ucPixelMargin= " << ucPixelMargin   << endl;

  pRGBDisplay->setCursor      (usDisplayWidth, usDisplayHeight);
  pRGBDisplay->setTextColor   (usTextColor   , usTextBGColor);
  pRGBDisplay->setTextSize    (ucTextXscale  , ucTextYscale, ucPixelMargin);

  pRGBDisplay->println        ("Hello World!");
  return;
} //RandomDisplay
//Last line
