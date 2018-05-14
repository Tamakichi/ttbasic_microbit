//
// file:tTermscreen.cpp
// ターミナルスクリーン制御ライブラリ 
//  V1.0 作成日 2017/03/22 by たま吉さん
//  修正日 2017/03/26, 色制御関連関数の追加
//  修正日 2017/03/30, moveLineEnd()の追加,[HOME],[END]の編集キーの仕様変更
//  修正日 2017/06/27, 汎用化のための修正
//  修正日 2018/02/10, 基底クラスtSerialDevの廃止対応
//

#include <string.h>
#include "tTermscreen.h"

//******* mcurses用フック関数の定義(開始)  *****************************************

// シリアル経由1文字出力
static void Arduino_putchar(uint8_t c) {
  Serial.write(c);
}

// シリアル経由1文字入力
static char Arduino_getchar() {
  while (!Serial.available());
  return Serial.read();
}
//******* mcurses用フック関数の定義(終了)  *****************************************

//****** シリアルターミナルデバイス依存のメンバー関数のオーバーライド定義(開始) ****

// カーソルの移動
// pos_x,pos_yは本関数のみでのみ変更可能
// カーソルの表示も行う
void tTermscreen::MOVE(uint8_t y, uint8_t x) {
  ::move(y,x);
  pos_x = x;
  pos_y = y;
};

// 文字の表示
void tTermscreen::WRITE(uint8_t x, uint8_t y, uint8_t c) {
  ::move(y,x);
  ::addch(c);
  ::move(pos_y, pos_x);
}

// 指定行に1行挿入(下スクロール)
void tTermscreen::INSLINE(uint8_t l) {
  ::move(l,0);
  ::insertln();
  ::move(pos_y,pos_x);
}

// 依存デバイスの初期化
// シリアルコンソール mcursesの設定
void tTermscreen::INIT_DEV() {
  // mcursesの設定
  ::setFunction_putchar(Arduino_putchar);  // 依存関数
  ::setFunction_getchar(Arduino_getchar);  // 依存関数
  ::initscr();                             // 依存関数
  ::setscrreg(0,height-1);
}

// キー入力チェック
uint8_t tTermscreen::isKeyIn() {
  if (Serial.available())
     return get_ch();
  return 0;
}

// カーソルの表示/非表示
// flg: カーソル非表示 0、表示 1、強調表示 2
void tTermscreen::show_curs(uint8_t flg) {
  flgCur = flg;
  ::curs_set(flg);  // 依存関数
}

// 文字色指定
void tTermscreen::setColor(uint16_t fc, uint16_t bc) {
  static const uint16_t tbl_fcolor[]  =
     { F_BLACK,F_RED,F_GREEN,F_BROWN,F_BLUE,F_MAGENTA,F_CYAN,A_NORMAL,F_YELLOW};
  static const uint16_t tbl_bcolor[]  =
     { B_BLACK,B_RED,B_GREEN,B_BROWN,B_BLUE,B_MAGENTA,B_CYAN,B_WHITE,B_YELLOW};

  if ( fc <= 8 && bc <= 8 )
     attrset(tbl_fcolor[fc]|tbl_bcolor[bc]);  // 依存関数
}

// 文字属性
void tTermscreen::setAttr(uint16_t attr) {
  static const uint16_t tbl_attr[]  =
    { A_NORMAL, A_UNDERLINE, A_REVERSE, A_BLINK, A_BOLD };
  
  if ( attr <= 4 )
     attrset(tbl_attr[attr]);  // 依存関数
}


//****** シリアルターミナルデバイス依存のメンバー関数のオーバーライド定義(終了) ****
