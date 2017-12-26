//
// フラッシュメモリ書き込みライブラリ
// 2017/12/25 by たま吉さん
// 参考文献
//  nRF51 Series Reference Manual Version 3.0 「6 Non-Volatile Memory Controller (NVMC)」
//

#include <Arduino.h>
#include "nrf.h"

//
// フラッシュメモリ指定ページ消去
// フラッシュメモリアドレスは、0x00000000 ～ 0x0003FFFFの範囲とする
// 1ページのサイズは1024バイトとし、アドレスは
//
// 引数
//   addr 消去対象ページ先頭アドレス
// 戻り値
//   0:正常終了
//   1:異常終了(指定アドレスが正しくない)
//
uint8_t  flashPageErase(uint32_t addr) {
  if ( addr >= 0x00040000 )
    return 1;  // フラッシュメモリ領域外
  if ( addr % 1024 )
    return 1;  // ページ先頭アドレスでない
  
  // 書込みアンロック
  NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Een;
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

  // 指定ページの消去
  NRF_NVMC->ERASEPAGE = addr;
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

  // 書込みロック
  NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
  return 0;
}

// フラッシュメモリ32ビットワードデータ書き込み
// 引数
//  addr: 書込みアドレス(4バイト境界アドレスであること)
//  ptr : 書込みデータ格納アドレス(4バイト境界アドレスであること)
//  len : 書込みデータ長さ
// 戻り値
//   0:正常終了
//   1:異常終了(指定アドレスが正しくない)
//
uint8_t flashWriteData(uint32_t* addr, uint32_t *ptr, uint16_t len) {

  if ( ((uint32_t)addr) >= 0x00040000 )
    return 1;  // フラッシュメモリ領域外
  if ( ((uint32_t)addr) % 1024 )
    return 1;  // ページ先頭アドレスでない
  if (((uint32_t)ptr)%4)
    return 1;  // 4バイト境界アドレスでない
    
  // 書込み有効設定
  NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy);

  // データ書き込みループ
  for (uint32_t i = 0; i<len; i++) {
    *addr++ = *ptr++;
    //Serial.println(i,DEC); 
    while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
  }

  // 書込み無効設定
  NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
  while (NRF_NVMC->READY == NVMC_READY_READY_Busy);
  return 0;
}


