
#include "web.h"

#include <ArduinoJson.h>  //  ArduinoJson库

extern String localIp;

ESP8266WebServer esp8266_server(
    80);  // 建立ESP8266WebServer对象，对象名称为esp8266_server

void handleJson();

void webServer_setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // 设置内置LED引脚为输出模式以便控制LED
  esp8266_server
      .begin();  //  详细讲解请参见太极创客网站《零基础入门学用物联网》
  // esp8266_server.on("/", handleRoot);       //  第3章-第2节
  // ESP8266-NodeMCU网络服务器-1 esp8266_server.on("/home", handleHome);
  esp8266_server.on("/pin", handlePin);  //
  // 设置处理LED控制请求的函数'handleLED'
  esp8266_server.on("/json", handleJson);
  esp8266_server.on("/openComputer", handleOpenComputer);
  esp8266_server.on("/openComputer2", handleOpenComputer2);
  esp8266_server.onNotFound(handleUserRequet);
}

void webServer_loop() {
  esp8266_server.handleClient();  // 检查http服务器访问
}
void handleHome() {}


void handleOpenComputer() {
  digitalWrite(D6, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(D6, LOW);
  digitalWrite(LED_BUILTIN, HIGH);

  StaticJsonDocument<32> doc;

  doc["status"] = "D6:" + String(digitalRead(D6));
  String output;
  serializeJson(doc, output);

  esp8266_server.send(200, "application/json", output);

}

void handleOpenComputer2() {
  digitalWrite(D7, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(D7, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);

  StaticJsonDocument<32> doc;

  doc["status"] = "D7:" + String(digitalRead(D7));
  String output;
  serializeJson(doc, output);

  esp8266_server.send(200, "application/json", output);

}

String rootJson2() {
  StaticJsonDocument<384> doc;

  doc["info"]["ip"] = "192.168.0.103";

  JsonObject digital_pin = doc.createNestedObject("digital_pin");
  digital_pin["d0"] = String(digitalRead(D0));
  digital_pin["d1"] = String(digitalRead(D1));
  digital_pin["d2"] = String(digitalRead(D2));
  digital_pin["d3"] = String(digitalRead(D3));
  digital_pin["d4"] = String(digitalRead(D4));
  digital_pin["d5"] = String(digitalRead(D5));
  digital_pin["d6"] = String(digitalRead(D6));
  digital_pin["d7"] = String(digitalRead(D7));
  digital_pin["d8"] = String(digitalRead(D8));
  doc["analog_pin"]["a0"] = String(analogRead(A0));

  String jsonCode;
  serializeJson(doc, jsonCode);
  Serial.print("Root Json Code: ");
  Serial.println(jsonCode);
  return jsonCode;
}

void handleJson() { esp8266_server.send(200, "application/json", rootJson2()); }

// 处理LED控制请求的函数'handleLED'
void handlePin() {
  digitalWrite(D6, !digitalRead(D6));  // 改变LED的点亮或者熄灭状态
  String t = "switch " + String(digitalRead(LED_BUILTIN));
  Serial.println(t);
  StaticJsonDocument<32> doc;

  doc["status"] = "D6:" + String(digitalRead(D6));
  String output;
  serializeJson(doc, output);

  esp8266_server.send(200, "application/json", output);
}

// 设置处理404情况的函数'handleNotFound'
void handleNotFound() {
  esp8266_server.send(404, "text/plain",
                      "404: Not found");  // 发送 HTTP 状态 404 (未找到页面)
                                          // 并向浏览器发送文字 "404: Not found"
}

// 处理用户浏览器的HTTP访问
void handleUserRequet() {
  // 获取用户请求网址信息
  String webAddress = esp8266_server.uri();

  // 通过handleFileRead函数处处理用户访问
  bool fileReadOK = handleFileRead(webAddress);

  // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
  if (!fileReadOK) {
    esp8266_server.send(404, "text/plain", "404 Not Found");
  }
}

bool handleFileRead(String path) {  // 处理浏览器HTTP访问

  if (path.endsWith("/")) {  // 如果访问地址以"/"为结尾
    path = "/index.html";  // 则将访问地址修改为/index.html便于SPIFFS访问
  }

  String contentType = getContentType(path);  // 获取文件类型

  if (LittleFS.exists(path)) {  // 如果访问的文件可以在SPIFFS中找到
    File file = LittleFS.open(path, "r");  // 则尝试打开该文件
    esp8266_server.streamFile(file, contentType);  // 并且将该文件返回给浏览器
    file.close();                                  // 并且关闭文件
    return true;                                   // 返回true
  }
  return false;  // 如果文件未找到，则返回false
}

// 获取文件类型
String getContentType(String filename) {
  if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/plain";
}