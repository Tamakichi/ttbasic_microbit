// 
// スクリーン制御基本クラス ヘッダーファイル
// 作成日 2017/06/27 by たま吉さん
// 修正日 2017/08/05 ファンクションキーをNTSC版同様に利用可能対応
// 修正日 2017/08/12 edit_scrollUp() で最終行が2行以上の場合の処理ミス修正
// 修正日 2017/10/15 定義競合のためKEY_F1、KEY_F(n)をKEY_Fn1、KEY_Fn(n)変更
// 修正日 2018/01/30 制御キーのキーコード変更、全角（シフトJIS)対応
// 修正日 2018/01/30 [F7]:行の分割、[F8]:行の結合操作の追加
// 修正日 2018/02/02 editLine()の追加
// 修正日 2018/02/10, 基底クラスtSerialDevの廃止対応
// 修正日 2018/02/16, while() をfor(;;)に変更

#include <string.h>
#include <stdlib.h>
#include "tscreenBase.h"

#if DEPEND_TTBASIC == 1
  int16_t getPrevLineNo(int16_t lineno);
  int16_t getNextLineNo(int16_t lineno);
  char* getLineStr(int16_t lineno);
#endif

char c_isdigit(char c);


// http://katsura-kotonoha.sakura.ne.jp/prog/c/tip00010.shtml
//*********************************************************
// 文字列 str の str[nPos] について、
//   ０ …… １バイト文字
//   １ …… ２バイト文字の一部（第１バイト）
//   ２ …… ２バイト文字の一部（第２バイト）
// のいずれかを返す。
//*********************************************************
#define jms1(c) (((0x81<=c)&&(c<=0x9F))||((0xE0<=c)&&(c<=0xFC))) 
#define jms2(c) ((0x7F!=c)&&(0x40<=c)&&(c<=0xFC))
int isJMS( uint8_t *str, uint16_t nPos ) {
	int i;
	int state; // { 0, 1, 2 }

	state = 0;
	for( i = 0; str[i] != '\0'; i++ )	{
		if      ( ( state == 0 ) && ( jms1( str[i] ) ) ) state = 1; // 0 -> 1
		else if ( ( state == 1 ) && ( jms2( str[i] ) ) ) state = 2; // 1 -> 2
		else if ( ( state == 2 ) && ( jms1( str[i] ) ) ) state = 1; // 2 -> 1
		else                                             state = 0; // 2 -> 0, その他
		// str[nPos] での状態を返す。
		if ( i == nPos ) return state;
	}
	return 0;
}//isJMS

// スクリーンの初期設定
// 引数
//  w      : スクリーン横文字数
//  h      : スクリーン縦文字数
//  l      : 1行の最大長
//  extmem : 外部獲得メモリアドレス NULL:なし NULL以外 あり
// 戻り値
//  なし
void tscreenBase::init(uint16_t w, uint16_t h, uint16_t l,uint8_t* extmem) {
  width   = w;
  height  = h;
  maxllen = l;
  flgCur = 0;
  
  // デバイスの初期化
  INIT_DEV();

  // 直前の獲得メモリの開放
  if (!flgExtMem) {
  	if (screen != NULL) {
      free(screen);
    }
  }

  // スクリーン用バッファ領域の設定
  if (extmem == NULL) {
    flgExtMem = 0;
    screen = (uint8_t*)malloc( width * height );
  } else {
     flgExtMem = 1;
  	 screen = extmem;
  }
  
  cls();
  show_curs(true);  
  MOVE(pos_y, pos_x);

  // 編集機能の設定
  flgIns = true;
}

// スクリーン利用終了
void tscreenBase::end() {
  
  // デバイスの終了処理
  END_DEV();

  // 動的確保したメモリーの開放
  if (!flgExtMem) {
    if (screen != NULL) {
      free(screen);
      screen = NULL;
    }
  }
}

// 指定行の1行分クリア
void tscreenBase::clerLine(uint16_t l) {
  memset(screen+width*l, 0, width);
  CLEAR_LINE(l);
  MOVE(pos_y, pos_x);
}

// スクリーンのクリア
void tscreenBase::cls() {
  CLEAR();
  memset(screen, 0, width*height);
  locate(0,0);
}

// スクリーンリフレッシュ表示
void tscreenBase::refresh() {
  for (uint16_t i = 0; i < height; i++)
    refresh_line(i);
  MOVE(pos_y, pos_x);
}

