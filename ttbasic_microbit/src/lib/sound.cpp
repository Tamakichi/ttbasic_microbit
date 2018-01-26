//
// file: sound.cpp
// サウンド再生
//
// 作成日 2018/01/25 by たま吉さん
//

#include "sound.h"


#define TIMERFREQ 1000000L    // タイマー基本周期
#define TONE_TIMER NRF_TIMER0 // 利用タイマー資源

//
// 音の停止
// 引数
//
void dev_notone() {
    TONE_TIMER->TASKS_STOP = 1;  // タイマストップ
}

//
// 音出し
// 引数
//  pin     : PWM出力ピン
//  freq    : 出力周波数 (Hz) 15～ 50000
//  duration: 出力時間(msec)
//
void dev_tone(uint8_t pin, uint16_t freq, uint16_t duration) {
  uint32_t ulPin;
  uint32_t f =TIMERFREQ/freq;
  
  // GPIOTEの設定：LEDピン・トグルタスクを定義する
  ulPin = g_ADigitalPinMap[pin];   // TonePinの実ピン番号の取得 
  NRF_GPIOTE->CONFIG[0] =          // チャネル0に機能設定
    (GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos) |            // タスクモード
    (ulPin << GPIOTE_CONFIG_PSEL_Pos) |                              // ピン番号設定
    (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos) |  // 動作指定：トグル
    (GPIOTE_CONFIG_OUTINIT_Low << GPIOTE_CONFIG_OUTINIT_Pos);        // ピン出力初期値
  NRF_GPIOTE->POWER = 1;                                             // GPIOTE有効
    
  //タイマ設定
  TONE_TIMER->TASKS_STOP = 1;                          // タイマストップ
  TONE_TIMER->TASKS_CLEAR = 1;                         // カウンタクリア
  TONE_TIMER->MODE = TIMER_MODE_MODE_Timer;            // モード設定：タイマモード
  TONE_TIMER->PRESCALER   = 4;                         // プリスケーラ設定：16分周(1MHz)
  TONE_TIMER->BITMODE = TIMER_BITMODE_BITMODE_16Bit;   // カウンタ長設定：16ビット長指定
  TONE_TIMER->CC[0] = f/2;                             // コンパレータ0の設定(出力周波数設定)

  TONE_TIMER->SHORTS =                                 // ショートカット設定：クリアタスク指定
      (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);

  // PPIの設定(チャネル0を利用)
  //   TIMER0 コンパレータ0一致イベント と GPIOTE(ch0)LEDピン・トグルタスク を結び付ける
  NRF_PPI->CH[0].TEP  = (uint32_t)&NRF_GPIOTE->TASKS_OUT[0];       // PPI.ch0 にLEDピン・トグルタスク設定
  NRF_PPI->CH[0].EEP  = (uint32_t)&TONE_TIMER->EVENTS_COMPARE[0];  // PPI ch0 にコンパレータ0一致イベント設定  
  NRF_PPI->CHENSET   |= PPI_CHENSET_CH0_Enabled;                   // PPI ch0 有効

  TONE_TIMER->TASKS_START = 1;   // タイマスタート

  if (duration) {
    delay(duration);
    TONE_TIMER->TASKS_STOP = 1;  // タイマストップ
  }
}
