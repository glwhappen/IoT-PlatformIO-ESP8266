#ifndef _WIFI_HEADER_
#define _WIFI_HEADER_
/**connectWifi 通过网页连接WiFi begin**/
#include <ESP8266WiFi.h>
#include <DNSServer.h>

#include <WiFiManager.h>

void connectWifi_setup();
/**通过网页连接WiFi end**/

void wifi_ap_setup();

void wifi_setup();

#endif