void tscreenBase::refresh_line(uint16_t l) {
  CLEAR_LINE(l);
  for (uint16_t j = 0; j < width; j++) {
    if( IS_PRINT( VPEEK(j,l) )) { 
      WRITE(VPEEK(j,l));
    }
  }
}

// 1行分スクリーンのスクロールアップ
void tscreenBase::scroll_up() {
  memmove(screen, screen + width, (height-1)*width);
  draw_cls_curs();
  SCROLL_UP();
  clerLine(height-1);
  MOVE(pos_y, pos_x);
}

// 1行分スクリーンのスクロールダウン
void tscreenBase::scroll_down() {
  memmove(screen + width, screen, (height-1)*width);
  draw_cls_curs();
  SCROLL_DOWN();
  clerLine(0);
  MOVE(pos_y, pos_x);
}

// 指定行に空白行挿入
void tscreenBase::Insert_newLine(uint16_t l) {
  if (l < height-1) {
    memmove(screen+(l+2)*width, screen+(l+1)*width, width*(height-1-l-1));
  }
  memset(screen+(l+1)*width, 0, width);
  INSLINE(l+1);
}

// 指定行を削除
void tscreenBase::deleteLine(uint16_t l) {
  if (l < height-1) {
    memmove(&VPEEK(0,l), &VPEEK(0,l+1), width*(height-1-l));
  }
  memset(&VPEEK(0,height-1), 0, width);
  refresh();
}

// カーソル位置で行を分割する
void tscreenBase::splitLine() {
  uint8_t* start_adr = &VPEEK(pos_x,pos_y); // 現在位置のアドレス取得
  uint8_t* top = start_adr;                 // 分割する文字列の先頭
  uint16_t ln = 0;                          // 分割する文字列長さ
  uint16_t insLine = 0;                     // 挿入する行数
  
  if (!*top) // 0文字分割不能
    return;
  
  while( *top ) { ln++; top++; } // 行端,長さ調査
  if (pos_x+ln > width-1) 
    ln = width - pos_x;
  
  insLine = ln/width + 1;        // 下に挿入する行数
  if (pos_y + insLine > height-1) {
    return; // 分割不能
  }
  
  // 下の行に空白挿入
  for (uint8_t i = 0; i < insLine; i++) {
    Insert_newLine(pos_y);
  }
  
  // 分割行の移動
  memmove(&VPEEK(0,pos_y+1), start_adr, ln);
  
  // 移動元の消去
  memset(start_adr,0,ln);
  
  refresh(); 
}  

// 現在行の末尾に次の行を結合する
void tscreenBase::margeLine() {
  if (pos_y >= height-1)
    return;
  
  uint8_t* start_adr = &VPEEK(0,pos_y);       // 現在位置のアドレス取得
  uint8_t* top = start_adr;                   // 行末の先頭
  uint16_t ln = 0;                            // 結合先文字列長さ
  while( *top ) { ln++; top++; }              // 行端,長さ調査
  
  if (ln > width)
    return;
  
  uint8_t* next_start_adr = &VPEEK(0,pos_y+1);   // 次の行の先頭のアドレス取得
  uint8_t* next_top = next_start_adr;            // 次の行の行末の先頭
  uint16_t next_ln = 0;                          // 次の行の結合する文字列長さ
  while( *next_top ) { next_ln++; next_top++; }  // 行端,長さ調査
  uint16_t offset = 0;
  if (ln + next_ln >= width) {
    offset = width - ln ;
  }  
    
  // 行の結合
  memmove(top, next_start_adr, next_ln);
  
  // 結合元のデータ消去
  if (offset)
    memset(next_start_adr+next_ln-offset, 0,offset);
  else
    memset(next_start_adr, 0,+next_ln);
  refresh(); 
}

// 現在のカーソル位置の文字削除(全角対応)

void tscreenBase::delete_char() {
  uint8_t* start_adr = &VPEEK(pos_x,pos_y);
  uint8_t* top = start_adr;
  uint16_t ln = 0;

  if (!*top) {
    if (pos_y < height-1 && pos_x == 0) {
       // 空白行を詰める
      deleteLine(pos_y);
      refresh();
      return;
    } else {
       return; // 0文字削除不能
    }
  }
    
  while( *top ) { ln++; top++; } // 行端,長さ調査
  if (isShiftJIS(*start_adr) && ln>=2) {
    memmove(start_adr, start_adr + 2, ln-2); // 2文字詰める
    *(top-1) = 0;
    *(top-2) = 0;
  } else if ( ln >=1 ) {
    memmove(start_adr, start_adr + 1, ln-1); // 1文字詰める
    *(top-1) = 0; 
  }

  for (uint8_t i=0; i < (pos_x+ln)/width+1; i++)
    refresh_line(pos_y+i);   
  MOVE(pos_y,pos_x);
  return;
}

