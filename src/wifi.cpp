#include "wifi.h"

String localIp = "none";

/**
   连接WiFi
*/
void connectWifi_setup() {
  // 建立WiFiManager对象
  WiFiManager wifiManager;

  // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
  wifiManager.autoConnect("ESP8266");

  // 如果您希望该WiFi添加密码，可以使用以下语句：
  // wifiManager.autoConnect("AutoConnectAP", "12345678");
  // 以上语句中的12345678是连接AutoConnectAP的密码

  // WiFi连接成功后将通过串口监视器输出连接成功信息
  Serial.println("");
  Serial.print("ESP8266 Connected to ");
  Serial.println(WiFi.SSID());              // WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // IP
  IPAddress ip = WiFi.localIP();
  localIp = ip.toString();
}

void wifi_ap_setup() {
 WiFi.softAP("esp8266");     // 此语句是重点。WiFi.softAP用于启动NodeMCU的AP模式。
 // 括号中有两个参数，ssid是WiFi名。password是WiFi密码。
 // 这两个参数具体内容在setup函数之前的位置进行定义。

 Serial.print("IP address: ");      // 以及NodeMCU的IP地址
 Serial.println(WiFi.softAPIP());   // 通过调用WiFi.softAPIP()可以得到NodeMCU的IP地址
}

void wifi_setup() {
  connectWifi_setup();
  wifi_ap_setup();
}