// UART
#include <SoftwareSerial.h>
#define myRX 6
#define myTX 7
SoftwareSerial mySerial(myRX, myTX);

// LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd( 8, 9, 10, 11, 12, 13 );

// ENC12
#include <Rotary.h>
#define ENC12_A 2
#define ENC12_B 3
Rotary r(ENC12_A, ENC12_B);
int enc12_cnt = 0;

// VOL
const int VOL_PIN = A0;
int vol = 0;
int range = 0;

// ENC500
int enc500_cnt = 0;

// Setup
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1);
  mySerial.begin(9600);
  mySerial.setTimeout(1);

  r.begin(true);
  lcd.begin(16, 2);
  lcd.clear();
  range_print(range);
  range_print(enc500_cnt);

  attachInterrupt(digitalPinToInterrupt(ENC12_A), enc12_pluse, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC12_B), enc12_pluse, RISING);
}

// メイン
void loop() {
  //可変抵抗からレンジを算出してLCDに表示
  vol = analogRead(VOL_PIN);
  range = to_range(vol);
  range_print(range);

  //コントローラーENCのカウントをスレーブに送信
  serial_write(enc12_cnt);

  //スレーブENCの値をLCDに表示
  enc500_cnt = serial_read();
  rev_print(enc500_cnt);

  // シリアルモニタに表示
  Serial.println("VOL: "+String(vol)+", ENC12: "+String(enc12_cnt)+", ENC500: "+String(enc500_cnt));
}

//レンジをLCDに表示
void range_print(int range) {
  char msg[17];
  char s[17];
  sprintf(msg, "Range: %s [-]", dtostrf(range, 5, 0, s));
  lcd.setCursor(0, 0);
  lcd.print(msg);
}

//回転をLCDに表示
void rev_print(int enc500_cnt) {
  char msg[17];
  char s[17];
  sprintf(msg, "Rotate: %s[r]", dtostrf(enc500_cnt, 5, 0, s));
  lcd.setCursor(0, 1);
  lcd.print(msg);
}

//ENC12割り込み
void enc12_pluse() {
  if (r.process() == DIR_NONE) {}// do nothing
  else if (r.process() == DIR_CW) {
    enc12_cnt += range;
  }
  else if (r.process() == DIR_CCW) {
    enc12_cnt -= range;
  }
}

//可変抵抗のアナログ値をレンジに変換
int to_range(double vol) {
  double div = 5 * (1024 - vol ) / 1024;
  if (div < 1) {
    return 0;
  }
  else if (div < 2) {
    return 1;
  }
  else if (div < 3) {
    return 5;
  }
  else if (div < 4) {
    return 10;
  }
  else if (div >= 4) {
    return 100;
  }
}

//シリアルデータ読み込み
int serial_read(void){
  //受信データがある場合if内を処理
  if (Serial.available() > 0) {
    int val = Serial.parseInt();     //文字列データを数値に変換
    while (Serial.available() > 0) { //受信バッファクリア
      char t = Serial.read();
    }
    return val;
  }
  mySerial.listen();
  if (mySerial.available() > 0) {
    int val = mySerial.parseInt();
    while (mySerial.available() > 0) {
      char t = mySerial.read();
    }
    return val;
  }
}

//シリアルデータ書き込み
void serial_write(int cnt_data){
  mySerial.listen();
  mySerial.println(cnt_data);
}
