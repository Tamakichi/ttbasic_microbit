/*
  TOYOSHIKI Tiny BASIC for Arduino
 (C)2012 Tetsuya Suzuki
  2017/03/22 Modified by Tamakichi、for Arduino STM32
 */

//
// 2017/12/25 Arduino micro:bit 対応　v0.01 by たま吉さん
//

#include "ttconfig.h"

void basic();

void basic(void);

void setup(void){
  // put your setup code here, to run once:
  Serial.begin(115200);
#if defined (__STM32F1__)   
  while (!Serial.isConnected()) delay(100);
#endif
  randomSeed(analogRead(0));
}

void loop(void){
  // put your main code here, to run repeatedly:
  basic();
}
