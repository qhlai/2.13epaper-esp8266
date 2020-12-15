#include <FS.h>
#include "Config.h"
#include "bitmaps/Bitmaps104x212.h" // 2.13" b/w
//#include <Fonts/TomThumb.h>//6
#define FLUSH_TIME 3
#define FLUSH_NETTIME 60
#define FLUSH_WEATHER 40
#define FLUSH_SCREEN 60
//#include "myDraw.h"
void BootMessage();
void NowWeatherShow();
void  CleanScreen();
extern WeatherData Weather;

DATE Date_Time;
DEVICEINFO DeviceInfo;
//const uint8_t FreeMonoBold9pt7bBitmaps[] PROGMEM=
int16_t tbx, tby; uint16_t tbw, tbh;
String tmp_s[6];
uint16_t center_x[6];

unsigned long noWiFi_time_start_ms=0;
unsigned long noWiFi_time_now_s=0;

void setup()
{
  Serial.begin(115200);
  display.init(115200);
  BootMessage();
//  display.powerOff();
}

void loop()
{
	Serial.println("FLUSH"+ (String)system_get_time());
	CleanScreen();
	display.setPartialWindow(0, 0, display.width(), display.height());
	//////////////////////////////////////////////////////////////	
	//draw();
	TimePartialUpdate();
	NowWeatherShow();

	//esp_wifi_fpm_set_sleep_type(wifi_sleep_type_t type);
	////////////////////////////////////////////////////////////
	display.hibernate();
	WiFi.disconnect();
	WiFi.mode(WIFI_OFF);
	WiFi.forceSleepBegin();
	delay(FLUSH_TIME*60*1000);

	//setSleepMode(WIFI_LIGHT_SLEEP)
	//ESP.deepSleep(microseconds, mode)����ʹESP8266����˯��ģʽ������microsecondsΪ˯��ʱ�䣬modeΪ˯�߻��Ѻ���ģʽ���������£�

   /* WAKE_RF_DEFAULT : do or not do the radio calibration depending on the init byte 108. ���������ߵ�У׼������init�ֽ�108��
    WAKE_RFCAL : do the radio calibration every time.ÿ�ζ�Ҫ�������ߵ�У׼��
    WAKE_NO_RFCAL : do NOT the radio calibration on wake up.��Ҫ������ʱʹ�����ߵ�У׼��
    WAKE_RF_DISABLED : on wake up DISABLE the modem. So for example I can��t connect the esp to wifi.������ʱ���õ��ƽ���������磬���޷���esp���ӵ�wifi��

ʹ��ESP.deepSleepInstant(microseconds, mode)ͬ�����Խ���˯�ߣ����÷����᲻�ȴ�ϵͳ�ر�wifiֱ�ӽ������ߣ�
*/
	//esp_wifi_fpm_set_sleep_type()
	//ESP.getVcc()//���Դ�ӡģ���Դ��ѹ��U(V) = ESP.getVcc()/1023����ʹ�øù���ʱADC��Ų��������κζ�����������Ҫ��ȫ�ֵ���ADC_MODE(ADC_VCC);�����������ô˵�ء����Ҹо�����ûɶ�ð������ǿ�����ɣ�ADC_MODE(ADC_VCC); //�л�ADCģʽ
	
}


void BootMessage()
{
	Serial.println();
	Serial.println("setup");
	Serial.print("SN:");
	DeviceInfo.SN = (String)system_get_chip_id();
	Serial.println(DeviceInfo.SN);
	//Serial.println("helloFullScreenPartialMode");
	display.fillScreen(GxEPD_WHITE);
	display.setPartialWindow(0, 0, display.width(), display.height());
	display.setRotation(0);
	//display.setFont(&Org_01); 
	display.setFont(&DejaVu_Serif_Bold_12);
	display.setTextColor(GxEPD_BLACK);

	tmp_s[0] = "BOOT Initing";
	display.getTextBounds(tmp_s[0], 0, 0, &tbx, &tby, &tbw, &tbh);
	center_x[0] = ((display.width() - tbw) / 2) - tbx;

	// first update should be full refresh
	//uint16_t hwy = ((display.height() - tbh) / 2) - tby;
	//display.firstPage();
	do
	{
		//display.fillScreen(GxEPD_WHITE);
		display.setCursor(center_x[0], 10);
		display.println(tmp_s[0]);

	} while (display.nextPage());
	display.setFont(&DejaVu_Serif_Bold_10);

	if (WiFiInit())
	{
		do
		{
			//display.fillScreen(GxEPD_WHITE);
			display.println("NoWiFi Please Reset" );
			display.println("192.168.1.0");
		} while (display.nextPage());
		ResetWiFi();
	}

	//display.firstPage();
	do
	{
		//display.fillScreen(GxEPD_WHITE);
		display.println("SN:" + DeviceInfo.SN);
		display.println("WiFi.SSID:");
		display.println(WiFi.SSID());
		if (WiFi.status() == WL_CONNECTED)
		{
			display.println("IP:" + IpAddress2String(WiFi.localIP()));
		}
		else
		{
			display.println("WiFi Connect Failed!");
		}
		//display.println(tmp_s[3]);
	} while (display.nextPage());
	Serial.println("WiFi init done");
	/*if (WiFi.status() != WL_CONNECTED)
	{

	}*/
	if (WiFi.status() == WL_CONNECTED)
	{
		Date_Time = DateHandle();
		if(Date_Time.GetNetDate == true)
		{
			do
			{
				display.println("GetNetDate OK");
				Serial.println("GetNetDate OK");
			} while (display.nextPage());
		}

	}
	else
	{
		do
		{
			display.println("GetNetDate Failed");
			Serial.println("GetNetDate Failed");
		} while (display.nextPage());
	}

	

	//display.firstPage();
	if (WiFi.status() == WL_CONNECTED)
	{
		//Date_Time = DateHandle();
		WeatherHandle();
		Serial.print("Weather:");
		Serial.println(Weather.city+ Weather.weather+ Weather.temperature);
		if (Weather.GetWeather  == true)
		{
			do
			{
				display.println("GetWeather OK");
			} while (display.nextPage());
		}
		else
		{
			do
			{
				display.println("GetWeather Failed");
			} while (display.nextPage());
		}
	}

	//display.firstPage();
	

	//display.firstPage();
	do
	{
		display.println("Will Close in 2s");
	} while (display.nextPage());

	delay(3000);
	//Flush ALL Screen
	display.setFullWindow();
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
	} while (display.nextPage());

	display.setPartialWindow(0, 0, display.width(), display.height());
	display.firstPage();
	do
	{
		display.fillScreen(GxEPD_WHITE);
	} while (display.nextPage());
	//Serial.println("helloFullScreenPartialMode done");
	Serial.println("setup done");
}

