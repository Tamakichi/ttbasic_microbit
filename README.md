# 豊四季Tiny Basic for micro:bit V0.03

「豊四季Tiny Basic for micro:bit」は、オリジナル版「TOYOSHIKI Tiny BASIC for Arduino」を  
Arduino micro:bit向けに移植・機能拡張したプログラムです。  

オリジナル版配布サイト   https://github.com/vintagechips/ttbasic_arduino  
関連情報 電脳伝説 Vintagechips - 豊四季タイニーBASIC確定版  

**「豊四季タイニーBASIC」の著作権は開発者のTetsuya Suzuki氏**にあります.  
プログラム利用については、オリジナル版の著作権者の配布条件に従うものとします  .
著作権者の同意なしに経済的な利益を得てはいけません.  
この条件のもとで、利用、複写、改編、再配布を認めます.  
バイナリファイルのみの配布は行わないで下さい.  

**修正内容**
- ラインエディタ部の差し換え  
  オリジナルのラインエディタ部分をフルスリーンテキストエディタに差し換えました.  
  ターミナル上で昔のBASICっぽい編集操作を出来るようにしました.  
- コマンドの追加  
  - **RENUM** ：行番号再割り当て  

  - **CLS** ：画面クリア  

  - **LOCATE**：カーソル移動  

  - **COLOR**： 文字色の指定  

  - **ATTR**：文字装飾の指定  

  - **WAIT** n：時間待ち(マイクロ秒)  

  - **VPEEK**：スクリーン位置の文字コード参照  

  - **CHR$()**、**HEX$()**、**BIN$()**、**ASC()**、**INKEY()** 関数の追加  

  - **TICK()** 関数追加  

  - **?**：**PRINT**文の省略系を追加  

  - **AND**、**OR**、!、~、!の追加  

  - **GPIO**、**OUT**、**IN()**、**ANA()**：GPIO利用コマンドの追加  

  - **FILES**、**SAVE**、**LOAD**： 内部フラッシュメモリへのプログラム保存機能の追加  

  - LEDマトリックスの描画コマンド追加

    - **PSET** x,y,c

    - **LINE** x1,y1,x2,y2,c

    - **RECT** x1,y1,w,h,c,mode

    - **CIRCLE** x,y,r,c,mode

      ​
