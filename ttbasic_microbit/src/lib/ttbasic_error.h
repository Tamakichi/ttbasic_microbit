//
// 豊四季Tiny BASIC for Arduino STM32 エラーコード定義
// 2017/11/07 by たま吉さん
// 2018/01/14 micro:bit 対応 by たま吉さん
//

#ifndef __ttbasic_error_h__
#define __ttbasic_error_h__

// エラーコード
enum ErrorInfo {
  ERR_OK,
  ERR_DIVBY0,
  ERR_VOF,
  ERR_SOR,
  ERR_IBUFOF, ERR_LBUFOF,
  ERR_GSTKOF, ERR_GSTKUF,
  ERR_LSTKOF, ERR_LSTKUF,
  ERR_NEXTWOV, ERR_NEXTUM, ERR_FORWOV, ERR_FORWOTO,
  ERR_LETWOV, ERR_IFWOC,
  ERR_ULN,
  ERR_PAREN, ERR_VWOEQ,
  ERR_COM,
  ERR_VALUE, // 追加
  ERR_RANGE, // 追加
  ERR_NOPRG, // 追加
  ERR_SYNTAX,
  ERR_SYS,
  ERR_CTR_C,
  ERR_LONG,                 // 追加
  ERR_GPIO,                 // 追加 V0.83
  ERR_NOT_SUPPORTED,        // 追加 V0.84
  ERR_NP_NOT_START,         // micro:bit V0.05で追加
};

extern const char* errmsg[];
  
#endif