void TimePartialUpdate()
{
	bool NET = true;

	if (((system_get_time() - Date_Time.deviceTime_last) / 1000000 / 60) % FLUSH_NETTIME < FLUSH_TIME+1)
	{	
		//Serial.println("??:"+ String((system_get_time() - Date_Time.deviceTime_last) / 1000000 / 60));
		Date_Time = DateHandle();
		NET = true;
	}
	else
	{
		DateLocal(&Date_Time);
		NET = false;
	}
	
	display.setTextColor(GxEPD_BLACK);
	display.setRotation(1);
	display.firstPage();
	do
	{

		display.setFont(&DejaVu_Serif_Bold_12);
		display.setCursor(110, 16);
		display.print(String(Date_Time.tm.tm_year) + "/" + toStringAddZero2(Date_Time.tm.tm_mon) + "/" + toStringAddZero2(Date_Time.tm.tm_mday) + " "+ WeekDay[Date_Time.tm.tm_wday]);
		if (NET)
		{
		display.setCursor(250-25, 16);
		display.println("!");
		}
		else
		{
			display.setCursor(250 - 25, 16);
			display.println("?");
		}
		display.setFont(&DSEG7_Classic_Mini_Bold_40);
		display.setCursor(100, 60);
		display.println(toStringAddZero2(Date_Time.tm.tm_hour) + ":" + toStringAddZero2(Date_Time.tm.tm_min));

		display.setFont(&DSEG7_Classic_Mini_Bold_30);
		//display.fillRect(140, 120, 60, 40, GxEPD_WHITE);
		/*display.setCursor(190, 120);
		display.println(toStringAddZero2(Date_Time.Second));*/
		/*display.setCursor(100, 80);
		display.println(".");*/
	} while (display.nextPage());


}

void NowWeatherShow()
{
	static bool WeatherFlag = true;
	static bool firstWeatherFlag = true;
	if (((system_get_time()-Weather.deviceTime_last)/1000000/60) % FLUSH_WEATHER < FLUSH_TIME+1 && WeatherFlag|| firstWeatherFlag)
	{
		WeatherHandle();
		WeatherFlag = false;
		firstWeatherFlag = false;
	}
	else
	{
		WeatherFlag = true;
	}

	display.setTextColor(GxEPD_BLACK);
	display.setRotation(1);
	//display.firstPage();
	do
	{
		display.setFont(&DejaVu_Serif_Bold_12);
		display.setCursor(100, 75);
		display.println("City:"+Weather.city);
		display.setCursor(100, 87);
		display.println("Weather:" + Weather.weather );
		display.setCursor(100, 99);
		display.println("T:" + Weather.temperature + "C");
	} while (display.nextPage());
}

void draw()
{
	const unsigned char* bitmaps[] =
	{
	  WS_Bitmap104x212, Bitmap104x212_1, Bitmap104x212_2, Bitmap104x212_3
	};
	//bool m = display.mirror(true);
	//display.setCursor(10, 10);
	display.setRotation(1);
	//for (uint16_t i = 0; i < sizeof(bitmaps) / sizeof(char*); i++)
	//{
		//display.firstPage();
		do
		{
			display.fillRect(250-100, 128-100, 100, 100, GxEPD_BLACK);
			//display.fillScreen(GxEPD_WHITE);
			// void drawInvertedBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)
			//display.drawInvertedBitmap(120, 10, bitmaps[0], display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
		} while (display.nextPage());
		delay(3000);
	//}
	//display.mirror(m);
	//display.drawImage(bitmaps[i], 0, 0, 200, 200, false, mirror_y, true);
}
void  CleanScreen()
{
	static bool CleanScreenFlag = true;
	if (abs(Date_Time.tm.tm_min - FLUSH_SCREEN) < FLUSH_TIME + 1 && CleanScreenFlag )
	{

		CleanScreenFlag = false;

		display.setFullWindow();
		display.firstPage();
		do
		{
			display.fillScreen(GxEPD_WHITE);
		} while (display.nextPage());

		display.setPartialWindow(0, 0, display.width(), display.height());
		display.firstPage();
		do
		{
			display.fillScreen(GxEPD_WHITE);
		} while (display.nextPage());
	}
	else
	{
		CleanScreenFlag = true;
	}

	
}
