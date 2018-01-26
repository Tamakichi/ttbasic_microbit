//
// 豊四季Tiny BASIC for Arduino STM32 エラーメッセージ定義
// 2017/11/07 by たま吉さん
// 2018/01/14 micro:bit 対応 by たま吉さん
//

#include <Arduino.h>
const char* errmsg[] /*__FLASH__*/ = {
  "OK",
  "Devision by zero",
  "Overflow",
  "Subscript out of range",
  "Icode buffer full",
  "List full",
  "GOSUB too many nested",
  "RETURN stack underflow",
  "FOR too many nested",
  "NEXT without FOR",
  "NEXT without counter",
  "NEXT mismatch FOR",
  "FOR without variable",
  "FOR without TO",
  "LET without variable",
  "IF without condition",
  "Undefined line number or label",
  "\'(\' or \')\' expected",
  "\'=\' expected",
  "Cannot use system command", // v0.83 メッセージ文変更
  "Illegal value",      // 追加
  "Out of range value", // 追加
  "Program not found",  // 追加
  "Syntax error",
  "Internal error",
  "Break",
  "Line too long",
  "Cannot use GPIO function",// 追加
  "Not supported",           // 追加
  "Neopixel not start",      // micro:bit V0.05で追加
  "Illegal MML",             // micro:bit V0.06で追加
  
};
