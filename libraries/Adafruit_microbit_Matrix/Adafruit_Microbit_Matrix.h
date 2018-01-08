#ifndef _ADAFRUIT_MICROBIT_MATRIX_H_
#define _ADAFRUIT_MICROBIT_MATRIX_H_

#include <Adafruit_GFX.h>
#include <Arduino.h>

#define LED_ON   1
#define LED_OFF  0

void IRQ_MATRIX_HANDLER(void);


class Adafruit_Microbit_Matrix : public Adafruit_GFX {
 public:
  Adafruit_Microbit_Matrix();
  ~Adafruit_Microbit_Matrix(void);
  boolean   begin(void);
  void      drawPixel(int16_t x, int16_t y, uint16_t color),
            fillScreen(uint16_t color),
            clear(void),
            rowHandler(void);
   uint16_t getPixel(int16_t x, int16_t y); // 2017/12/28 add by ÇΩÇ‹ãg
  
// private: // 2017/12/28 by ÇΩÇ‹ãgÅipublicéwíË)
  void startTimer();
  void stopTimer();
  void restartTimer();  
  uint8_t matrix_buffer[3][9];
};

#endif
