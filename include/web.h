#ifndef _WEB_HEADER_
#define _WEB_HEADER_

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"

/**webServer begin**/

void handlePin();
void handleOpenComputer2();
void handleOpenComputer();
void handleHome();
void handleNotFound();
void webServer_setup();
void webServer_loop();

void handleUserRequet();
bool handleFileRead(String path);
String getContentType(String filename);
/**webServer end**/

#endif