//
// 豊四季Tiny BASIC for micro:bit 構築コンフィグレーション
// 作成日 2017/12/25 Arduino STM32版 V0.85βからの移植
//

#ifndef __ttconfig_h__
#define __ttconfig_h__

#define __FLASH__

// ** (1)デフォルトスクリーンモード 0:シリアルターミナル 1:NTSC・OLED・TFTデバイススクリーン
#define USE_SCREEN_MODE 0  // ※デバイススクリーン利用の場合、1を指定する (デフォルト:1)

// ※次の(2)～(4)は排他選択(全て0または、どれか1つが1)

// ** (2)NTSCビデオ出力利用有無 **********************************************
#define USE_NTSC    0 // 0:利用しない 1:利用する (デフォルト:1)
#define NTSC_SCMODE 1 // スクリーンモード(1～3 デオフォルト:1 )

// ** (3)OLED(SH1106/SSD1306/SSD1309) (SPI/I2C)OLEDモジュール利用有無*********
#define USE_OLED    0 // 0:利用しない 1:利用する (デフォルト:0)
                       // 利用時は USE_NTSC を0にすること
 #define OLED_IFMODE 1 // OLED接続モード(0:I2C 1:SPI デオフォルト:1 )
 #define OLED_SCMODE 1 // スクリーンモード(1～3 デオフォルト:1 )
 #define OLED_RTMODE 0 // 画面の向き (0～3: デフォルト: 0)

// ※1 OLEDモジュールはデフォルトでSH1106 SPIの設定です
//     SSD1306/SSD1309を利用する場合、以下の修正も必要です
//     libraries/TTBAS_LIB/tOLEDScreen.hの下記の値の修正
//       #define OLED_DEV 0 // 0:SH1106 1:SSD1306/SSD1309
//
// ※2 ArduinoSTM32モジュール安定版以降のバージョンを利用している場合、
//     次の定義の修正が必要です。
//     libraries/Adafruit_SH1106_STM32/Adafruit_SH1106_STM32.cpp
//       #define OLD_ARDUINO_STM32 1  // Arduino STM32環境が R20170323:1、 それ以降 0
//     libraries/Adafruit_SSD1306_STM32_TT/Adafruit_SSD1306_STM32_TT.cpp
//       #define OLD_ARDUINO_STM32 1  // Arduino STM32環境が R20170323:1、 それ以降 0
//

// ** (4)TFTILI9341 SPI)液晶モジュール利用有無 *******************************
#define USE_TFT     0 // 0:利用しない 1:利用する (デフォルト:0)
                      // 利用時は USE_NTSC を0にすること
 #define TFT_SCMODE 1 // スクリーンモード(1～6 デオフォルト:1 )
 #define TFT_RTMODE 3 // 画面の向き (0～3: デフォルト: 3)

// ** ターミナルモード時のデフォルト スクリーンサイズ  *************************
// ※ 可動中では、WIDTHコマンドで変更可能  (デフォルト:80x24)
#define TERM_W       80
#define TERM_H       24

// ** Serial1のデフォルト通信速度 *********************************************
#define GPIO_S1_BAUD    115200 // // (デフォルト:115200)

// ** デフォルトのターミナル用シリアルポートの指定 0:USBシリアル 1:GPIO UART1
// ※ 可動中では、SMODEコマンドで変更可能
#define DEF_SMODE     0 // (デフォルト:0)

// ** 起動時にBOOT1ピン(PB2)によるシリアルターミナルモード起動選択の有無
#define FLG_CHK_BOOT1  0 // 0:なし  1:あり // (デフォルト:1)

//** Wireライブラリ新旧指定対応
#define OLD_WIRE_LIB  1 // 0:2017/08/04以降のバージョン 1:R20170323相当

// ** I2Cライブラリの選択 0:Wire(ソフトエミュレーション) 1:HWire  *************
#define I2C_USE_HWIRE  1 // (デフォルト:1)

// ** 内蔵RTCの利用指定   0:利用しない 1:利用する *****************************
#define OLD_RTC_LIB    0 // 0:2017/08/04以降のバージョン 1:R20170323相当
#define USE_INNERRTC   1 // (デフォルト:1) ※ SDカード利用時は必ず1とする
#define RTC_CLOCK_SRC  RTCSEL_LSE // 外部32768Hzオシレータ(デフォルトRTCSEL_LSE)
                                  // RTCSEL_LSI,RTCSEL_HSEの指定も可能

// ** SDカードの利用      0:利用しない 1:利用する *****************************
#define USE_SD_CARD    0 // (デフォルト:1)
#if USE_SD_CARD == 1 && USE_INNERRTC == 0
 #define USE_INNERRTC 1
#endif

// ** フォントデータ指定 ******************************************************
#define FONTSELECT  4  // 0 ～ 3 (デフォルト :1)

#if FONTSELECT == 0
  // 6x8 TVoutフォント
  #define DEVICE_FONT font6x8
  #include <font6x8.h>

#elif FONTSELECT == 1
  // 6x8ドット オリジナルフォント(デフォルト)
  #define DEVICE_FONT font6x8tt
  #include <font6x8tt.h>

#elif FONTSELECT == 2
  // 8x8 TVoutフォント
  #define DEVICE_FONT font8x8
  #include <font8x8.h>

#elif FONTSELECT == 3
  // 8x8 IchigoJamフォント(オプション機能 要フォント)
  #define DEVICE_FONT ichigoFont8x8 
  #include <ichigoFont8x8.h>
#else
  #define DEVICE_FONT ((uint8_t *)NULL)
#endif

// 設定の矛盾補正
#if USE_TFT  == 1 || USE_OLED == 1
 #define USE_NTSC 0
#endif
#if USE_NTSC == 0 && USE_TFT == 0 && USE_OLED == 0
 #define USE_SCREEN_MODE 0
#endif

// デバイスコンソール抽象化定義（修正禁止）
#if USE_NTSC == 1
 #define DEV_SCMODE NTSC_SCMODE
 #define DEV_RTMODE CONFIG.NTSC
 #define DEV_IFMODE 0
 #define MAX_SCMODE 3
#elif USE_OLED == 1
 #define DEV_SCMODE OLED_SCMODE
 #define DEV_RTMODE OLED_RTMODE
 #define DEV_IFMODE OLED_IFMODE
 #define MAX_SCMODE 3 
#elif USE_TFT == 1
 #define DEV_SCMODE TFT_SCMODE
 #define DEV_RTMODE TFT_RTMODE
 #define DEV_IFMODE 0
 #define MAX_SCMODE 6 
#endif

#endif
