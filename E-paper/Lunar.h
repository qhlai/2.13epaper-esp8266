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


/*const char* ChDay[] = { "*","��һ","����","����","����","����",
					   "����","����","����","����","��ʮ",
					   "ʮһ","ʮ��","ʮ��","ʮ��","ʮ��",
					   "ʮ��","ʮ��","ʮ��","ʮ��","��ʮ",
					   "إһ","إ��","إ��","إ��","إ��",
					   "إ��","إ��","إ��","إ��","��ʮ"
						  };
const char* ChMonth[] = { "*","��","��","��","��","��","��","��","��","��","ʮ","ʮһ","��" };*/


String Lunar(int year, int month, int day);
#endif