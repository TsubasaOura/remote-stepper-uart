#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7);
///////////////////////////////////////////////////////
//setup
///////////////////////////////////////////////////////////
void setup() {
  mySerial.begin(115200);
  mySerial.setTimeout(10);
}

///////////////////////////////////////////////////////
//loop
///////////////////////////////////////////////////////////
void loop() {
  if (mySerial.available() > 0) {
    int val = mySerial.parseInt();    //文字列データを数値に変換
    while (mySerial.available() > 0) {//受信バッファクリア
      char t = mySerial.read();
    }
    mySerial.println(val+2);
  }
}