// 文字の出力
void tscreenBase::putch(uint8_t c) {
  VPOKE(pos_x, pos_y, c); // VRAMへの書込み
  WRITE(c);
  movePosNextNewChar();
}

// 文字の出力（シフトJIS対応)
void tscreenBase::putwch(uint16_t c) {
  if (c>0xff) {
   VPOKE(pos_x, pos_y, c>>8); // VRAMへの書込み
   VPOKE(pos_x+1, pos_y, c&0xff); // VRAMへの書込み
   WRITE(c>>8); WRITE(c&0xff);
   
   movePosNextNewChar();
   movePosNextNewChar();
  } else {
   VPOKE(pos_x, pos_y, c); // VRAMへの書込み
   WRITE(c);
   movePosNextNewChar();
  }  
}

void tscreenBase::Insert_char(uint16_t c) {  
  uint8_t* start_adr = &VPEEK(pos_x,pos_y);
  uint8_t* last = start_adr;
  uint16_t ln = 0;
  uint8_t clen = (c>0xff) ? 2:1 ; // 文字バイト数
  
  // 入力位置の既存文字列長(カーソル位置からの長さ)の参照
  while( *last ) {
    ln++;
    last++;
  }
  if (ln == 0 || flgIns == false) {
     // 文字列長さが0または上書きモードの場合、そのまま1文字表示
    if (pos_y + (pos_x+ln+clen)/width >= height) {
      // 最終行を超える場合は、挿入前に1行上にスクロールして表示行を確保
      scroll_up();
      start_adr-=width;
      MOVE(pos_y-1, pos_x);
    } else  if ( (pos_x + ln >= width-1) && !VPEEK(width-1,pos_y) ) {
       // 画面左端に1文字を書く場合で、次行と連続でない場合は下の行に1行空白を挿入する
       Insert_newLine(pos_y+(pos_x+ln)/width);       
    }
    putwch(c);
  } else {
     // 挿入処理が必要の場合
    if (pos_y + (pos_x+ln+clen)/width >= height) {
      // 最終行を超える場合は、挿入前に1行上にスクロールして表示行を確保
      scroll_up();
      start_adr-=width;
      MOVE(pos_y-1, pos_x);
    } else  if ( ((pos_x + ln +clen)%width == width-clen) && !VPEEK(pos_x + ln , pos_y) ) {
       // 画面左端に1文字を書く場合で、次行と連続でない場合は下の行に1行空白を挿入する
          Insert_newLine(pos_y+(pos_x+ln)/width);
    }
    // 1文字挿入のために1文字分のスペースを確保
    memmove(start_adr+clen, start_adr, ln);
    if (clen ==1) {
      *start_adr=c; // 確保したスペースに1文字表示
      movePosNextNewChar();

    } else {
      *start_adr     = (c>>8);   // 確保したスペースに1バイト目
      *(start_adr+1) = c & 0xff; // 確保したスペースに2バイト目
      movePosNextNewChar();
      movePosNextNewChar();
    }
    
    // 挿入した行の再表示
    for (uint8_t i=0; i < (pos_x+ln)/width+1; i++)
       refresh_line(pos_y+i);   
    MOVE(pos_y,pos_x);
  }
}

// 改行
void tscreenBase::newLine() {
  int16_t x = 0;
  int16_t y = pos_y+1;
  if (y >= height) {
     scroll_up();
     y--;
   }    
   MOVE(y, x);
}

// カーソルを１文字分次に移動
void tscreenBase::movePosNextNewChar() {
 int16_t x = pos_x;
 int16_t y = pos_y; 
 x++;
 if (x >= width) {
    x = 0;
    y++;        
   if (y >= height) {
      scroll_up();
      y--;
    }    
 }
 MOVE(y, x);
}

