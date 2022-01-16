#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7);
///////////////////////////////////////////////////////
//setup
///////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);
  mySerial.begin(115200);
  mySerial.setTimeout(10);
}

///////////////////////////////////////////////////////
//loop
///////////////////////////////////////////////////////////
void loop() {
  if (Serial.available() > 0) {
    int val = Serial.parseInt();    //文字列データを数値に変換
    while (Serial.available() > 0) {//受信バッファクリア
      char t = Serial.read();
    }
    Serial.println("start: "+String(val));
    serial_write(val+3);
  }
  mySerial.listen();
  if (mySerial.available() > 0) {
    int val = mySerial.parseInt();    //文字列データを数値に変換
    while (mySerial.available() > 0) {//受信バッファクリア
      char t = mySerial.read();
    }
    Serial.println("goal: "+String(val+5));
  }
}

//シリアルデータ書き込み
void serial_write(int cnt_data){
  mySerial.listen();
  mySerial.print(cnt_data);
  Serial.println("wip: "+String(cnt_data));
}
