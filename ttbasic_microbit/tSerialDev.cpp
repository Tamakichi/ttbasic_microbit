//
// 豊四季Tiny BASIC for Arduino STM32 シリアルデバイス制御
// 2017/07/19 by たま吉さん
//

#include <Arduino.h>
#include <stdint.h>
#include "tSerialDev.h"

void tSerialDev::Serial_open(uint32_t baud) {
#if 0
 if (serialMode == 0)
    Serial1.begin(baud);
#endif
}

// シリアルポートクローズ
void tSerialDev::Serial_close() {
#if 0
 if (serialMode == 0)
    Serial1.end();  ;
#endif
}

// シリアル1バイト出力
void tSerialDev::Serial_write(uint8_t c) {
#if 0
  if (serialMode == 0) {
     Serial1.write( (uint8_t)c);
  } else if (serialMode == 1) {
     Serial.write( (uint8_t)c);  
  }
#endif
}

// シリアル1バイト入力
int16_t tSerialDev::Serial_read() {
#if 0
  if (serialMode == 0) 
    return Serial1.read();
  else if (serialMode == 1) 
    return Serial.read();
  return 0;
#endif
}

// シリアル改行出力
void tSerialDev::Serial_newLine() {
#if 0
  if (serialMode == 0) {
    Serial1.write(0x0d);
    Serial1.write(0x0a);
  } else if (serialMode == 1) {
    Serial.write(0x0d);
    Serial.write(0x0a);
  }
#endif
}

// シリアルデータ着信チェック
uint8_t tSerialDev::Serial_available() {
#if 0
  if (serialMode == 0) 
     return Serial1.available();
  else if (serialMode == 1) 
     return Serial.available();
#endif
  return 0;
}

// シリアルポートモード設定
void tSerialDev::Serial_mode(uint8_t c, uint32_t b) {
#if 0
  serialMode = c;
  if (serialMode == 1) {
    Serial1.begin(b);
  } else {

  }
#endif
}
