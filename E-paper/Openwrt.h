#pragma once
#ifndef __OPENWRT_CLIENT_H
#define __OPENWRT_CLIENT_H
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
//#include "Config.h"
#define OPENWRT_LUCI_HOST  "192.168.2.1"
#define OPENWRT_LUCI_URL "/cgi-bin/luci"
#define OPENWRT_LUCI_URL_STATUS "/?status=1"
#define OPENWRT_LUCI_ADDR  "http://"+OPENWRT_LUCI_HOST+OPENWRT_LUCI_URL
#define OPENWRT_LUCI_ADDR_STATUS  "http://"+OPENWRT_LUCI_HOST+OPENWRT_LUCI_URL+OPENWRT_LUCI_URL_STATUS

#define OPENWRT_LUCI_NAME  "root"
#define OPENWRT_LUCI_PASSWORD "password"
struct OpenwrtCpuInfo {
	u8_t Freq;
	u8 Temper;
	u8_t Cpuusage;
};
struct OpenwrtTime {
    u8_t Year;
	u8_t Mounth;
	u8_t Day;
	u8_t Hour;
	u8_t Second;
	char Day_s[3];
	char Mon_s[3];
};
struct OpenwrtMemInfo {
	u16_t total;
	u16_t shared;
	u16_t free;
	u16_t cached;
	u16_t available;
	u16 buffered;
};
struct OpenwrtLeasesInfo {
	u16_t Expires;
	char Ipaddr[15];
	char MacAddr[17];//real 17
	char hostname[32];
};
struct OpenwrtLeases6Info {
	u16_t Expires;
	char Ip6addr[32];
	char MacAddr[17];//real 17
	char hostname[32];
};
struct OpenwrtData {
	OpenwrtTime Time;
	OpenwrtCpuInfo CpuInfo;
	OpenwrtMemInfo MemInfo;
	OpenwrtLeasesInfo LeasesInfo[32];
	OpenwrtLeases6Info Leases6Info[8];
};
extern int httpCode;
extern String payload;
extern String inputString;  //接收到的数据
extern bool GetNetDate;
void GetOpenwrtData();
#endif