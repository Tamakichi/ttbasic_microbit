#ifndef _CGRAPTH_H_
#define _CGRAPTH_H_

#include <Adafruit_GFX.h>
#include <Arduino.h>

class CGraph : public Adafruit_GFX {
 private:
  uint16_t _dotChar;
 public:
  CGraph(int16_t w, int16_t h);
  ~CGraph(void);
  boolean   begin(void);
  void      drawPixel(int16_t x, int16_t y, uint16_t color);
  void      setDotChar(uint16_t c) { _dotChar = c;};
};

#endif

