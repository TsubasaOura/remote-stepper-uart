// UART
#include <SoftwareSerial.h>
#define myRX 6
#define myTX 7
SoftwareSerial mySerial(myRX, myTX);

//MOTOR
#include <A4988.h>
const int MOTOR_STEPS = 200;
const int DIR   = 8;
const int STEP  = 9;
A4988 stepper(MOTOR_STEPS, DIR, STEP);
double  rpm = 60;
int microsteps  = 16;

//ENC500
byte  ENC500_A  = 2;
byte  ENC500_B  = 3;
byte  ENC500_Z  = 4;
int enc500_cnt  = 0;
int enc500_rot_cnt = 0;

//ENC12
int enc12_cnt = 0;

// Setup
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1);
  mySerial.begin(9600);
  mySerial.setTimeout(1);

  stepper.begin(rpm, microsteps);

  pinMode(ENC500_A, INPUT_PULLUP);
  pinMode(ENC500_B, INPUT_PULLUP);
  pinMode(ENC500_Z, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENC500_A), enc500_pluse, RISING);
}

// メイン
void loop() {
  //スレーブENCのカウントをマスタに送信
  serial_write(enc500_cnt);

  //マスタENCの値でモータ制御
  enc500_cnt = serial_read();
  stepper.move(enc500_cnt);

  // シリアルモニタに表示
  Serial.println("ENC500: "+String(enc500_cnt)+", ENC12: "+String(enc12_cnt));
}

//ENC500割り込み
void enc500_pluse() {
  if (digitalRead(ENC500_B) == 0) {
    enc500_cnt++;
    if (digitalRead(ENC500_Z) == 0) {
      enc500_rot_cnt++;
    }
  }
  else {
    enc500_cnt--;
    if (digitalRead(ENC500_Z) == 0) {
      enc500_rot_cnt--;
    }
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