/*
  TOYOSHIKI Tiny BASIC for Arduino
 (C)2012 Tetsuya Suzuki
  2017/12/25 Arduino micro:bit 対応 by たま吉さん
 */


#include "ttconfig.h"
void basic(void);

void setup(void){
  // LFCLKSRCの設定
  NRF_CLOCK->TASKS_LFCLKSTOP = 1;
  NRF_CLOCK->LFCLKSRC = 
    (uint32_t)((CLOCK_LFCLKSRC_SRC_Synth << CLOCK_LFCLKSRC_SRC_Pos) &
    CLOCK_LFCLKSRC_SRC_Msk);
  NRF_CLOCK->TASKS_LFCLKSTART = 1;
    
  Serial.begin(115200);
  randomSeed(analogRead(0));
}

void loop(void){
  basic();
}