- 文法の変更  
  - 定数の16進数表記対応(例 $1234,$AB)  
  - 命令文区切りを';'から':'に変更  
  - **PRINT**文の行継続を';'でも可能に変更  
  - **PRINT**文の数値桁指定#nのnにマイナス値指定時、空白ではなく0埋め出来るように追加  
  - **IF**文の不一致判定を"!="でも可能に変更、ELSE文対応  
  - 演算子 剰余計算'%'、シフト演算子 '>>'、'<<'を追加  
  - 論理積・論理和 '|'、'&'ビット演算子を追加  
  - **STOP**命令を**END**に変更  
  - **SIZE**を**FREE**に変更  
  - 文字列囲みにシングルクォーテーションを利用出来ないように変更  
  - コメントをシングルクォーテーション(')での記述出来るように修正  
  - 配列の連続値設定を可能した(例: @(0)=1,2,3,4,5)  
  - GOTO,GOSUBをラベル対応(例: GOTO "ラベル" ダブルクォーテーション囲み)  
  - INPUT文の入力改善、オーバーフローエラー回避対策追加  

- その他  
  - プログラム領域を256バイトから4096バイトに拡大  
  - 配列サイズを32から100に拡大  
  - 変数はA～Zの他に、A0..A6 ～ Z0..Z6の数字を付けた2桁も利用可能  

本プログラムは下記のライブラリを利用しています。  
- mcursesライブラリ　<https://github.com/ChrisMicro/mcurses>  
- Adafruit_microbit_Matrix (Adafruit_Microbit libraryを流用し改造) 
  https://github.com/adafruit/Adafruit_Microbit   

別途、Adafruit GFX libraryのインストールが必要です。  



## 利用方法

1. ダウンロード  
   https://github.com/Tamakichi/ttbasic_microbit/archive/master.zip  

2. インストトール  
   micro:bitを接続します。  
   micro:bitのドライブ MICROBITを開いてファイルを書き込める状態にします。  
   ダウンロードファイル **ttbasic_microbit-master.zip**を解凍します。  
   フォルダbin内の**ttbasic_microbit.ino.hex**をmicro:bit用のドライブにドラック＆ドロップします。
   プログラム書き込みが終了したら、ターミナルソフト（TeraTerm）等でシリアル接続します。  


## フルスリーンテキストエディタの機能

**スクリーンサイス**
80列ｘ24行

**ターミナル通信条件**
通信速度：115200bbs、パリティ：none、ストップビット：1、フロー制御：なし  
TeraTermを利用して、プログラムをコピー＆ペーストして取り込み場合は、  
送信遅延を1ミリ秒/字に設定して下さい。  

**利用可能キー**
- カーソル移動 カーソルキー  
- [Delete]、[CTRL-X] ：カーソル位置の文字削除  
- [BackSpace]：カーソル前の文字削除と前に移動
- [PageUP]、[PageDown]、[CTRL-R]：画面の再表示  
- [HOME]、[END]：行内でカーソルを左端、右端に移動  
- [INS]：挿入・上書きのトグル切り替え  
- [Enter]：行入力確定  
- [ESC]2回押し、[CTRL-C]：実行プログラムの中断  
- [CTRL-L]、[F1]：画面のクリア  
- [CTRL-R]、[F5]：画面再表示  


## ピン割り付け・ピン指定
- ピン： **PN0 ～ PN32**  で指定、または **0 ～ 32**で指定する  
- ピンモード指定コマンド ：**GPIO** *ピン*,  OUTPUT|INPUT_PU|INPUT_PD|INPUT_FL"   
- デジタル出力：**OUT** ピン, HIGH|LOW|0|1  
- デジタル入力：変数=**IN**(ピン)  
- アナログ入力：変数=**ANA**(ピン)  


## サンプルプログラム

### 画面に色付き文字表示

```
10 FOR I=0 TO 10
20 FOR J=0 TO 10
30 COLOR RND(8): ? "*";
35 WAIT 100
40 NEXT J
50 ?
60 NEXT I
```

### ボード上のLEDの点滅

```
1 'Lﾁｶ
10 GPIO 3,OUTPUT
20 OUT 3,LOW
30 GPIO 26,OUTPUT
35 "@loop"
40 OUT 26,HIGH
50 WAIT 300
60 OUT 26,LOW
70 WAIT 300
80 GOTO "@loop"
```

### アナログ値表示

```
10 CLS
20 A=ANA(PN0)
30 LOCATE 5,5:?A;"    "
40 GOTO 20
```

### ボタン入力判定

```
10 CLS
20 IF !IN(BTNA) ?"Button A"
30 IF !IN(BTNB) ?"Button B"
40 WAIT 200
50 GOTO 20
```

### LEDマトリックス ドット表示

```
10 CLS 1
20 D=1
30 FOR Y=0 TO 4
40 FOR X=0 TO 4
50 PSET X,Y,D
60 WAIT 100
70 NEXT X
80 NEXT Y
90 IF D D=0 ELSE D=1
100 GOTO 30
```



## 以降はオリジナルのドキュメントです

TOYOSHIKI Tiny BASIC for Arduino

The code tested in Arduino Uno R3.
Use UART terminal, or temporarily use Arduino IDE serial monitor.

Operation example

\> list
10 FOR I=2 TO -2 STEP -1; GOSUB 100; NEXT I
20 STOP
100 REM Subroutine
110 PRINT ABS(I); RETURN

OK
\>run
2
1
0
1
2

OK
\>

The grammar is the same as
PALO ALTO TinyBASIC by Li-Chen Wang
Except 3 point to show below.

(1)The contracted form of the description is invalid.

(2)Force abort key
PALO ALTO TinyBASIC -> [Ctrl]+[C]
TOYOSHIKI TinyBASIC -> [ESC]
NOTE: Probably, there is no input means in serial monitor.

(3)Other some beyond my expectations.

(C)2012 Tetsuya Suzuki
GNU General Public License