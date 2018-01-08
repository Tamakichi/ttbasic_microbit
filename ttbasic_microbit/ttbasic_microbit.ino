/*
  TOYOSHIKI Tiny BASIC for Arduino
 (C)2012 Tetsuya Suzuki
  2017/12/25 Arduino micro:bit 対応 by たま吉さん
 */


#include "ttconfig.h"
void basic(void);

void setup(void){
  Serial.begin(115200);
  randomSeed(analogRead(0));
}

void loop(void){
  basic();
}
