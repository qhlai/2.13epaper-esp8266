#include "NetTime.h"
int httpCode;
String payload;
String inputString = "";  //���յ�������
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
	if (WiFi.status() == WL_CONNECTED)   //ȷ��WiFi����
	{
		HTTPClient http;    //����һ���ͻ�����

		http.begin(TimeURL);      //����IP���˿�/·�� �� ��ַ���˴��õ���api
		http.addHeader("Content-Type", "text/plain");  //������Ϣ����
		//static u8 count = 0;
		httpCode = http.POST("Message from ESP8266");   //��������
		payload = http.getString();   //��ȡ����payload
		//if 
		/*������
		//Serial.println(httpCode);   //
		//Serial.println(payload);    //
		//inputString += http.getString();
		*/
		http.end();  //�ر�TCP �ͷ���Դ
		NetDate = true;
	}
	else
	{
		Serial.println("WiFi Unconnected!!!!");
	}
	//delay(1000); //��ȡʱ����
	
}
DATE DateHandle()
{
	DATE DateTime;
	GetNetTime();
	int jsonBeginAt = payload.indexOf("{");   //�ж�json����������
	int jsonEndAt = payload.lastIndexOf("}");
	if (jsonBeginAt != -1 && jsonEndAt != -1&& NetDate == true)
	{
		
			//����json����
			payload = payload.substring(jsonBeginAt, jsonEndAt + 1);//ȡ��һ��������JSON�ַ���

			const size_t capacity = JSON_OBJECT_SIZE(2) + 60;
			DynamicJsonDocument doc(capacity);
			//const char* json = "{\"sysTime2\":\"2020-03-07 19:13:28\",\"sysTime1\":\"20200307191328\"}";
			deserializeJson(doc, payload);//����payload

			const char* sysTime2 = doc["sysTime2"]; // "2020-03-07 19:13:28"
			const char* sysTime1 = doc["sysTime1"]; // "20200307191328"

			String Date = sysTime1;
			//int nian = sysTime2.lastIndexOf("-");//��λ��ĳ��
			//int tm_sec;  /*�룬������Χ0-59�� ��������61*/
			//int tm_min;  /*���ӣ�0-59*/
			//int tm_hour; /*Сʱ�� 0-23*/
			//int tm_mday; /*�գ���һ�����еĵڼ��죬1-31*/
			//int tm_mon;  /*�£� ��һ������0-11 1+p->tm_mon;  */
			//int tm_year;  /*�꣬ ��1900�����Ѿ������� 1900�� p->tm_year;  */
			//int tm_wday; /*���ڣ�һ���еĵڼ��죬 ������������0-6*/
			//int tm_yday; /*�ӽ���1��1�յ�Ŀǰ����������Χ0-365*/
			//int tm_isdst; /*�չ��Լʱ������*/
			DateTime.tm.tm_year = (Date.substring(0, 4)).toInt();//��ȡ���±��0��ʼ��[0,4)������
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
			/*������
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
	//sprintf(buffer, "%02d", input);//ת��123Ϊ"00123"
	//puts(buffer);
	
	return buffer;
}
int weekDay(int y, int m, int d)
{
	int i;
	int r = 0;  //����1900��������������
	int a[] = { 31,28,31,30,31,30,31,31,30,31,30 }; //m��ÿ���µ�����
	if (y >= 1900 && m > 0 && m < 13 && d>0 && d < 32)
	{
		for (i = 1900; i <= y; i++)  //��������ĸ���
		{
			if (i % 400 == 0 || i % 100 != 0 && i % 4 == 0)
				r++;
		}
		for (i = 0; i < m - 1; i++)  //ͳ�Ƶ���1��1�յ����µ�����
		{
			d += a[i];
		}
		if ((y % 400 == 0 || y % 100 != 0 && y % 4 == 0) && m < 3)  //������������굫�������¾Ͳ������ȥ
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