// カーソルを1文字分前に移動
void tscreenBase::movePosPrevChar() {
  if (pos_x > 0) {
    if ( IS_PRINT(VPEEK(pos_x-1 , pos_y)) ) {
        // 1つ前の文字が全角2バイト目かをチェック
        if ( (pos_x -2 >= 0) && (isJMS(&VPEEK(0,pos_y),pos_x-1) !=2 ) ) {
          MOVE(pos_y, pos_x-1);
        } else if ( (pos_x -2 >= 0) && isShiftJIS(VPEEK(pos_x-2 , pos_y)) ) {
          // 全角文字対応
          MOVE(pos_y, pos_x-2);
        } else {
          MOVE(pos_y, pos_x-1);
        }
    }
  } else {
   if(pos_y > 0) {
      if ( IS_PRINT(VPEEK(width-1, pos_y-1)) ) {
        if ( isShiftJIS(VPEEK(width-2 , pos_y-1)) ) {
          // 全角文字対応
          MOVE(pos_y-1, width - 2);
        } else {
          MOVE(pos_y-1, width - 1);
        }
      } 
    }
  }
}

// カーソルを1文字分次に移動(全角対応)
void tscreenBase::movePosNextChar() {
  if (pos_x+1 < width) {
    if ( IS_PRINT( VPEEK(pos_x ,pos_y)) ) {
      if ( isShiftJIS(VPEEK(pos_x ,pos_y)) ) {
        // 現在位置が全角1バイト目の場合,2バイト分移動する
        if (pos_x+2 < width) {
          MOVE(pos_y, pos_x+2);
        } else {
          if (pos_y+1 < height) {
            if ( IS_PRINT(VPEEK(0, pos_y + 1)) ) {
              MOVE(pos_y+1, 0);
            }
          }
        }
      } else { 
        MOVE(pos_y, pos_x+1);
      }
    }
  } else {
    if (pos_y+1 < height) {
      if ( IS_PRINT(VPEEK(0, pos_y + 1)) ) {
        MOVE(pos_y+1, 0);
      }
    }
  }
}

// カーソルを次行に移動(全角対応)
void tscreenBase::movePosNextLineChar() {
  if (pos_y+1 < height) {
    if ( IS_PRINT(VPEEK(pos_x, pos_y + 1)) ) {
      // カーソルを真下に移動
      if ( (isJMS(&VPEEK(0,pos_y+1),pos_x) == 2) &&  (pos_x > 0) ) {
         // 真下が全角2バイト目の場合、全角1バイト目にカーソルを移動する 
         MOVE(pos_y+1, pos_x-1);
      } else {
         MOVE(pos_y+1, pos_x);
      }
    } else {
      // カーソルを次行の行末文字に移動
      int16_t x = pos_x;
      for(;;) {
        if (IS_PRINT(VPEEK(x, pos_y + 1)) ) 
           break;  
        if (x > 0)
          x--;
        else
          break;
      }
      if ( !isShiftJIS(VPEEK(x ,pos_y+1)) && x > 0 && isShiftJIS(VPEEK(x-1 ,pos_y+1)) ) {
         MOVE(pos_y+1, x-1);
       } else {
         MOVE(pos_y+1, x);      
       }
    }
  } else if (pos_y+1 == height) {
    edit_scrollUp();    
  }
}

// カーソルを前行に移動(全角対応)
void tscreenBase::movePosPrevLineChar() {
  if (pos_y > 0) {
    if ( IS_PRINT(VPEEK(pos_x, pos_y-1)) ) {
      // カーソルを真上に移動
      if ( (isJMS(&VPEEK(0,pos_y-1),pos_x) == 2) &&  (pos_x > 0) ) {
         // 真上が全角2バイト目の場合、全角1バイト目にカーソルを移動する 
         MOVE(pos_y-1, pos_x-1);
      } else {
         MOVE(pos_y-1, pos_x);
      }
    } else {
      // カーソルの真上に文字が無い場合は、前行の行末文字に移動する
      int16_t x = pos_x;
      for(;;) {
        if (IS_PRINT(VPEEK(x, pos_y - 1)) ) 
           break;  
        if (x > 0)
          x--;
        else
          break;
      }      
      if ( !isShiftJIS(VPEEK(x ,pos_y-1)) && (x > 0) && isShiftJIS(VPEEK(x-1 ,pos_y-1)) ) {
         // 行末が全角2バイト目の場合、カーソルを全角1バイト目に移動する
         MOVE(pos_y-1, x-1);
       } else {
         MOVE(pos_y-1, x);      
       }
    }
  } else if (pos_y == 0){
    edit_scrollDown();
  }
}

// カーソルを行末に移動
void tscreenBase::moveLineEnd() {
  int16_t x = width-1;
  for(;;) {
    if (IS_PRINT(VPEEK(x, pos_y)) ) 
       break;  
    if (x > 0)
      x--;
    else
      break;
  }
  if (x>1 && (isJMS(&VPEEK(0,pos_y),x) == 2) ) {
    // シフトJIS２バイト目の場合、カーソルを１バイト目に移動する 
    x--;
  }
  MOVE(pos_y, x);     
}

