//#pragma once
#ifndef __WiFiCONFIG_H
#define __WiFiCONFIG_H
#include <ESP8266WiFi.h>
//---------服务器---------
#include <ESP8266WebServer.h>  
#include <FS.h> 

#include <string.h>
#include <math.h>  // 数学工具库
#include <EEPROM.h>// eeprom  存储库
//#include <ESP8266WiFiMulti.h>

// 用于存上次的WIFI和密码
#define MAGIC_NUMBER 0xAB
#define DEFAULT_STASSID "HIT"
#define DEFAULT_STAPSW  "a299792458"

struct WiFiSetting
{
	char stassid[50];
	char stapsw[50];
	uint8_t magic;
};

//extern char* IP;

String IpAddress2String(const IPAddress& ipAddress);
int WiFiInit();
int ResetWiFi();
void saveConfig();
void loadConfig();
void Server_int();
void SET_AP();
void handleNotFound();
void handleMain();
void handlePin();
void handleWifi();
//void http_jiexi(http_request ruqest);
void handleWifi_wangye();
void handleTest();

#endif