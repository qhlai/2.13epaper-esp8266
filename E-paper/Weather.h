#pragma once
#ifndef _WEARHER_H
#define _WEARHER_H
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "WiFiConfig.h"
extern int httpCode;
extern String payload;

const String IPurl= "http://whois.pconline.com.cn/ipJson.jsp?json=true";

//http://whois.pconline.com.cn/ipJson.jsp?json=true
//http://ip-api.com/json/?lang=zh-CN
//http://ip.ws.126.net/ipquery
struct RealIP {
	bool GetRealIP = false;
	String IP = "";
	String pro = "";
	String city = "";
	String addr = "";
};
struct WeatherData {
	bool GetWeather = false;
	String city = "";
	String weather = "";
	String temperature = "";
	uint32 deviceTime_last;
};
//PROGMEM
bool GetRealIP();
RealIP RealIPHandle();
bool GetWeatherRaw();
WeatherData WeatherHandle();
#endif //_WEARHER_H