//
// 豊四季Tiny BASIC for micro:bit グラフィック描画デバイス
// 修正日 2018/01/05, micro:bit用に修正
//

#ifndef __tGraphicDev_h__
#define __tGraphicDev_h__

#include <Arduino.h>

class tGraphicDev {
  protected:
    uint16_t g_width;             // スクリーングラフィック横サイズ
    uint16_t g_height;            // スクリーングラフィック縦サイズ

  public:
    virtual void  ginit() = 0;
    virtual inline uint8_t *getfontadr() = 0;                     // フォントアドレスの参照
	virtual uint16_t getGWidth()  { return g_width;} ;            // グラフックスクリーン横幅取得
    virtual uint16_t getGHeight() { return g_height;} ;           // グラフックスクリーン縦幅取得

    // グラフィック描画
    virtual uint8_t* getGRAM()  = 0;
    virtual uint16_t getGRAMsize()  = 0;
    virtual void pset(int16_t x, int16_t y, uint16_t c) = 0;
    virtual void line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t c) = 0;
    virtual void circle(int16_t x, int16_t y, int16_t r, uint16_t c, int8_t f) = 0;
    virtual void rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c, int8_t f)  = 0;
    virtual void bitmap(int16_t x, int16_t y, uint8_t* adr, uint16_t index, uint16_t w, uint16_t h, uint16_t d, uint8_t rgb=0)  = 0;
    virtual void gscroll(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t mode)  = 0;
    virtual void cscroll(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t d) = 0; 
    virtual int16_t gpeek(int16_t x, int16_t y)  = 0;
    virtual int16_t ginp(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t c)  = 0;
    virtual void set_gcursor(int16_t, int16_t)  = 0;
    virtual void gputch(uint8_t c)  = 0;

};

#endif

