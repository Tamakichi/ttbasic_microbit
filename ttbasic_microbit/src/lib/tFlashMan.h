//
// 豊四季Tiny BASIC for micro:bit フラッシュメモリ統合管理クラス
// 2017/12/26 by たま吉さん Arduino STM32版 V0.85βからの移植
//

#ifndef __tFlashMan_h__
#define __tFlashMan_h__

#include <Arduino.h>
#include "ttbasic_types.h"

// *** システム設定関連 **************
class tFlashMan {
 private:
  uint16_t _totalPageNum;  // ページ総数
  uint16_t _pageSize;      // 1ページ内バイト数
  uint16_t _prgPageNum;    // 1プログラム当たりのページ数
  uint16_t _maxPrgNum;     // プログラム保存可能数
  uint16_t _topPrgPageNo;  // プログラム保存用先頭ページ番号
 
 public:

  // コンストラクタ
  tFlashMan(uint16_t totalPageNum, uint16_t pageSize,
  uint16_t maxPrgNum, uint16_t prgPageNum )
  { init(totalPageNum,pageSize,maxPrgNum, prgPageNum); };
   
  // 初期設定
  void init(uint16_t totalPageNum,uint16_t pageSize,
            uint16_t maxPrgNum,uint16_t prgPageNum) ;  
  
  // 総ページ数の取得
  uint16_t getTotalPageNum()
    {return _totalPageNum; };
  
  // 1ページ内バイト数取得
  uint16_t getPageSize()
    {return _pageSize; };
  
  // １プログラム当たりのページ数の取得
  uint16_t getPrgPageNum()
    {return _prgPageNum; };

  // 指定プログラム 格納先頭アドレスの取得
  // prgNo :  プログラム番号
  uint8_t* getPrgAddress(uint8_t prgNo);

  // 指定プログラムの有無のチェック
  //  prgNo : プログラム番号
  uint8_t isExistPrg(uint8_t prgNo);
  
  // 指定プログラムの削除
  //  prgNo : プログラム番号
  uint8_t eraseProgram(uint8_t prgNo);

  // 指定プログラムの保存
  //  prgNo : プログラム番号, prgData : プログラム格納アドレス
  uint8_t saveProgram(uint8_t prgNo,uint8_t* prgData);
  
  // 指定プログラムのロード
  //  prgNo : プログラム番号, prgData : プログラム格納アドレス
  uint8_t loadProgram(uint8_t prgNo,uint8_t* prgData);

};
#endif
