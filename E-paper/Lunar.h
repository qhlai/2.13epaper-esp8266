#pragma once
#ifndef LUNAR_H_
#define LUNAR_H_
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <stdio.h>
#include <String.h>
#include <stdlib.h>
//using namespace std;


/*const char* ChDay[] = { "*","初一","初二","初三","初四","初五",
					   "初六","初七","初八","初九","初十",
					   "十一","十二","十三","十四","十五",
					   "十六","十七","十八","十九","二十",
					   "廿一","廿二","廿三","廿四","廿五",
					   "廿六","廿七","廿八","廿九","三十"
						  };
const char* ChMonth[] = { "*","正","二","三","四","五","六","七","八","九","十","十一","腊" };*/


String Lunar(int year, int month, int day);
#endif