//#pragma once
#ifndef __WiFiCONFIG_H
#define __WiFiCONFIG_H
#include <ESP8266WiFi.h>
//---------������---------
#include <ESP8266WebServer.h>  
#include <FS.h> 

#include <string.h>
#include <math.h>  // ��ѧ���߿�
#include <EEPROM.h>// eeprom  �洢��
//#include <ESP8266WiFiMulti.h>

// ���ڴ��ϴε�WIFI������
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