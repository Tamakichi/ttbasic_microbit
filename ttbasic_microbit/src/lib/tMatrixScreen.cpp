//
// file: tMatrixScreen.cpp
// micro:bit LEDマトリックス制御クラス
// 2017/12/28 作成
// 2018/02/03 修正 ,driveMatrix()で停止時にポート26,27,28をLOWにセット
//

#include <string.h>
#include "tMatrixScreen.h"
//#include <Fonts/TomThumb.h>
//extern const unsigned char font5x5tt[];
extern const uint8_t* ttbasic_font;
#ifndef swap
#define swap(a, b) { int16_t t = a; a = b; b = t; }
#endif

// GVRAMサイズ取得
__attribute__((always_inline)) uint16_t tMatrixScreen::getGRAMsize() {
  g_width*g_height;
}

// グラフィク表示用メモリアドレス参照
__attribute__((always_inline)) uint8_t* tMatrixScreen::getGRAM() {
  return (uint8_t*)this->matrix.matrix_buffer;	
}

boolean tMatrixScreen::begin(void) {
  fontEx   = 1;                // フォント拡大率
 // font = (uint8_t*)font5x5tt;  // フォン
  font = (uint8_t*)ttbasic_font;  // フォン
  f_width  = *(font+0)*fontEx; // 横フォントドット数
  f_height = *(font+1)*fontEx; // 縦フォントドット数
  g_width  = 5;
  g_height = 5;
  fgcolor  = 1;
  bgcolor  = 0;
  pos_gx   = 0;     // グラフィックカーソル
  pos_gy   = 0;     // グラフィックカーソル

  matrix.begin();
  //matrix.setFont(&TomThumb);
  //matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  return true;
}


// グラフィックカーソル設定
void tMatrixScreen::set_gcursor(int16_t x, int16_t y) {
  this->pos_gx = x;
  this->pos_gy = y;
}

// グラフィック文字表示
void  tMatrixScreen::gputch(uint8_t c) {

  drawBitmap_x2( this->pos_gx, this->pos_gy, this->font+3+((uint16_t)c)*f_height,
    	           this->f_width/ this->fontEx,  this->f_height/this->fontEx,  this->fgcolor, this->fontEx,1);
  //Serial.print("[DEBUG]:gputch font=");
  //Serial.println((uint32_t)this->font+3+((uint16_t)c)*5,HEX);
  this->pos_gx += this->f_width;
/*
  if (this->pos_gx + this->f_width >= this->g_width) {
     this->pos_gx = 0;
     this->pos_gy += this->f_height;
  }
*/
}


// 画面全消去
void tMatrixScreen::cls() {
  this->matrix.fillScreen(bgcolor);
  pos_gx =0;  pos_gy =0;
}

// 文字色指定
void tMatrixScreen::setColor(uint16_t fc, uint16_t bc) {
  fgcolor = fc;
  bgcolor = bc;
}

// 文字属性
void tMatrixScreen::setAttr(uint16_t attr) {

}

// ビットマップの拡大描画
void tMatrixScreen::drawBitmap_x2(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color, uint16_t ex, uint8_t f) {
  int16_t i, j,b=(w+7)/8;
  for( j = 0; j < h; j++) {
    for(i = 0; i < w; i++ ) { 
      if(*(bitmap + j*b + i / 8) & (128 >> (i & 7))) {
        // ドットあり
        if (ex == 1)
           this->matrix.drawPixel(x+i, y+j, color); //1倍
        else
          this->matrix.fillRect(x + i * ex, y + j * ex, ex, ex, color); // ex倍
      } else {
        // ドットなし
        if (f) {
          // 黒を透明扱いしない
          if (ex == 1)      
            this->matrix.drawPixel(x+i, y+j, bgcolor);
          else
            this->matrix.fillRect(x + i * ex, y + j * ex, ex, ex, bgcolor);
       }
     }
   }
 }
}


// ドット描画
void tMatrixScreen::pset(int16_t x, int16_t y, uint16_t c) {
  this->matrix.drawPixel(x, y, c);
}

// 線の描画
void tMatrixScreen::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c){
  this->matrix.drawLine(x1, y1, x2, y2, c);
}

// 円の描画
void tMatrixScreen::circle(int16_t x, int16_t y, int16_t r, uint16_t c, int8_t f) {
 if(f)
   this->matrix.fillCircle(x, y, r, c);
 else
   this->matrix.drawCircle(x, y, r, c);
}

// 四角の描画
void tMatrixScreen::rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c, int8_t f) {
  if(f)
   this->matrix.fillRect(x, y, w, h, c);
 else
   this->matrix.drawRect(x, y, w, h, c);
}

// ビットマップの描画
void  tMatrixScreen::bitmap(int16_t x, int16_t y, uint8_t* adr, uint16_t index, uint16_t w, uint16_t h, uint16_t d, uint8_t rgb) {
  uint8_t*bmp;
  bmp = adr + ((w + 7) / 8) * h * index;
  this->drawBitmap_x2(x, y, (const uint8_t*)bmp, w, h, fgcolor, d, 1);
}

// 指定座標のドットの取得
int16_t tMatrixScreen::gpeek(int16_t x, int16_t y) {
  return this->matrix.getPixel(x, y);
}

