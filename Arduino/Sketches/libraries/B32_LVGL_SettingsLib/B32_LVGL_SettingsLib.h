//Larry Beck, B32_LVGL_GraphicsLib.h
#pragma once
#pragma message("B32_LVGL_GraphicsLib.h, 1/9/24c: Begin declaring colors and fonts")
#include <lvgl.h>

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
/*
const lv_font_t               *pTitleTextFont     = &lv_font_montserrat_30;
const lv_font_t               *pDataTextFont      = &lv_font_montserrat_24;
const lv_font_t               *pLocationTextFont  = &lv_font_montserrat_30;
*/
const lv_font_t               *pTitleTextFont     = &lv_font_montserrat_16;
const lv_font_t               *pDataTextFont      = &lv_font_montserrat_16;
const lv_font_t               *pLocationTextFont  = &lv_font_montserrat_16;

static lv_style_t             stTinyTextStyle;
static lv_style_t             stTitleTextStyle;
static lv_style_t             stDataTextStyle;
static lv_style_t             stLocationTextStyle;
//Last line.
