#include <Arduino.h>

#include "LittleFS.h"

#include "wifi.h"
#include "web.h"

#include <ArduinoJson.h>  //  ArduinoJson库

extern String localIp;


const uint16_t port = 4321;             //服务端的TCP端口
const char *host = "124.222.68.249";       //这里填云服务器的地址
WiFiClient client;                      //创建一个tcp client连接
int DirtyFlag = 1;                      //
String StateFb = "";                  //初始化反馈的字符串
void client_loop();
void client_setup();
String esp_info(); // 获取esp8266的各种信息

void fs_setup();

void setup()
{
  Serial.begin(9600);

  wifi_setup();

  // webServer_setup();

  fs_setup();

  // pinMode(D7, OUTPUT);
  // digitalWrite(D7, HIGH);

  // pinMode(D6, OUTPUT);
  // digitalWrite(D6, LOW);

  // pinMode(LED_BUILTIN, OUTPUT);
  // digitalWrite(LED_BUILTIN, HIGH);

  client_setup();


}

void loop()
{
  // webServer_loop();
  client_loop();
}

String esp_info() {
  StaticJsonDocument<256> doc;

  doc["info"]["ip"] = localIp;

  JsonObject digital_pin = doc.createNestedObject("digital_pin");
  digital_pin["D0"] = digitalRead(D0);
  digital_pin["D1"] = digitalRead(D1);
  digital_pin["D2"] = digitalRead(D2);
  digital_pin["D3"] = digitalRead(D3);
  digital_pin["D4"] = digitalRead(D4);
  digital_pin["D5"] = digitalRead(D5);
  digital_pin["D6"] = digitalRead(D6);
  digital_pin["D7"] = digitalRead(D7);
  digital_pin["D8"] = digitalRead(D8);
  doc["analog_pin"]["A0"] = analogRead(A0);

  String jsonCode;
  serializeJson(doc, jsonCode);
  Serial.print("Root Json Code: ");
  Serial.println(jsonCode);
  return jsonCode;
}

void client_setup() {
  StateFb = esp_info();
}

void deal_touch(String pin, int begin_value, int touch_value = 1000) {
  Serial.println("pin: " + pin + " begin_value: " + begin_value + " touch_value: " + touch_value);
  if(pin == "D0") {
    pinMode(D0, OUTPUT);
    digitalWrite(D0, begin_value);
    delay(touch_value);
    digitalWrite(D0, !begin_value);
  } else if(pin == "D1") {
    pinMode(D1, OUTPUT);
    digitalWrite(D1, begin_value);
    delay(touch_value);
    digitalWrite(D1, !begin_value);
  } else if(pin == "D2") {
    pinMode(D2, OUTPUT);
    digitalWrite(D2, begin_value);
    delay(touch_value);
    digitalWrite(D2, !begin_value);
  } else if(pin == "D3") {
    pinMode(D3, OUTPUT);
    digitalWrite(D3, begin_value);
    delay(touch_value);
    digitalWrite(D3, !begin_value);
  } else if(pin == "D4") {
    pinMode(D4, OUTPUT);
    digitalWrite(D4, begin_value);
    delay(touch_value);
    digitalWrite(D4, !begin_value);
  } else if(pin == "D5") {
    pinMode(D5, OUTPUT);
    digitalWrite(D5, begin_value);
    delay(touch_value);
    digitalWrite(D5, !begin_value);
  } else if(pin == "D6") {
    pinMode(D6, OUTPUT);
    digitalWrite(D6, begin_value);
    delay(touch_value);
    digitalWrite(D6, !begin_value);
  } else if(pin == "D7") {
    pinMode(D7, OUTPUT);
    digitalWrite(D7, begin_value);
    delay(touch_value);
    digitalWrite(D7, !begin_value);
  } else if(pin == "D8") {
    pinMode(D8, OUTPUT);
    digitalWrite(D8, begin_value);
    delay(touch_value);
    digitalWrite(D8, !begin_value);
  } else if(pin == "A0") {
    pinMode(A0, OUTPUT);
    analogWrite(A0, begin_value);
    delay(touch_value);
    analogWrite(A0, !begin_value);
  }
}

