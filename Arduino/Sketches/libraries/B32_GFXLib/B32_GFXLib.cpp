//B32_GFXLib.cpp, 1/27/24d
#include <B32_GFXLib.h>
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


void RGBScreen::ShowMyMAC(void){
  const int   wNumBytesInMAC      = 6;
  uint8_t     ucXscale            = 2;
  uint8_t     ucYscale            = 2;
  int16_t     sStartX             = 0;
  int16_t     sStartY             = (usScreenHeight - 20);

  pRGBDisplay->setTextColor   (uwMACColor , uwBGColor);
  pRGBDisplay->setTextSize    (ucXscale, ucYscale);
  pRGBDisplay->setCursor      (sStartX      , sStartY);

  *pRGBDisplay << szSketchName << "," << szFileDate << ",MAC-";
  for (int wByteNum= 0; wByteNum < wNumBytesInMAC; wByteNum++){
    //Add a leading 0 if necessary
    if(aucMyMACAddress[wByteNum] <= 0xF){
      *pRGBDisplay << "0";
    }
    *pRGBDisplay << _HEX(aucMyMACAddress[wByteNum]);
    if (wByteNum != 5){
      *pRGBDisplay << " ";
    } //if (wByteNum!=5)
  } //for(int wByteNum=0;...

    Serial << "My MAC- ";
    for (int wByteNum= 0; wByteNum < wNumBytesInMAC; wByteNum++){
      //Add a leading 0 if necessary
      if(aucMyMACAddress[wByteNum] <= 0xF){
        Serial << "0";
      }
      Serial << _HEX(aucMyMACAddress[wByteNum]);
      if (wByteNum != 5){
        Serial << ":";
      } //if (wByteNum!=5)
    } //for(int wByteNum=0;...
    Serial << endl;

  return;
} //ShowMyMAC


void RGBScreen::CreateData(void){
  //Create dummy data to display, instead of actual TCouple readings
  static double   dDummyAddDegF= 0.00;

  dDummyAddDegF += 0.10;
  for (int wReadingNum= 0; (wReadingNum < wNumReadings); wReadingNum++) {
    stReadings.adReading[wReadingNum]= (100.00 + (wReadingNum * 10.00) + dDummyAddDegF);
  } //for(int wReadingNum=0;...
  return;
}   //CreateData


void RGBScreen::SetupDisplay(void){
  //Initialize the last displayed values.
  for (int wReadingNum=0; (wReadingNum < wNumReadings); wReadingNum++) {
    stLastReadings.adReading[wReadingNum]= 0.00;
  } //for(int wReadingNum=0;...

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
  uint8_t   ucTitleXscale       = 6;
  uint8_t   ucTitleYscale       = 6;
  int16_t   sTitleX             = 130;
  int16_t   sTitleY             = 0;

  uint8_t   ucLabelXscale       = 5;
  uint8_t   ucLabelYscale       = 5;

  pRGBDisplay->setTextColor   (uwTitleColor , uwBGColor);
  pRGBDisplay->setTextSize    (ucTitleXscale, ucTitleYscale);
  pRGBDisplay->setCursor      (sTitleX      , sTitleY);
  *pRGBDisplay << "Bug Temperatures";

  pRGBDisplay->setTextColor   (uwLabelColor, uwBGColor);
  pRGBDisplay->setTextSize    (ucLabelXscale, ucLabelYscale);

  for (int wLineNum= 0; wLineNum < 4; wLineNum++) {
    pRGBDisplay->setCursor(sLeftLabelX, (sLeftLabelFirstY + (wLineNum * sLineSpacing)));
    *pRGBDisplay << aucLeftLabel[wLineNum];

    pRGBDisplay->setCursor(sRightLabelX, (sRightLabelFirstY + (wLineNum * sLineSpacing)));
    *pRGBDisplay << aucRightLabel[wLineNum];
  } //for(int wLineNum= 0...
  return;
} //DisplayLabels


void RGBScreen::DisplayReadings(stMessageStruct stReadings){
  char      ac100ByteBuffer[100];

 //pRGBDisplay->setFreeFont  (&Monofonto15pt7b);
 for (int wLineNum= 0; wLineNum < 4; wLineNum++) {
   int wLeftIndex   = wLineNum;
   int wRightIndex  = wLineNum + 4;
   if (stLastReadings.adReading[wLeftIndex] != stReadings.adReading[wLeftIndex]){
     pRGBDisplay->setCursor(sLeftDataX, (sLeftDataFirstY + (wLineNum * sLineSpacing)));
     pRGBDisplay->setTextColor(uwBGColor, uwBGColor);
     sprintf(ac100ByteBuffer, "%3.0ff", stLastReadings.adReading[wLeftIndex]);
     *pRGBDisplay << ac100ByteBuffer;

     pRGBDisplay->setCursor(sLeftDataX, (sLeftDataFirstY + (wLineNum * sLineSpacing)));
     pRGBDisplay->setTextColor(uwDataColor, uwBGColor);
     sprintf(ac100ByteBuffer, "%3.0ff", stReadings.adReading[wLeftIndex]);
     *pRGBDisplay << ac100ByteBuffer;
   }

   if (stLastReadings.adReading[wRightIndex] != stReadings.adReading[wRightIndex]){
     pRGBDisplay->setCursor(sRightDataX, (sRightDataFirstY + (wLineNum * sLineSpacing)));
     pRGBDisplay->setTextColor(uwBGColor, uwBGColor);
     sprintf(ac100ByteBuffer, "%5.0ff", stLastReadings.adReading[wRightIndex]);
     *pRGBDisplay << ac100ByteBuffer;

     pRGBDisplay->setCursor(sRightDataX, (sRightDataFirstY + (wLineNum * sLineSpacing)));
     pRGBDisplay->setTextColor(uwDataColor, uwBGColor);
     sprintf(ac100ByteBuffer, "%5.0ff", stReadings.adReading[wRightIndex]);
     *pRGBDisplay << ac100ByteBuffer;
   }
 } //for(int wLineNum= 0...
 for (int wReadingNum= 0; wReadingNum < wNumReadings; wReadingNum++) {
   stLastReadings.adReading[wReadingNum]= stReadings.adReading[wReadingNum];
 }
 return;
}   //DisplayReadings


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