// 指定領域のピクセル有無チェック
int16_t tMatrixScreen::ginp(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t c) {
  for (int16_t i = y ; i < y+h; i++) {
    for (int16_t j= x; j < x+w; j++) {
      if (this->gpeek(x,y) == c) {
          return 1;
      }
    }
  }
  return 0;	
}

// キャラクタ画面スクロール
// x: スクロール開始位置 x
// y: スクロール開始位置 y
// w: スクロール幅
// h: スクロール高さ
// d:方向
/*
void tMatrixScreen::cscroll(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t d) {
  tGraphicScreen::cscroll(x, y, w,h,d);
  //this->matrix.display();
}
*/

// グラフィックスクロール(横向き起点)
// x: スクロール開始位置 x (1ドット単位)
// y: スクロール開始位置 y (8ドット単位)
// w: スクロール幅 (1ドット単位)
// h: スクロール高さ(8ドット単位)
// d:方向 0:上 1:下 2:右 3:左
//

void tMatrixScreen::gscroll(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t mode) {
  static const uint8_t remap[4][4] = { {0,1,2,3},{2,3,1,0},{1,0,3,2},{3,2,0,1}};
  uint8_t d;                // 取り出しデータ
  
  // 画面の向きによるスクロール方向補正
  mode = remap[this->matrix.getRotation()][mode];  // スクロール方向の補正
  switch (this->matrix.getRotation()) {            // x,y,w,hの補正
    case 1:
      swap(x,y);
      swap(w,h);
      x = MATRIX_WIDTH-x-w;
      break;
    case 2:
      x = MATRIX_WIDTH-x-w;
      y = MATRIX_HEIGHT-y-h;
      break;
    case 3:
      swap(x,y);
      swap(w,h);
      y = MATRIX_HEIGHT-y-h;
      break;
  }
  
  switch(mode) {
  case 0: // 上
    for (int16_t i = x; i < x+w; i++) {     // 横ループ
      for (int16_t j = y+1; j < y+h; j++) { // 縦ループ
        d = this->matrix.getPixel(i,j);     // ドットの取得
        this->matrix.drawPixel(i,j-1,d);            
      }
      this->matrix.drawPixel(i,y+h-1,0);
    }
    break;
    
  case 1: // 下
    for (int16_t i = x; i < x+w; i++) {     // 横ループ
      for (int16_t j = y+h-1; j > y; j--) { // 縦ループ
        d = this->matrix.getPixel(i,j-1);   // ドットの取得
        this->matrix.drawPixel(i,j,d);            
      }
      this->matrix.drawPixel(i,y,0);
    }
    break;
    
  case 2: // 右
    for (int16_t j = y; j < y+h; j++) {     // 縦ループ
      for (int16_t i = x+w-1; i > x; i--) { // 横ループ
        d = this->matrix.getPixel(i-1,j);   // ドットの取得
        this->matrix.drawPixel(i,j,d);            
      }
      this->matrix.drawPixel(x,j,0);
    }
    break;
    
  case 3: // 左
    for (int16_t j = y; j < y+h; j++) {     // 縦ループ
      for (int16_t i = x+1; i < x+w; i++) { // 横ループ
        d = this->matrix.getPixel(i,j);     // ドットの取得
        this->matrix.drawPixel(i-1,j,d);            
      }
      this->matrix.drawPixel(x+w-1,j,0);
    }
    break;
  }
} 


// 文字のスクロールイン表示
// c    : 文字コード
void tMatrixScreen::scrollIn(uint8_t c) {
  switch(msg_dir) {
  case 0:// 下から上にスクロールイン
    for (int16_t y=6; y>=0; y--) {
      gscroll(0, 0, 5, 5, 0);
      set_gcursor(0,y);
      gputch(c);
      delay(msg_tm);
    }  
    break;
  case 1:// 上から下にスクロールイン
    for (int16_t y=-6; y<=0; y++) {
      gscroll(0, 0, 5, 5, 1);
      set_gcursor(0,y);
      gputch(c);
      delay(msg_tm);
    }  
    break;
  case 2:// 右から左にスクロールイン
    for (int16_t x=-6; x<=0; x++) {
      gscroll(0, 0, 5, 5, 2);
      set_gcursor(x,0);
      gputch(c);
      delay(msg_tm);
    }
    break;    
  case 3:// 左から右にスクロールイン
    for (int16_t x=6; x>=0; x--) {
      gscroll(0, 0, 5, 5, 3);
      set_gcursor(x,0);
      gputch(c);
      delay(msg_tm);
    }  
    break;
  case 4:// スクロールなし
    set_gcursor(0,0);
    gputch(c);
    delay(msg_tm);
    break;
  }
}

void tMatrixScreen::set_msg(uint8_t dir, uint16_t tm) {
  msg_dir = dir;
  msg_tm   = tm;
}  

void tMatrixScreen::driveMatrix(uint8_t sw, uint8_t flgCls) {
  if (flgCls) {
    cls();delay(100);
  }
  if (sw) {
    matrix.restartTimer();
  } else {    
    matrix.stopTimer();
    digitalWrite(26,0);
    digitalWrite(27,0);
    digitalWrite(28,0);
  }
}
