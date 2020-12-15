#include "Weather.h"

const String APIKEY  = "SKXG98d1PrF9i9vk6";
//http://api.seniverse.com/v3/weather/now.json?key=SKXG98d1PrF9i9vk6&location=beijing&language=en&unit=c
RealIP RealIPData;
WeatherData Weather;
bool GetRealIP()
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

		http.begin(IPurl);      //����IP���˿�/·�� �� ��ַ���˴��õ���api
		http.addHeader("Content-Type", "text/plain");  //������Ϣ����

		httpCode = http.POST("Message from ESP8266");   //��������
		payload = http.getString();                  //��ȡ����payload
		////������
		//Serial.println(httpCode);   //
		//Serial.println(payload);    //
		//inputString += http.getString();
		
		http.end();  //�ر�TCP �ͷ���Դ
		return true;
	}
	else
	{
		Serial.println("GetRealIP Failed");
		return false;
	}
	//delay(1000); //��ȡʱ����

}
RealIP RealIPHandle()
{
	//DATE DateTime;
	if (GetRealIP() == false)
	{
		return RealIPData;
	}

	int jsonBeginAt = payload.indexOf("{");   //�ж�json����������
	int jsonEndAt = payload.lastIndexOf("}");
	if (jsonBeginAt != -1 && jsonEndAt != -1)
	{

		//����json����
		payload = payload.substring(jsonBeginAt, jsonEndAt + 1);//ȡ��һ��������JSON�ַ���

		const size_t capacity = JSON_OBJECT_SIZE(2) + 128;
		DynamicJsonDocument doc(capacity);
		//{"ip":"223.73.111.131","pro":"�㶫ʡ","proCode":"440000","city":"������","cityCode":"440300","region":"","regionCode":"0","addr":"�㶫ʡ������ �ƶ�","regionNames":"","err":""} 
		//const char* json = "{\"sysTime2\":\"2020-03-07 19:13:28\",\"sysTime1\":\"20200307191328\"}";
		deserializeJson(doc, payload);//����payload

		//const char* sysTime2 = doc["sysTime2"]; // "2020-03-07 19:13:28"
		//const char* temp = doc["ip"]; // "20200307191328"

		//int nian = sysTime2.lastIndexOf("-");//��λ��ĳ��
		RealIPData.IP = (const char*) doc["ip"];
		//RealIPData.
		RealIPData.GetRealIP = true;
		/*������
		
		*/
		Serial.print("RealIPData.IP:");
		Serial.println(RealIPData.IP);


	}
	else
	{
		RealIPData.GetRealIP  = false;
	}
	return RealIPData;
	//return NULL;
}


bool GetWeatherRaw() {

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
	RealIPHandle();
	HTTPClient http;
	String Url = String("http://api.seniverse.com/v3/weather/now.json?key=") + String(APIKEY) + String("&location=")+RealIPData.IP + String("&language=en&unit=c");
	//Serial.println("GET URL: %s"+ Url);
	http.begin(Url);
	int httpCode = http.GET();
	if (httpCode > 0) {
		//Serial.printf("[HTTP] GET... code: %d\n", httpCode);

		if (httpCode == 200) {
			payload = http.getString();
			//Serial.println(payload);
			return true;
		}
		else {
			delay(5000);
			GetWeatherRaw();
		}
	}
	else {
		delay(5000);
		GetWeatherRaw();
	}
	//Serial.println(payload);
	//strcpy(response, payload.c_str());       // convert to const char *
	//free(Url);
	return false;
}
WeatherData WeatherHandle()
{

	//DATE DateTime;
	if (GetWeatherRaw() == false)
	{
		return Weather;
	}
	int jsonBeginAt = payload.indexOf("{");   //�ж�json����������
	int jsonEndAt = payload.lastIndexOf("}");
	if (jsonBeginAt != -1 && jsonEndAt != -1)
	{

		//����json����
		payload = payload.substring(jsonBeginAt, jsonEndAt + 1);//ȡ��һ��������JSON�ַ���
		//Serial.print("payload:");
		//Serial.println(payload);
		const size_t capacity = JSON_OBJECT_SIZE(2) + 512;
		DynamicJsonDocument doc(capacity);
		//doc.getElement
		//{"ip":"223.73.111.131","pro":"�㶫ʡ","proCode":"440000","city":"������","cityCode":"440300","region":"","regionCode":"0","addr":"�㶫ʡ������ �ƶ�","regionNames":"","err":""} 
		//const char* json = "{\"sysTime2\":\"2020-03-07 19:13:28\",\"sysTime1\":\"20200307191328\"}";
		deserializeJson(doc, payload);//����payload
		//const char* sysTime2 = doc["sysTime2"]; // "2020-03-07 19:13:28"

		//int nian = sysTime2.lastIndexOf("-");//��λ��ĳ��
		Weather.city = (const char*)doc["results"][0]["location"]["name"];
		Weather.weather = (const char*)doc["results"][0]["now"]["text"];
		Weather.temperature = (const char*)doc["results"][0]["now"]["temperature"];
		Weather.deviceTime_last= system_get_time();
		//RealIPData.
		Weather.GetWeather = true;
		/*������

		*/
		//Serial.print("Weather:");
		//Serial.println(Weather.city+ Weather.weather+ Weather.temperature);


	}
	else
	{
		Weather.GetWeather = false;
	}
	return Weather;
	//return NULL;
}

//void clearResponseBuffer() {
//	memset(response, 0, MAX_CONTENT_SIZE);      //???
//	memset(d.city, 0, sizeof(d.city));
//	memset(d.weather, 0, sizeof(d.weather));
//	memset(d.temp, 0, sizeof(d.temp));
//}
//
//data getdata() {
//
//	char error[5] = "NULL";
//	clearResponseBuffer();
//
//	if (sendRequest(HOST, CITY, APIKEY)) {
//		Serial.println("Request success!\n\n");
//	}
//	if (parseData(d)) {
//		return d;
//	}
//	else {
//		strcpy(d.city, error);
//		strcpy(d.weather, error);
//		strcpy(d.temp, error);
//
//		return d;
//	}
//}
//
//
//bool parseData(data &d) {
//
//	DynamicJsonBuffer jsonBuffer;
//
//	JsonObject& root = jsonBuffer.parseObject(response);
//
//	if (!root.success()) {
//		Serial.println("JSON parse failed!\n");
//		return false;
//	}
//
//	strcpy(d.city, root["results"][0]["location"]["name"]);
//	strcpy(d.weather, root["results"][0]["now"]["text"]);
//	strcpy(d.temp, root["results"][0]["now"]["temperature"]);
//
//	return true;
//}