// スクリーン表示の最終表示の行先頭に移動
void tscreenBase::moveBottom() {
  int16_t y = height-1;
  while(y) {
    if (IS_PRINT(VPEEK(0, y)) ) 
       break;
    y--;
  }
  MOVE(y,0);
}

// カーソルを指定位置に移動
void tscreenBase::locate(uint16_t x, uint16_t y) {
  if ( x >= width )  x = width-1;
  if ( y >= height)  y = height;
  MOVE(y, x);
}

// カーソル位置の文字コード取得
uint16_t tscreenBase::vpeek(uint16_t x, uint16_t y) {
  if (x >= width || y >= height) 
     return 0;
  return VPEEK(x,y);
}

// 行データの入力確定
uint8_t tscreenBase::enter_text() {

  // 現在のカーソル位置の行先頭アドレス取得
  uint8_t *ptr = &VPEEK(0, pos_y); 
  if (pos_x == 0 && pos_y)
    ptr--;

  // ポインタをさかのぼって、前行からの文字列の連続性を調べる
  // その文字列先頭アドレスをtopにセットする
  uint8_t *top = ptr;
  while (top > screen && *top != 0 )
    top--;
  if ( top != screen ) top++;
 text = top;
  return true;
}

// 指定行の行番号の取得
int16_t tscreenBase::getLineNum(int16_t l) {
  uint8_t* ptr = screen+width*l;
  uint32_t n = 0;
  int rc = -1;  
  while (c_isdigit(*ptr)) {
    n *= 10;
    n+= *ptr-'0';
    if (n>32767) {
      n = 0;
      break;
    }
    ptr++;
  }
  if (!n)
    rc = -1;
  else {
    if (*ptr==32 && *(ptr+1) > 0)
      rc = n;
    else 
      rc = -1;
  }
  return rc;
}

// 編集中画面をスクロールアップする
uint8_t tscreenBase::edit_scrollUp() {
  static uint16_t prvlineNum = 0; // 直前の処理行の行数
#if DEPEND_TTBASIC == 0
   scroll_up();
#else
  // 1行分スクロールアップを試みる
  int16_t lineno,nm,len;
  char* text;
  lineno = getLineNum(height-1); // 最終行の表示行番号の取得
  if (lineno <= 0) {
    lineno = prvlineNum;
   }
  if (lineno > 0) {
    // 取得出来た場合、次の行番号を取得する
    nm = getNextLineNo(lineno); 
    if (nm > 0) {
      // 次の行が存在する
      text = getLineStr(nm);
      len = strlen(text);
      for (uint8_t i=0; i < len/width+1; i++) {
        scroll_up();
      }
      strcpy((char*)&VPEEK(0,height-1-(len/width)),text);
      for (uint8_t i=0; i < len/width+1; i++)
         refresh_line(height-1-i);
      prvlineNum = nm; // 今回の処理した行を保持
    } else {
      prvlineNum = 0; // 保持していた処理行をクリア
      scroll_up();      
    }
  } else {
    scroll_up();    
  }
  MOVE(pos_y, pos_x);
#endif
  return 0;
}

// 編集中画面をスクロールダウンする
uint8_t tscreenBase::edit_scrollDown() {
#if DEPEND_TTBASIC == 0
  scroll_down();
#else
  // 1行分スクロールダウンを試みる
  int16_t lineno,prv_nm,len;
  char* text;
  lineno = getLineNum(0); // 最終行の表示行番号の取得
  if (lineno > 0) {
    prv_nm = getPrevLineNo(lineno);
    if (prv_nm > 0) {
      text = getLineStr(prv_nm);
      len = strlen(text);
      for (uint8_t i=0; i < len/width+1; i++) {
        scroll_down();
      }
      strcpy((char*)&VPEEK(0,0),text);
      for (uint8_t i=0; i < len/width+1; i++)
         refresh_line(0+i);
    } else {
      scroll_down();      
    }
  } else {
    scroll_down();
  }
 MOVE(pos_y, pos_x);
#endif
  return 0;
}

// 文字の取得（シフトJIS対応)
uint16_t tscreenBase::get_wch() {
  uint8_t ch1,ch2;           // 入力文字
  uint16_t wch;              // 2バイト文字コード
  ch1 = get_ch();
  if (isShiftJIS(ch1)) {
     ch2 = get_ch();
     wch = ch1<<8 | ch2;
  } else {
    wch = ch1;
  }
  return wch;
}

