//
// file: tMatrixScreen.h
// micro:bit Matrixスクリーン制御クラス
// 2017/12/28 作成 by たま吉さん
//

#ifndef __tMatrixScreen_h__
#define __tMatrixScreen_h__

#include "tGraphicDev.h"
#include <Adafruit_Microbit_Matrix.h>

#define MATRIX_HEIGHT 5
#define MATRIX_WIDTH  5

class tMatrixScreen :public tGraphicDev {
 private:
  Adafruit_Microbit_Matrix matrix;
  
  uint16_t f_width;    // フォント幅(ドット)
  uint16_t f_height;   // フォント高さ(ドット)
  uint16_t fgcolor;
  uint16_t bgcolor;
  uint16_t fontEx;     // フォント拡大率
  int16_t pos_gx;      // グラフィックカーソル
  int16_t pos_gy;      // グラフィックカーソル
	uint8_t* font;       // 利用フォント(tTVscreenと同じ)
  uint8_t msg_dir;
  uint16_t msg_tm;

 protected:
/*
  void INIT_DEV();                             // デバイスの初期化
    void WRITE(uint8_t x, uint8_t y, uint8_t c); // 文字の表示
    void CLEAR();                                // 画面全消去
    void CLEAR_LINE(uint8_t l);                  // 行の消去
    void SCROLL_UP();                            // スクロールアップ
    void SCROLL_DOWN();                          // スクロールダウン
    void INSLINE(uint8_t l);                     // 指定行に1行挿入(下スクロール)
    void scrollFrame(uint16_t vsp);              // 指定利用のスクロール
*/
  void drawBitmap_x2(int16_t x, int16_t y,
           const uint8_t *bitmap, int16_t w, int16_t h,
           uint16_t color, uint16_t ex, uint8_t f=0);

  void colorDrawBitmap(int16_t x, int16_t y,
           const uint8_t *bitmap, int16_t w, int16_t h,
           uint16_t ex, uint8_t f=0);

  
 public:
    //virtual void init(const uint8_t* fnt, uint16_t ln=256, uint8_t kbd_type=false, uint8_t* extmem=NULL, uint8_t vmode=TV_FONT_EX, uint8_t rt=3, uint8_t ifmode=0);
    boolean begin(void);
    void setColor(uint16_t fc, uint16_t bc);   // 文字色指定
    void setAttr(uint16_t attr);               // 文字属性

    //uint8_t drawCurs(uint8_t x, uint8_t y);    // カーソル表示
	  //virtual void setScreen(uint8_t mode,uint8_t rt=3);   // スクリーンモード設定
	
	 // グラフィック描画
	  void  ginit() {};
	  inline uint8_t *getfontadr(){return 0;};// フォントアドレスの参照
    uint8_t* getGRAM();
	  uint16_t getGRAMsize();
    void     cls();
    void     pset(int16_t x, int16_t y, uint16_t c);
    void     line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c);
    void     circle(int16_t x, int16_t y, int16_t r, uint16_t c, int8_t f);
    void     rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c, int8_t f);
    void     bitmap(int16_t x, int16_t y, uint8_t* adr, uint16_t index, uint16_t w, uint16_t h, uint16_t d, uint8_t rgb=0);
    int16_t  gpeek(int16_t x, int16_t y);
    int16_t  ginp(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t c);
    void     set_gcursor(int16_t, int16_t);
    void     gputch(uint8_t c);    
    void     cscroll(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t d) {};
    void     gscroll(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t mode);
    void     scrollIn(uint8_t c);
    void     set_msg(uint8_t dir, uint16_t tm);
    void     driveMatrix(uint8_t sw);
};

#endif
