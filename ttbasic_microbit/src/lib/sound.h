//
// file: sound.h
// サウンド再生
//
// 作成日 2018/01/25 by たま吉さん
//
  

#ifndef __sound_h__
#define __sound_h__

#include <Arduino.h>
void dev_tone(uint8_t pin,uint16_t freq, uint16_t duration) ;  // 音の停止
void dev_notone() ;                                            // 音の停止
 
#endif
  