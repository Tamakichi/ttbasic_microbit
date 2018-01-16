//
// micro:bit用 RTC by たま吉さん 2018/01/12
//
// 下記のソースコードを参考にして実装しています
// ・Francis Schumacher /  nrf51_rtc 
//   https://os.mbed.com/users/fxschumacher/code/nrf51_rtc/
// ・Francis Schumacher /  nRF51_rtc_example
//   https://os.mbed.com/users/fxschumacher/code/nRF51_rtc_example/file/c1f06d0a5e11/main.cpp/
//

#include "nrf.h"
#include "nrf51_rtc.h"

#define LFCLK_FREQUENCY  0x8000
#define RTC_COUNTER_SIZE 0x1000000

// RTC利用開始
void nrf51RTC::begin() {
  // rtc0を利用
  // 582.542 時間(24日間）カウント
  NRF_RTC0->TASKS_STOP  = 1;                     // RTCストップ
  NRF_RTC0->TASKS_CLEAR = 1;                     // カウンタークリア

  NRF_RTC0->PRESCALER = 0x7ff;                   // プリスケーラ分周 で125msの周期を設定
  NRF_RTC0->INTENSET = 0;                        // 割り込みなし
  NRF_RTC0->EVTENSET = 0;                        // イベントなし

  rtc_previous=0;                                // 直前の経過時間(秒)
  time_base=0;                                   // time_tベース時間
  ticks_per_second =                             // 1秒あたりのカウント数
    LFCLK_FREQUENCY / (NRF_RTC0->PRESCALER + 1);
  counter_size_in_seconds =                      // 最大カウント秒
    RTC_COUNTER_SIZE / ticks_per_second;    

  NRF_RTC0->TASKS_START = 1;                     // RTCスタート
}

// RTC利用終了
void nrf51RTC::end() {
  NRF_RTC0->TASKS_STOP = 1;                      // RTCストップ
}

// 時刻の設定
uint32_t nrf51RTC::setTime(time_t rawtime) {
  time_base = rawtime;
  rtc_previous = uint32_t (NRF_RTC0->COUNTER) / ticks_per_second;
  return 0;
}

// 時刻の取得
time_t nrf51RTC::getTime() {
  uint32_t rtc_now = (NRF_RTC0->COUNTER) / ticks_per_second;
  uint32_t delta_seconds = ((rtc_now + counter_size_in_seconds) - rtc_previous) % counter_size_in_seconds;
  time_base = time_base + (time_t) delta_seconds;
  rtc_previous = rtc_now;
  return time_base;
}

// RTCの更新
// RTCのオーバーフロ発生以内（582.542 時間(24日間）カウント）に
// 更新する必要がある
void nrf51RTC::updateRTC() {
  getTime();
}

