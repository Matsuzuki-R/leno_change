#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "yourssid";                 //SSID
const char *password = "yourpassword";         //Wi-Fi-password

const String token = "Your LINE's token";         //LINEのやつ

int mode;     //親探知-インターフォンモードの切り替え用変数

int kaonashi_count;     //無限カオナシ防止用変数

void setup() {
  // シリアル通信を開始
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  mode = 1;     //適宜変更、1:親探知  2:インターフォン
}

// ループ
void loop() {

  switch(mode){  

    case 1:               //親探知モード

      // シリアルポートに到着しているデータのバイト数
      if (Serial.available() > 0 ) {
        kaonashi_count = 0;
        // シリアルデータの受信 (改行まで)
        String data = Serial.readStringUntil('\n');
        if (data.indexOf("name") != -1) {
          if (data.indexOf("face1") != -1) {
            Serial.println("いた");
            sendMessage("奴が来た");
            delay(5000);
          } else {
            Serial.println("いない");
          }
        }
      } else {

        if(kaonashi_count == 1000){
          Serial.println("カオナシ");
          kaonashi_count = 0;
        }else{
          kaonashi_count ++;
          delay(1);
        }
      }

      break;

    case 2:               //インターフォンモード

      // シリアルポートに到着しているデータのバイト数
      if (Serial.available() > 0 ) {
        kaonashi_count = 0;
        // シリアルデータの受信 (改行まで)
        String data = Serial.readStringUntil('\n');
        if (data.indexOf("name") != -1) {
          if (data.indexOf("face2") != -1) {
            Serial.println("いた2");
            sendMessage("2番が来た");
            delay(5000);
          } else if (data.indexOf("face3") != -1) {
            Serial.println("いた3");
            sendMessage("3番が来た");
            delay(5000);
          }else if (data.indexOf("face4") != -1) {
            Serial.println("いた4");
            sendMessage("4番が来た");
            delay(5000);
          }else if (data.indexOf("face5") != -1) {
            Serial.println("いた5");
            sendMessage("5番が来た");
            delay(5000);
          }else{
            Serial.println("知らない人が来た");
          }
        }
      } else {

        if(kaonashi_count == 1000){
          Serial.println("カオナシ");
          kaonashi_count = 0;
        }else{
          kaonashi_count ++;
          delay(1);
        }
      }

      break;

     default:
      Serial.println("エラー");
     break;

  }

}



void sendMessage(String message) { 
  HTTPClient client;

  String url = "https://notify-api.line.me/api/notify";
  client.begin(url);
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  client.addHeader("Authorization", "Bearer " + token);

  String query = "message=" + message;
  client.POST(query);

  String body = client.getString();
  Serial.println("Sent the message");
  Serial.println(body);
  client.end();
}
