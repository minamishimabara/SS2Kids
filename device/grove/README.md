# 南島原タイニーBASIC（GROVEバージョン）
## 南島原発小中学生向けのIoT言語
- ESPr Oneで動作

## ハードウエア
### ESPr One
- WiFi付き、マイコンボード
<img src="https://docid81hrs3j1.cloudfront.net/contents/large/2620_1.jpg">
- 価格2,980円
<https://www.switch-science.com/catalog/2620/>

### GROVE - ベースシールド
- センサ類を共通のコネクタで繋げられる、はんだ付けや配線などは不要
<img src="https://statics3.seeedstudio.com/seeed/img/2016-09/oJYc6A5jTrgslQWC5j7gw9tI.jpg">
- 価格1,242円
<https://www.switch-science.com/catalog/1293/>

## 南島原タイニーBASICの文法
- 値の範囲 16ビット符号付き整数(-32767～32767)、数値のみ！
- 代入は「=」でなく「:」1を使う
 - 例）「I=I+1」でなく「I:I+1」
- 文字列 “または’で括った文字列
- 変数 A～Z
- 配列@(0)～@(31)

### 命令
- LIST [《行番号》]
- RUN
- NEW
- SAVE [BOOT]
 - EEPROMにプログラムを保存する。BOOT指定時は、起動時に保存したプログラムを自動実行する
- LOAD
 - EEPROMに保存したプログラムを読み込む


### 文
- PRINT [《文字列》|#《桁数》| 《式》 ][,《文字列》|#《桁数》|《式》 …]
- INPUT [《文字列》][《変数》|《配列》][,[《文字列》][《変数》|《配列》]…]
- [LET]《変数》|《配列》=《式》
- REM [《コメント》]
- IF 《式》 [《文》]
- GOTO 《式》
- GOSUB 《式》、RETURN
- FOR 《変数》=《開始値》 TO 《終了値》[STEP 《増分》]、NEXT 《変数》
- STOP
- SLEEP 《式》
 - 指定時間（ミリ秒）停止
- LCD [《文字列》|#《桁数》| 《式》 ][,《文字列》|#《桁数》|《式》 …]
- LED 《式》 [ON|OFF]
 - 指定された番号のLEDを制御する。ボード上のLEDの番号は14
- MQTT 《文字列》,《文字列》,《式》[,《文字列》,《式》]
- HTTP 《文字列》,《文字列》,《式》[,《文字列》,《式》]
 - 指定されたURLにPOSTする、データ形式は階層のないJSON
- CAMERA 《文字列》
 - 指定されたURLに写真をPOSTする、データ形式はJSON+BASE64


### 関数
- SIZE()
- ABS(《式》)
- RND(《式》)
- SW()
- ILLUMI()
 - 照度を返す
- ILLUMIIR()
 - 赤外線照度を返す
- UVINDEX()
 - 紫外線インデックス（パーセンテージ）を返す
- ANALOG()
 - A0ポートのアナログ値を返す
- HUMIDITY()
 - 湿度（摂氏）を返す
- TEMPERATURE()
 - 温度（パーセンテージ）を返す


## その他の情報
タイニーBASICは、場所の名前をつけるのが通例だそうです。  
なので、南島原ハッカソンで誕生したので、
南島原タイニーBASICと命名

### TODO
- WiFiのSSIDとパスワードがハードコーディングになってる
- SW()の仕様をSW(《式》)にする
- MQTTがIBM固定、HOST名、クライアントIDを動的に設定可能にする
- さくらのIoTも対応したい
- 通信エラー後の復帰が不安
- I2Cに対応したい
 - 定義ファイル？で汎用的に作りたい
 - SW押しながらの電源ONで設定モードになり、SETENV文が有効になる


元となった「豊四季タイニーBASIC」
<https://vintagechips.wordpress.com/2015/12/06/%E8%B1%8A%E5%9B%9B%E5%AD%A3%E3%82%BF%E3%82%A4%E3%83%8B%E3%83%BCbasic%E7%A2%BA%E5%AE%9A%E7%89%88/>

ESP8266で動作する「豊四季タイニーBASIC」
<https://github.com/robo8080/ttbasic_ESP8266>
