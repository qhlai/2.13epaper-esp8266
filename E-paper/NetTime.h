//#pragma once
#ifndef __NETTIME_H
#define __NETTIME_H
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "WiFiConfig.h"
#include <String.h>
#include <time.h>
//#include "Lunar.h"
//#include "WiFiClient.h"
const String TimeURL = "http://quan.suning.com/getSysTime.do";//主机IP：端口/路径 或 网址

struct DATE {
	bool GetNetDate = false;
	struct tm tm;
	time_t UNIXTimeStamp;
	uint32 deviceTime_last;
	/*int Year=2020;
	int Month=8;
	int Day=18;
	int Weeday = 0;
	int Hour=18;
	int Minute = 0;
	int Second = 0;*/
	//String Lunar ="";
	

};
const String WeekDay[8] = { "Sun","Mon","Tues","Wed","Thur","Fri","Sat", "ERR" };
DATE DateHandle();
void GetNetTime();
DATE DateLocal(DATE* date);
struct tm unix2Time(time_t n);
time_t Time2unix(struct tm tm);
String toStringAddZero2(int input);
int weekDay(int y, int m, int d);
#endif 