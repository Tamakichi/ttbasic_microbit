//
// micro:bit用 RTC by たま吉さん 2018/01/12
// 
// 下記のソースコードを参考にして実装しています
// ・Francis Schumacher /  nrf51_rtc 
//   https://os.mbed.com/users/fxschumacher/code/nrf51_rtc/
// ・Francis Schumacher /  nRF51_rtc_example
//   https://os.mbed.com/users/fxschumacher/code/nRF51_rtc_example/file/c1f06d0a5e11/main.cpp/
//
#ifndef __nrf51_rtc__
#define __nrf51_rtc__

#include <Arduino.h>
#include <Time.h>
class nrf51RTC {
  public:
    nrf51RTC() {};
    time_t getTime();
    uint32_t setTime(time_t rawtime);
    void begin();
    void end();
    
  private:
    time_t time_base;
    uint32_t rtc_previous;
    uint32_t ticks_per_second, counter_size_in_seconds;
    void updateRTC(); 
};

#endif

