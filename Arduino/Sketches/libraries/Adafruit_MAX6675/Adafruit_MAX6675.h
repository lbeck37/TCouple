//LBeck 10/9/23b
// file= Adafruit_MAX6675.h
// this library is public domain. enjoy!
// https://learn.adafruit.com/thermocouple/
/*
#ifndef ADAFRUIT_MAX6675_H
#define ADAFRUIT_MAX6675_H
*/
#pragma once

#include "Arduino.h"

/**************************************************************************/
/*!
    @brief  Class for communicating with thermocouple sensor
*/
/**************************************************************************/
class Adafruit_MAX6675 {
public:
  Adafruit_MAX6675(int8_t SCLK, int8_t CS, int8_t MISO);

  float readCelsius(void);
  float readFahrenheit(void);

  /*!    @brief  For compatibility with older versions
         @returns Temperature in F or NAN on failure! */
  float readFarenheit(void) { return readFahrenheit(); }

private:
  int8_t sclk, miso, cs;
  uint8_t spiread(void);
};

//#endif
