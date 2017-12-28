//
// file: tMatrixScreen.cpp
// SH1106/SSD1306/SSD1309利用ターミナルスクリーン制御クラス
// 2017/09/14 作成
//

#include <string.h>
#include "tMatrixScreen.h"
#include <Fonts/TomThumb.h>
  
#ifndef swap
#define swap(a, b) { int16_t t = a; a = b; b = t; }
#endif

// GVRAMサイズ取得
__attribute__((always_inline)) uint16_t tMatrixScreen::getGRAMsize() {
  //return this->g_width*(this->g_height>>3);
  g_width*g_height;
}

// グラフィク表示用メモリアドレス参照
__attribute__((always_inline)) uint8_t* tMatrixScreen::getGRAM() {
  return (uint8_t*)this->matrix.matrix_buffer;	
}

boolean tMatrixScreen::begin(void) {
  g_width  = 5;
  g_height = 5;
  fgcolor  = 1;
  bgcolor  = 0;
  fontEx   = 1;     // フォント拡大率
  pos_gx   = 0;     // グラフィックカーソル
  pos_gy   = 0;     // グラフィックカーソル

  matrix.begin();
  matrix.setFont(&TomThumb);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  return true;
}

/*
void tMatrixScreen::update() {
  this->matrix.display();
}  
*/
/*
//
// 初期化 - OLEDディスプレイの初期化を行う
//  fnt      : 表示フォント指定
//  ln       : 行バッファサイズ
//  kbd_type : キーボードタイプ(0:JP 1:US)
//  extmem   : VRAM領域指定(0:初期化時に動的確保 0以外:指定値をアドレス先頭とする)
//  vmode    : スクリーンモード(1～3)
//  rt       : 画面回転(0～3)
//  ifmode   : インタフェース(0:I2C 1:SPI)
//
void tMatrixScreen::init(const uint8_t* fnt, uint16_t ln, uint8_t kbd_type, uint8_t* extmem, uint8_t vmode, uint8_t rt,uint8_t ifmode) {
  this->font = (uint8_t*)fnt;
  if (ifmode == 0) {
#if OLED_DEV == 0
    this->oled = new Adafruit_SH1106(OLED_RST); // Use hardware I2C1
    this->matrix.begin(SH1106_SWITCHCAPVCC, SH1106_I2C_ADDRESS); 
#else
    this->oled = new Adafruit_SSD1306(OLED_RST); // Use hardware I2C1
    this->matrix.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS); 
#endif    
  } else {
#if OLED_DEV == 0
    this->oled = new Adafruit_SH1106(OLED_DC, OLED_RST, OLED_CS, 2); // Use hardware SPI2
    this->matrix.begin();
#else
    this->oled = new Adafruit_SSD1306(OLED_DC, OLED_RST, OLED_CS, 2); // Use hardware SPI2
    this->matrix.begin();
#endif    
  }

  setScreen(vmode, rt); // スクリーンモード,画面回転指定
  if (extmem == NULL) {
    tscreenBase::init(this->width,this->height, ln);
  } else {
    tscreenBase::init(this->width,this->height, ln, extmem);
  }	
 
#if PS2DEV == 1
  setupPS2(kbd_type);
#endif

 // シリアルからの制御文字許可
  this->allowCtrl = true;
}


// 依存デバイスの初期化
void tMatrixScreen::INIT_DEV() {

}

// スクリーンモード設定
void tMatrixScreen::setScreen(uint8_t mode, uint8_t rt) {
  this->matrix.setRotation(rt);
  this->g_width  = this->matrix.width();             // 横ドット数
  this->g_height = this->matrix.height();            // 縦ドット数

  this->fontEx = mode;
  this->f_width  = *(font+0)*this->fontEx;         // 横フォントドット数
  this->f_height = *(font+1)*this->fontEx;         // 縦フォントドット数
  this->width  = this->g_width  / this->f_width;   // 横文字数
  this->height = this->g_height / this->f_height;  // 縦文字数
  this->fgcolor = OLED_WHITE;
  this->bgcolor = OLED_BLACK;
  this->matrix.setCursor(0, 0);
  pos_gx =0;  pos_gy =0;
}

// カーソル表示
uint8_t tMatrixScreen::drawCurs(uint8_t x, uint8_t y) {
  uint8_t c;
  c = VPEEK(x, y);
  this->matrix.fillRect(x*f_width,y*f_height,f_width,f_height,fgcolor);
  if (fontEx == 1)
    this->matrix.drawBitmap(x*f_width,y*f_height,font+3+((uint16_t)c)*8,f_width,f_height, bgcolor);
  else
	  drawBitmap_x2(x*f_width,y*f_height,font+3+((uint16_t)c)*8,f_width/fontEx, f_height/fontEx, bgcolor, fontEx);
  this->matrix.display();
  return 0;
}

// 文字の表示
void tMatrixScreen::WRITE(uint8_t x, uint8_t y, uint8_t c) {
  this->matrix.fillRect(x*f_width,y*f_height,f_width,f_height, bgcolor);
  if (fontEx == 1)
    this->matrix.drawBitmap(x*f_width,y*f_height,font+3+((uint16_t)c)*8,f_width,f_height, fgcolor);
  else
    drawBitmap_x2(x*f_width,y*f_height,font+3+((uint16_t)c)*8,f_width/fontEx,f_height/fontEx, fgcolor, fontEx);
}
*/

