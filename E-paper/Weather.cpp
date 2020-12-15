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
	if (WiFi.status() == WL_CONNECTED)   //确保WiFi连接
	{
		HTTPClient http;    //定义一个客户端类

		http.begin(IPurl);      //主机IP：端口/路径 或 网址，此处用的是api
		http.addHeader("Content-Type", "text/plain");  //设置消息类型

		httpCode = http.POST("Message from ESP8266");   //发送请求
		payload = http.getString();                  //获取返回payload
		////测试用
		//Serial.println(httpCode);   //
		//Serial.println(payload);    //
		//inputString += http.getString();
		
		http.end();  //关闭TCP 释放资源
		return true;
	}
	else
	{
		Serial.println("GetRealIP Failed");
		return false;
	}
	//delay(1000); //获取时间间隔

}
RealIP RealIPHandle()
{
	//DATE DateTime;
	if (GetRealIP() == false)
	{
		return RealIPData;
	}

	int jsonBeginAt = payload.indexOf("{");   //判断json数据完整性
	int jsonEndAt = payload.lastIndexOf("}");
	if (jsonBeginAt != -1 && jsonEndAt != -1)
	{

		//净化json数据
		payload = payload.substring(jsonBeginAt, jsonEndAt + 1);//取得一个完整的JSON字符串

		const size_t capacity = JSON_OBJECT_SIZE(2) + 128;
		DynamicJsonDocument doc(capacity);
		//{"ip":"223.73.111.131","pro":"广东省","proCode":"440000","city":"深圳市","cityCode":"440300","region":"","regionCode":"0","addr":"广东省深圳市 移动","regionNames":"","err":""} 
		//const char* json = "{\"sysTime2\":\"2020-03-07 19:13:28\",\"sysTime1\":\"20200307191328\"}";
		deserializeJson(doc, payload);//解析payload

		//const char* sysTime2 = doc["sysTime2"]; // "2020-03-07 19:13:28"
		//const char* temp = doc["ip"]; // "20200307191328"

		//int nian = sysTime2.lastIndexOf("-");//定位到某处
		RealIPData.IP = (const char*) doc["ip"];
		//RealIPData.
		RealIPData.GetRealIP = true;
		/*测试用
		
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
	int jsonBeginAt = payload.indexOf("{");   //判断json数据完整性
	int jsonEndAt = payload.lastIndexOf("}");
	if (jsonBeginAt != -1 && jsonEndAt != -1)
	{

		//净化json数据
		payload = payload.substring(jsonBeginAt, jsonEndAt + 1);//取得一个完整的JSON字符串
		//Serial.print("payload:");
		//Serial.println(payload);
		const size_t capacity = JSON_OBJECT_SIZE(2) + 512;
		DynamicJsonDocument doc(capacity);
		//doc.getElement
		//{"ip":"223.73.111.131","pro":"广东省","proCode":"440000","city":"深圳市","cityCode":"440300","region":"","regionCode":"0","addr":"广东省深圳市 移动","regionNames":"","err":""} 
		//const char* json = "{\"sysTime2\":\"2020-03-07 19:13:28\",\"sysTime1\":\"20200307191328\"}";
		deserializeJson(doc, payload);//解析payload
		//const char* sysTime2 = doc["sysTime2"]; // "2020-03-07 19:13:28"

		//int nian = sysTime2.lastIndexOf("-");//定位到某处
		Weather.city = (const char*)doc["results"][0]["location"]["name"];
		Weather.weather = (const char*)doc["results"][0]["now"]["text"];
		Weather.temperature = (const char*)doc["results"][0]["now"]["temperature"];
		Weather.deviceTime_last= system_get_time();
		//RealIPData.
		Weather.GetWeather = true;
		/*测试用

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

