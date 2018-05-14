//
// 豊四季Tiny BASIC for micro:bit フラッシュメモリ統合管理クラス
// 2017/12/26 by たま吉さん Arduino STM32版 V0.85βからの移植
//

#include "flash.h"
#include "ttbasic_error.h"
#include "tFlashMan.h"
#define FLASH_START_ADDRESS    ((uint32_t)(0x0000000))   // micro:bit フラッシュメモリ先頭アドレス
#define FLATH_EEPROM_USE       0                         // 仮想EEPROM利用ページ数

// 初期設定
//   フラッシュメモリ利用のための初期化を行う
// 引数
//   totalPageNum  ：総ページ数
//   pageSize      ：1ページ内バイト数
//   prgPageNum    ：1プログラム当たりのページ数
//
void tFlashMan::init(uint16_t totalPageNum, uint16_t pageSize, uint16_t maxPrgNum, uint16_t prgPageNum) {
  _totalPageNum = totalPageNum ; // 総ページ数
  _pageSize     = pageSize ;     // 1ページ内バイト数
  _maxPrgNum    = maxPrgNum;     // プログラム保存可能数
  _prgPageNum   = prgPageNum ;   // 1プログラム当たりのページ数
  
  // プログラム保存先頭ページ番号の計算
  _topPrgPageNo = _totalPageNum - _prgPageNum * _maxPrgNum - FLATH_EEPROM_USE;
}

// 指定プログラム 格納先頭アドレスの取得
// 引数
//  prgNo  :プログラム番号
// 戻り値
//  格納先頭アドレスの取得
uint8_t* tFlashMan::getPrgAddress(uint8_t prgNo) {
  return (uint8_t*)(FLASH_START_ADDRESS + (uint32_t)(_topPrgPageNo+prgNo*_prgPageNum) * (uint32_t)_pageSize); 
}

// 指定プログラムの消去
// 引数
//  prgNo   :プログラム番号
// 戻り値
//  0:正常終了 0以外異常
uint8_t tFlashMan::eraseProgram(uint8_t prgNo) {
  uint8_t status = 0;
  uint32_t flash_adr = (uint32_t)getPrgAddress(prgNo); // アドレスの取得  
  for (uint8_t i = 0; i < _prgPageNum; i++) {
    status = flashPageErase(flash_adr + _pageSize*i );
      if (status) {
         break;
      }
  }
  return status;
}

// 指定プログラムの保存
// 引数
//  prgNo   :プログラム番号
//  prgData :プログラム格納アドレス
// 戻り値
//  0:正常終了 0以外異常
uint8_t tFlashMan::saveProgram(uint8_t prgNo, uint8_t* prgData) {
  uint32_t flash_adr = (uint32_t)getPrgAddress(prgNo);
  // フラッシュメモリ消去
  if (eraseProgram(prgNo)) 
    return 1;
  
  // 内部フラッシュメモリへの保存
  flashWriteData((uint32_t*)flash_adr, (uint32_t*)prgData, _pageSize*_prgPageNum/4);
  return 0;
}

// 指定プログラムの有無のチェック
// 引数
//  prgNo   :プログラム番号
// 戻り値
//  1:有り 0 無し

uint8_t tFlashMan::isExistPrg(uint8_t prgNo) {
  // 指定領域に保存されているかチェックする
  uint8_t* flash_adr = getPrgAddress(prgNo);
  if ( *flash_adr == 0xff && *(flash_adr+1) == 0xff)
    return false;
  else 
    return true;
}

// 指定プログラムのロード
// 引数
//  prgNo   :プログラム番号
//  prgData :プログラム格納アドレス
// 戻り値
//  0:正常終了 0以外異常
uint8_t tFlashMan::loadProgram(uint8_t prgNo, uint8_t* prgData) {
 uint8_t* flash_adr = getPrgAddress(prgNo);

  // 指定領域に保存されているかチェックする
  if (!isExistPrg(prgNo)) {
    return ERR_NOPRG;
  }
  // 現在のプログラムの削除とロード
  memcpy(prgData , flash_adr, _pageSize * _prgPageNum);
  return 0;
}
