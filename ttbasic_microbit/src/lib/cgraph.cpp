
#include "cgraph.h"
#include "tTermscreen.h"    // シリアルコンソールクラス
extern tTermscreen sc1;     // ターミナルスクリーン

// コンストラクタ
CGraph::CGraph(int16_t w, int16_t h) : Adafruit_GFX(w,h) {
  _dotChar = '*';
}

// ディストラクタ
CGraph::~CGraph(void) {

}

// 点の描画
void CGraph::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;
  if ((_dotChar > 0xff)) x<<=1;
  sc1.locate(x, y);
  sc1.putwch(_dotChar);
}


