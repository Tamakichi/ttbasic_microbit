//
// フラッシュメモリ書き込みライブラリ
// 2017/12/25 by たま吉さん
// 参考文献
//  nRF51 Series Reference Manual Version 3.0 「6 Non-Volatile Memory Controller (NVMC)」
//

#ifndef __FLASH_H__
#define __FLASH_H__

#include <Arduino.h>

// 引数
//   addr 消去対象ページ先頭アドレス
// 戻り値
//   0:正常終了
//   1:異常終了(指定アドレスが正しくない)
//
uint8_t  flashPageErase(uint32_t addr);

// フラッシュメモリ32ビットワードデータ書き込み
// 引数
//  addr: 書込みアドレス(4バイト境界アドレスであること)
//  ptr : 書込みデータ格納アドレス(4バイト境界アドレスであること)
//  len : 書込みデータ長さ
// 戻り値
//   0:正常終了
//   1:異常終了(指定アドレスが正しくない)
//
uint8_t flashWriteData(uint32_t* addr, uint32_t *ptr, uint16_t len);

#endif
