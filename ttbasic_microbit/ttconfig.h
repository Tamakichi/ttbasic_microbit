//
// 豊四季Tiny BASIC for micro:bit 構築コンフィグレーション
// 修正日 2017/12/25 Arduino STM32版 V0.85βからの移植
// 修正日 2018/01/07 Arduino STM32版依存部の削除と整理
// 修正日 2018/02/09 UARTの外部ポート利用オプションの追加
//

#ifndef __ttconfig_h__
#define __ttconfig_h__

// ** ターミナルモード時のデフォルト スクリーンサイズ  *************************
// ※ 可動中では、WIDTHコマンドで変更可能  (デフォルト:80x24)
#define TERM_W       80
#define TERM_H       24

// ** UARTの外部ポート指定(要Arduinoのコア部の修正) ***************************
#define USEEXSERIAL 1 // 0:利用しない 1:利用する (デフォルト:1)

// ** フォントデータ指定 *****************************************************
#define FONTSELECT  1  // 1 or 0 (デフォルト :1)

#if FONTSELECT == 1
  // 5x5ドット オリジナルフォント(デフォルト)
  #define DEVICE_FONT font5x5tt
  #include "src/lib/font5x5tt.h"
#else
  #define DEVICE_FONT ((uint8_t *)NULL)
#endif

#endif
