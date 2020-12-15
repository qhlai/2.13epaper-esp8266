#include "NetTime.h"
int httpCode;
String payload;
String inputString = "";  //接收到的数据
bool NetDate = false;
bool GetNetDate = false;

void GetNetTime()
{
	if (WiFi.status() != WL_CONNECTED)
	{
		if (WiFiInit())
		{
			//do
			//{
			//	//display.fillScreen(GxEPD_WHITE);
			//	display.println("NoWiFi");
			//	display.println("Please Reset");
			//	display.println("192.168.1.0");
			//} while (display.nextPage());
			//ResetWiFi();
		}
	}
	if (WiFi.status() == WL_CONNECTED)   //确保WiFi连接
	{
		HTTPClient http;    //定义一个客户端类

		http.begin(TimeURL);      //主机IP：端口/路径 或 网址，此处用的是api
		http.addHeader("Content-Type", "text/plain");  //设置消息类型
		//static u8 count = 0;
		httpCode = http.POST("Message from ESP8266");   //发送请求
		payload = http.getString();   //获取返回payload
		//if 
		/*测试用
		//Serial.println(httpCode);   //
		//Serial.println(payload);    //
		//inputString += http.getString();
		*/
		http.end();  //关闭TCP 释放资源
		NetDate = true;
	}
	else
	{
		Serial.println("WiFi Unconnected!!!!");
	}
	//delay(1000); //获取时间间隔
	
}
DATE DateHandle()
{
	DATE DateTime;
	GetNetTime();
	int jsonBeginAt = payload.indexOf("{");   //判断json数据完整性
	int jsonEndAt = payload.lastIndexOf("}");
	if (jsonBeginAt != -1 && jsonEndAt != -1&& NetDate == true)
	{
		
			//净化json数据
			payload = payload.substring(jsonBeginAt, jsonEndAt + 1);//取得一个完整的JSON字符串

			const size_t capacity = JSON_OBJECT_SIZE(2) + 60;
			DynamicJsonDocument doc(capacity);
			//const char* json = "{\"sysTime2\":\"2020-03-07 19:13:28\",\"sysTime1\":\"20200307191328\"}";
			deserializeJson(doc, payload);//解析payload

			const char* sysTime2 = doc["sysTime2"]; // "2020-03-07 19:13:28"
			const char* sysTime1 = doc["sysTime1"]; // "20200307191328"

			String Date = sysTime1;
			//int nian = sysTime2.lastIndexOf("-");//定位到某处
			//int tm_sec;  /*秒，正常范围0-59， 但允许至61*/
			//int tm_min;  /*分钟，0-59*/
			//int tm_hour; /*小时， 0-23*/
			//int tm_mday; /*日，即一个月中的第几天，1-31*/
			//int tm_mon;  /*月， 从一月算起，0-11 1+p->tm_mon;  */
			//int tm_year;  /*年， 从1900至今已经多少年 1900＋ p->tm_year;  */
			//int tm_wday; /*星期，一周中的第几天， 从星期日算起，0-6*/
			//int tm_yday; /*从今年1月1日到目前的天数，范围0-365*/
			//int tm_isdst; /*日光节约时间的旗标*/
			DateTime.tm.tm_year = (Date.substring(0, 4)).toInt();//截取（下标从0开始）[0,4)的数据
			DateTime.tm.tm_mon = (Date.substring(4, 6)).toInt();
			DateTime.tm.tm_mday = (Date.substring(6, 8)).toInt();
			DateTime.tm.tm_hour = (Date.substring(8, 10)).toInt();
			DateTime.tm.tm_min = (Date.substring(10, 12)).toInt();
			DateTime.tm.tm_sec = (Date.substring(12, 14)).toInt();
			DateTime.tm.tm_wday = weekDay(DateTime.tm.tm_year, DateTime.tm.tm_mon, DateTime.tm.tm_mday);
			DateTime.UNIXTimeStamp = Time2unix(DateTime.tm);
			DateTime.deviceTime_last =system_get_time();
			//DateTime.Lunar = String(Lunar(DateTime.Year, DateTime.Month, DateTime.Day));
			DateTime.GetNetDate = true;
			/*测试用
			//Serial.println(Date);
			//Serial.println(Year);
			//Serial.println(Second);
			*/
			GetNetDate = true;
			NetDate = false;
		
	}
	else
    {
            DateTime.GetNetDate = false;
     }
	return DateTime;
	//return NULL;
}
DATE DateLocal(DATE* date)
{
	date->tm = unix2Time((system_get_time() - date->deviceTime_last) / 1000000 + date->UNIXTimeStamp);
}
String toStringAddZero2(int input)
{
	String buffer =String(input);
	if (input < 10)buffer = "0" + buffer;
	//char buffer[2];
	//sprintf(buffer, "%02d", input);//转换123为"00123"
	//puts(buffer);
	
	return buffer;
}
int weekDay(int y, int m, int d)
{
	int i;
	int r = 0;  //储存1900到该天的闰年个数
	int a[] = { 31,28,31,30,31,30,31,31,30,31,30 }; //m给每出月的天数
	if (y >= 1900 && m > 0 && m < 13 && d>0 && d < 32)
	{
		for (i = 1900; i <= y; i++)  //计算闰年的个数
		{
			if (i % 400 == 0 || i % 100 != 0 && i % 4 == 0)
				r++;
		}
		for (i = 0; i < m - 1; i++)  //统计当年1月1日到当月的天数
		{
			d += a[i];
		}
		if ((y % 400 == 0 || y % 100 != 0 && y % 4 == 0) && m < 3)  //如果当年是闰年但不到二月就不能算进去
			r--;
		return (365 * (y - 1900) + r + d) % 7;
	}
	else
		return  7;
}
struct tm unix2Time(time_t n)
{
	struct tm tm = *localtime((time_t*)&n);
	//date->tm = tm;
	return tm;
	//tm.tm_sec=
	//strftime(strTime, bufLen - 1, "%Y-%m-%d %H:%M:%S", &tm);
	//strTime[bufLen - 1] = '\0';
}
time_t Time2unix(struct tm tm)
{
	//struct tm tm;
	//memset(&tm, 0, sizeof(tm));

	/*sscanf(timeStamp, "%d-%d-%d %d:%d:%d",
		&tm.tm_year, &tm.tm_mon, &tm.tm_mday,
		&tm.tm_hour, &tm.tm_min, &tm.tm_sec);*/

	tm.tm_year -= 1900;
	tm.tm_mon--;

	return mktime(&tm);
}