// グラフィックカーソル設定
void tMatrixScreen::set_gcursor(uint16_t x, uint16_t y) {
  this->pos_gx = x;
  this->pos_gy = y;
}

// グラフィック文字表示
void  tMatrixScreen::gputch(uint8_t c) {
#if 0
  drawBitmap_x2( this->pos_gx, this->pos_gy, this->font+3+((uint16_t)c)*8,
    	           this->f_width/ this->fontEx,  this->f_height/this->fontEx,  this->fgcolor, this->fontEx);

  this->pos_gx += this->f_width;
  if (this->pos_gx + this->f_width >= this->g_width) {
     this->pos_gx = 0;
     this->pos_gy += this->f_height;
  }
  //this->matrix.display();
#endif
};


// 画面全消去
void tMatrixScreen::cls() {
  this->matrix.fillScreen(bgcolor);
  //this->matrix.display();
  pos_gx =0;  pos_gy =0;
}
/*
// 行の消去
void tMatrixScreen::CLEAR_LINE(uint8_t l) {
  this->matrix.fillRect(0,l*f_height,g_width,f_height,bgcolor);
  this->matrix.display();
}

// スクロールアップ
void tMatrixScreen::SCROLL_UP() {
  for (uint8_t cnt = 0; cnt < this->fontEx; cnt++) {
    switch ( this->matrix.getRotation() ) {
    case 0: // 横
      memmove(this->matrix.VRAM(), this->matrix.VRAM() + MATRIX_WIDTH, MATRIX_WIDTH*7);
      memset(this->matrix.VRAM()+MATRIX_WIDTH*7,0,MATRIX_WIDTH);
      break;
    case 2: // 横（逆)
      memmove(this->matrix.VRAM()+MATRIX_WIDTH, this->matrix.VRAM(), MATRIX_WIDTH*7);
      memset(this->matrix.VRAM(),0,MATRIX_WIDTH);
      break;
    case 1: // 縦
      for (uint16_t i=0; i <8; i++) {
        memmove(this->matrix.VRAM() + MATRIX_WIDTH*i + this->f_height ,this->matrix.VRAM() + MATRIX_WIDTH*i, MATRIX_WIDTH - this->f_height);
        memset(this->matrix.VRAM()+MATRIX_WIDTH*i, 0, this->f_height);
      }
      break;
    case 3: // 縦(逆)
      for (uint16_t i=0; i < 8; i++) {
        memmove(this->matrix.VRAM() + MATRIX_WIDTH*i ,this->matrix.VRAM() + MATRIX_WIDTH*i + this->f_height, MATRIX_WIDTH - this->f_height);
        memset(this->matrix.VRAM() + MATRIX_WIDTH*i + MATRIX_WIDTH - this->f_height, 0, this->f_height);
      }
      break;
    }
  }  
  this->matrix.display();
}

// スクロールダウン
void tMatrixScreen::SCROLL_DOWN() {
  for (uint8_t cnt = 0; cnt < this->fontEx; cnt++) {
    switch ( this->matrix.getRotation() ) {
    case 2: // 横
      memmove(this->matrix.VRAM(), this->matrix.VRAM() + MATRIX_WIDTH, MATRIX_WIDTH*7);
      memset(this->matrix.VRAM() + MATRIX_WIDTH*7,0, MATRIX_WIDTH);
      break;
    case 0: // 横（逆)
      memmove(this->matrix.VRAM() + MATRIX_WIDTH, this->matrix.VRAM(), MATRIX_WIDTH*7);
      memset(this->matrix.VRAM(), 0, MATRIX_WIDTH);
      break;
    case 3: // 縦
      for (uint16_t i=0; i < 8; i++) {
        memmove(this->matrix.VRAM() + MATRIX_WIDTH*i + this->f_height ,this->matrix.VRAM() + MATRIX_WIDTH*i, MATRIX_WIDTH - this->f_height);
        memset(this->matrix.VRAM() + MATRIX_WIDTH*i, 0, this->f_height);
      }
      break;
    case 1: // 縦(逆)
      for (uint16_t i=0; i < 8; i++) {
        memmove(this->matrix.VRAM() + MATRIX_WIDTH*i ,this->matrix.VRAM() + MATRIX_WIDTH*i+this->f_height, MATRIX_WIDTH - this->f_height);
        memset(this->matrix.VRAM() + MATRIX_WIDTH*i + MATRIX_WIDTH - this->f_height, 0, this->f_height);
      }
      break;
    }  
  }
  this->matrix.display();
}

// 指定行に1行挿入(下スクロール)
void tMatrixScreen::INSLINE(uint8_t l) {
  refresh();
}
*/
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
  //this->matrix.display();
}

