#pragma once
#ifndef __CONFIG_H
#define __CONFIG_H
#include <WiFiUdp.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecure.h>
#include <WiFiServer.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>
#include <CertStoreBearSSL.h>
#include <BearSSLHelpers.h>
#include <dummy.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <u8g2_fonts.h>
#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_MonoOLED.h>
#include <Adafruit_GFX.h>
#include <GxEPD2_GFX.h>
#include <GxEPD2_EPD.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_7C.h>
#include <GxEPD2_3C.h>
#include <GxEPD2.h>
//#include "Driver.h"
//#include "Config.h"
#include "Driver.h"
#include <FS.h>
#include "./myFonts/DejaVu_Serif_Bold_12.h"
#include "./myFonts/DejaVu_Serif_Bold_10.h"
#include <Fonts/FreeMonoBold9pt7b.h>//18
#include <Fonts/Org_01.h>//����7
#include <Fonts/Picopixel.h>//7
#include "./myFonts/DSEG7_Classic_Mini_Bold_24.h"
#include "./myFonts/DSEG7_Classic_Mini_Bold_30.h"
#include "./myFonts/DSEG7_Classic_Mini_Bold_40.h"

#include <GxEPD2.h>
#include <string.h>

#include "WiFiConfig.h"
#include "NetTime.h"
//#include "Openwrt.h"
//#include "Text.h"
#include "Weather.h"

struct WiFiINFO {
	boolean Alive = false;
	String SSID = " ";
	String IP = " ";
};
struct DEVICEINFO {
	String SN = "0";
	struct WiFiINFO WiFiInfo;
	//boolean WiFiAlive=false;
	boolean NetTime = false;
	boolean Weather = false;
	boolean Openwrt = false;
};

extern  DEVICEINFO DeviceInfo;



//display QRCode
#define BADGE_CONFIG_FILE_NAME "/badge.data"
#define DEFALUT_AVATAR_BMP "/avatar.bmp"
#define DEFALUT_QR_CODE_BMP "/qr.bmp"

//sleep to save elec
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  18000        /* Time ESP32 will go to sleep (in seconds) */
#define SLEEPTIME 1
//#define BME680_FLASHTIME 20


#endif