// スクリーン編集
uint8_t tscreenBase::edit() {
  uint16_t ch;  // 入力文字  
  for(;;) {
    ch = get_wch();   
    show_curs(false);
    switch(ch) {
      case SC_KEY_CR:         // [Enter]キー
        show_curs(true);
        return enter_text();
        break;

      //case SC_KEY_CTRL_L:  // [CTRL+L] 画面クリア
      case SC_KEY_F1:        // F1
        cls();
        //locate(0,0);
        break;
 
      case SC_KEY_HOME:      // [HOMEキー] 行先頭移動
        locate(0, pos_y);
        break;
        
      case SC_KEY_NPAGE:      // [PageDown] 表示プログラム最終行に移動
        if (pos_x == 0 && pos_y == height-1) {
          edit_scrollUp();
        } else {
          moveBottom();
        }
        break;
      
      case SC_KEY_PPAGE:     // [PageUP] 画面(0,0)に移動
        if (pos_x == 0 && pos_y == 0) {
          edit_scrollDown();
        } else {
          locate(0, 0);
        }  
        break;
        
      //case SC_KEY_CTRL_R:   // [CTRL_R] 画面更新
      case SC_KEY_F5:         // F5
        //beep();
        refresh();  break;

      case SC_KEY_END:        // [ENDキー] 行の右端移動
         moveLineEnd();
         break;

      case SC_KEY_IC:         // [Insert]キー
        flgIns = !flgIns;
        break;        

      case SC_KEY_BACKSPACE:  // [BS]キー
        movePosPrevChar();
        delete_char();
        break;        

      case SC_KEY_DC:         // [Del]キー
      case SC_KEY_CTRL_X:
        delete_char();
        break;        
      
      case SC_KEY_RIGHT:      // [→]キー
        movePosNextChar();
        break;

      case SC_KEY_LEFT:       // [←]キー
        movePosPrevChar();
        break;

      case SC_KEY_DOWN:       // [↓]キー
        movePosNextLineChar();
        break;
      
      case SC_KEY_UP:         // [↑]キー
        movePosPrevLineChar();
        break;

      //case SC_KEY_CTRL_N:  // 行挿入
      case SC_KEY_F3:        // F3
        Insert_newLine(pos_y);       
        break;

      //case SC_KEY_CTRL_D:  // 行削除
      case SC_KEY_F2:        // F2
        clerLine(pos_y);
        break;

      case SC_KEY_F7:        // F7 行の分割
        splitLine();
        break;

      case SC_KEY_F8:        // F8 行の結合
        margeLine();
        break;
      
      default:               // その他
      
      if (IS_PRINT(ch)) {
        Insert_char(ch);
      }  
      break;
    }
    show_curs(true);
  };
   show_curs(true);
}

#if USE_EDITLINE == 1

// ライン編集
// 中断の場合、0を返す
uint8_t tscreenBase::editLine() {
  uint16_t basePos_x = pos_x;
  uint16_t basePos_y = pos_y;
  uint16_t ch;  // 入力文字  
  
  show_curs(true);
  for(;;) {
    ch = get_wch();   
    switch(ch) {
      case SC_KEY_CR:         // [Enter]キー
        show_curs(false);
        text = &VPEEK(basePos_x, basePos_y);
        return 1;
        break;
 
      case SC_KEY_HOME:      // [HOMEキー] 行先頭移動
        locate(basePos_x, basePos_y);
        break;
        
      //case SC_KEY_CTRL_R:   // [CTRL_R] 画面更新
      case SC_KEY_F5:         // F5
        //beep();
        refresh();  break;

      case SC_KEY_END:        // [ENDキー] 行の右端移動
         moveLineEnd();
         break;

      case SC_KEY_IC:         // [Insert]キー
        flgIns = !flgIns;
        break;        

      case SC_KEY_BACKSPACE:  // [BS]キー
        movePosPrevChar();
        delete_char();
        break;        

      case SC_KEY_DC:         // [Del]キー
      case SC_KEY_CTRL_X:
        delete_char();
        break;        
    
      case SC_KEY_CTRL_C:   // [CTRL_C] 中断
      case SC_KEY_ESCAPE:
        return 0;

      default:               // その他
        if (ch >= 0x20) {
          Insert_char(ch);
        }  
        break;
    }
  };
}
#endif