void deal_pin(String pin, int value) {
  Serial.println("pin: " + pin + " value: " + value);

  if(pin == "D0") {
    pinMode(D0, OUTPUT);
    digitalWrite(D0, value);
  } else if(pin == "D1") {
    pinMode(D1, OUTPUT);
    digitalWrite(D1, value);
  } else if(pin == "D2") {
    pinMode(D2, OUTPUT);
    digitalWrite(D2, value);
  } else if(pin == "D3") {
    pinMode(D3, OUTPUT);
    digitalWrite(D3, value);
  } else if(pin == "D4") {
    pinMode(D4, OUTPUT);
    digitalWrite(D4, value);
  } else if(pin == "D5") {
    pinMode(D5, OUTPUT);
    digitalWrite(D5, value);
  } else if(pin == "D6") {
    pinMode(D6, OUTPUT);
    digitalWrite(D6, value);
  } else if(pin == "D7") {
    pinMode(D7, OUTPUT);
    digitalWrite(D7, value);
  } else if(pin == "D8") {
    pinMode(D8, OUTPUT);
    digitalWrite(D8, value);
  } else if(pin == "A0") {
    pinMode(A0, OUTPUT);
    analogWrite(A0, value);
  }
  
}

void client_loop() {
  if (!client.connect(host, port))      //连接上TCP Server
  {
    Serial.println("connection failed");
    Serial.println("wait 1 sec...");
    delay(1000);
    return;
  } else {
    Serial.println("connection success");
  }
 
  String readBuff = "";                 //初始化接受的字符串     
  long Cycle = 0;                       //初始化Cycle
  while (client.connected())
  {
    if(DirtyFlag)                       //连接上只发首次
    {
      client.print("ioT:ESP8266-computer " + StateFb);    //发送标识ESP8266 + LED状态
      // Serial.println("Connected ");
      DirtyFlag = 0;
    }
    
    if (client.available())             //如果有可读数据
    {
      Serial.println("已收到: " + readBuff);
      char c = client.read();           //读取一个字节//也可以用readLine()等其他方法
      if (c == '#')                     //接收到空格认为TCP接受结束
      {
        Serial.println("已收到: " + readBuff); //从串口打印
        break;                                 //跳出循环
      }
      readBuff += c;
      Cycle = 0;
    }
    else
    {
      Cycle = Cycle + 1;    //防止长时间没有数据造成异常 半个小时重连一次
      delay(10); // 10ms

      if(Cycle >= 180000) // 180000 * 10ms = 30min
        break;
    }
  }
  // Serial.println(readBuff);
  // pin_D0_1
  if(readBuff.indexOf("pin_") != -1) {
    
    int index = readBuff.lastIndexOf("_");
    String pin = readBuff.substring(4, index);
    String value = readBuff.substring(index + 1);
    deal_pin(pin, value.toInt());
    StateFb = esp_info();
  } else if(readBuff.indexOf("touch_") != -1) { // touch_D0_1_1000
    // 以下划线拆分touch_D0_1_1000为数组
    int index = readBuff.lastIndexOf("_");
    String pin_and_value = readBuff.substring(6, index);
    String touch_value = readBuff.substring(index + 1);
    index = pin_and_value.lastIndexOf("_");
    String pin = pin_and_value.substring(0, index);
    String begin_value = pin_and_value.substring(index + 1);
    deal_touch(pin, begin_value.toInt(), touch_value.toInt());
    StateFb = esp_info();

  } else if(readBuff == "on")
  {
    digitalWrite(LED_BUILTIN, LOW);  //操作PIN脚开启关闭LED
    StateFb = esp_info();
  } else if(readBuff == "off")
  {
    digitalWrite(LED_BUILTIN, HIGH);  
    StateFb = esp_info();
  } else if(readBuff == "state") {
    StateFb = esp_info();
  }

  client.print("ioT:ESP8266-computer " + esp_info());   //向客户端发送
  client.stop();    //停止TCP服务
  DirtyFlag = 1;

}

void fs_setup() {
   if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  
  // File file = LittleFS.open("/index.html", "r");
  // if(!file){
  //   Serial.println("Failed to open file for reading");
  //   return;
  // }
  
  // Serial.println("File Content:");
  // while(file.available()){
  //   Serial.write(file.read());
  // }
  // file.close();

}