// 線の描画
void tMatrixScreen::line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c){
  this->matrix.drawLine(x1, y1, x2, y2, c);
  //this->matrix.display();
}

// 円の描画
void tMatrixScreen::circle(int16_t x, int16_t y, int16_t r, uint16_t c, int8_t f) {
 if(f)
   this->matrix.fillCircle(x, y, r, c);
 else
   this->matrix.drawCircle(x, y, r, c);
  //this->matrix.display();
}

// 四角の描画
void tMatrixScreen::rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c, int8_t f) {
  if(f)
   this->matrix.fillRect(x, y, w, h, c);
 else
   this->matrix.drawRect(x, y, w, h, c);
 //this->matrix.display();
}

// ビットマップの描画
void  tMatrixScreen::bitmap(int16_t x, int16_t y, uint8_t* adr, uint16_t index, uint16_t w, uint16_t h, uint16_t d, uint8_t rgb) {
  uint8_t*bmp;
  bmp = adr + ((w + 7) / 8) * h * index;
  this->drawBitmap_x2(x, y, (const uint8_t*)bmp, w, h, fgcolor, d, 1);
  //this->matrix.display();
}

// 指定座標のドットの取得
int16_t tMatrixScreen::gpeek(int16_t x, int16_t y) {
  //return this->matrix.getPixel(x, y);
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

static const uint8_t remap[4][4] = { {0,1,2,3},{2,3,1,0},{1,0,3,2},{3,2,0,1}};
void tMatrixScreen::gscroll(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t mode) {
  mode = remap[this->matrix.getRotation()][mode]; // 画面の向きによるスクロール方向補正
  switch (this->matrix.getRotation()) {
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
  
  
  uint8_t* bmp = this->getGRAM();     // フレームバッファ参照位置 
  uint16_t top_by = y/8;              // 縦スクロール開始バイト位置
  uint16_t end_by = (y+h+7)/8-1;      // 縦スクロール終了バイト位置
  uint8_t prv_bit;                    // 直前のドット
  uint8_t d,d1,d2,msk;                // 取り出しデータ
  uint16_t addr;                      // データアドレス
    
  switch(mode) {
      case 0: // 上
        for (int16_t i = x; i < x+w; i++) { // 横ループ
          prv_bit = 0;
          for (int16_t j = end_by; j >=top_by ; j--) { // 縦ループ
            addr = j*MATRIX_WIDTH+i;
            d = bmp[addr];
            if (j == top_by && y%8) {             // 上部端数の処理
              msk = ~(0xff<<(y%8));                 // マスクデータ
              d1  =  msk & d;                       // スクロールしない部分
              d2  = (~msk & d)>>1 ;                 // スクロールデータ
              bmp[addr] = d1| d2;                   // 合成データ
            } else if (j == end_by && (y+h)%8) {  // 下部端数の処理
              msk = 0xff<<((y+h)%8);                // マスクデータ
              d1 =  msk & d;                        // スクロールしない部分
              d2 = (~msk & d)>>1;                   // スクロールする分のデータ
              bmp[addr] = d1| d2;                   // 合成データ
            } else {
              bmp[addr]>>=1;                      // 端数でない処理
            }
            bmp[addr] |= (prv_bit<<7);            // キャリーアウトビットの付加
            prv_bit = d & 1;
          }
        }
        break;
      case 1: // 下
        for (int16_t i = x; i < x+w; i++) { // 横ループ
          prv_bit = 0;
          for (int16_t j = top_by; j <=end_by ; j++) { // 縦ループ
            addr = j*MATRIX_WIDTH+i;
            d = bmp[addr];
            if (j == top_by && y%8) {             // 上部端数の処理
              msk = ~(0xff<<(y%8));                 // マスクデータ
              d1  =  msk & d;                       // スクロールしない部分
              d2  = (~msk & d)<<1 ;                 // スクロールデータ
              bmp[addr] = d1| d2;                   // 合成データ
            } else if (j == end_by && (y+h)%8) {  // 下部端数の処理
              msk = 0xff<<((y+h)%8);                // マスクデータ
              d1 =  msk & d;                        // スクロールしない部分
              d2 = (~msk & d)<<1;                   // スクロールする分のデータ
              bmp[addr] = d1| d2;                   // 合成データ
            } else {
              bmp[addr]<<=1;                      // 端数でない処理
            }
            bmp[addr] |= (prv_bit>>7);            // キャリーアウトビットの付加
            prv_bit = d & 0x80;
          }
        }
        break;
      case 2: // 右
        for (int16_t j = top_by; j <= end_by; j++) { // 縦ループ
          addr = MATRIX_WIDTH*j+x;
          if ((j == top_by) && y%8) {             // 上部端数の処理
            msk = ~(0xff<<(y%8));                   // マスクデータ
            for (int16_t i = w-1; i >= 1; i--) {
              d1  =  msk & bmp[addr+i];             // スクロールしない部分
              d2  = ~msk & bmp[addr+i-1] ;          // スクロールデータ
              bmp[addr+i] = d1| d2;                 // 合成データ
            }
            bmp[addr] &= msk;
          } else if (j == end_by && (y+h)%8) {  // 下部端数の処理
            msk = 0xff<<((y+h)%8);                  // マスクデータ
            for (int16_t i = w-1; i >= 1; i--) {
              d1  =  msk & bmp[addr+i];             // スクロールしない部分
              d2  = ~msk & bmp[addr+i-1] ;          // スクロールデータ
              bmp[addr+i] = d1| d2;                 // 合成データ
            }
            bmp[addr] &= msk;
          } else {                              // 端数でない処理
            for (uint16_t i =w-1; i >= 1; i--) {
               bmp[addr+i] = bmp[addr+i-1];
            }
            bmp[addr]=0;
          }  
        }
        break;
      case 3: // 左
        for (int16_t j = top_by; j <= end_by; j++) { // 縦ループ
          addr = MATRIX_WIDTH*j+x;
          if ((j == top_by) && y%8) {             // 上部端数の処理
            msk = ~(0xff<<(y%8));                   // マスクデータ
            for (uint16_t i =1; i <= w-1; i++) {
              d1  =  msk & bmp[addr+i-1];           // スクロールしない部分
              d2  = ~msk & bmp[addr+i] ;            // スクロールデータ
              bmp[addr+i-1] = d1| d2;                 // 合成データ
            }
            bmp[addr+w-1] &= msk;
          } else if (j == end_by && (y+h)%8) {  // 下部端数の処理
            msk = 0xff<<((y+h)%8);                  // マスクデータ
            for (uint16_t i =1; i <= w-1; i++) {
              d1  =  msk & bmp[addr+i-1];           // スクロールしない部分
              d2  = ~msk & bmp[addr+i] ;            // スクロールデータ
              bmp[addr+i-1] = d1| d2;                 // 合成データ
            }
            bmp[addr+w-1] &= msk;
          } else {                              // 端数でない処理
            for (uint16_t i =1; i <= w-1; i++) {
               bmp[addr+i-1] = bmp[addr+i];
            }
            bmp[addr+w-1]=0;
          }  
        }
        break;
  
  } 
  //this->matrix.display();
} 
