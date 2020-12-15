#include "Openwrt.h"

String postRequest = (String)("POST ") + OPENWRT_LUCI_ADDR + "/ HTTP/1.1\r\n" +
"Host: " + OPENWRT_LUCI_HOST + "\r\n" +
"Connection: keep-alive\r\n" +
"Content-Length: 41\r\n" +
"Cache-Control: max-age=0\r\n" +
"Origin: http://192.168.2.1\r\n" +
"Upgrade-Insecure-Requests: 1\r\n" +
"DNT: 1\r\n" +
"Content-Type: application/x-www-form-urlencoded\r\n" +
"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.125 Safari/537.36\r\n" +
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n" +
"Referer: http://192.168.2.1/cgi-bin/luci\r\n" +
"Accept-Encoding: gzip, deflate\r\n" +
"Accept-Language: zh-CN,zh;q=0.9\r\n" +
"Cookie: agh_session=fe3710435f2c3bc8afa3af820f23c415c5e8b7b4b0cdec4403e4bfd409d42046\r\n\r\n" +
"luci_username="+ OPENWRT_LUCI_NAME +"&"+"luci_password="+ OPENWRT_LUCI_PASSWORD +"\r\n\r\n";

String getRequest = (String)("GET ") + OPENWRT_LUCI_URL + "/ HTTP/1.1\r\n" +
"Host: " + OPENWRT_LUCI_HOST + "\r\n" +
"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:79.0) Gecko/20100101 Firefox/79.0\r\n" +
"Content-Type: text/html;charset=utf-8\r\n" +
"Accept: */*\r\n" +
"Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2" +
"Accept-Encoding: gzip, deflate\r\n" +
"DNT: 1\r\n" +
"Connection: Keep Alive\r\n\r\n";
void GetOpenwrtData()
{
	if (WiFi.status() == WL_CONNECTED)   //确保WiFi连接
	{
		WiFiClient client;
		HTTPClient http;    //定义一个客户端类
		//test connection
		/*if (!client.connect(OPENWRT_LUCI_HOST,80)) {
			Serial.println("connection failed");
			return;
		}
		delay(10);*/

		Serial.println(postRequest);
		client.print(postRequest);  // 发送HTTP请求

		http.begin(OPENWRT_LUCI_HOST);      //主机IP：端口/路径 或 网址，此处用的是api
		//http.("Content-Type", "text/plain");  //设置消息类型
		//httpCode = http.GET(getRequest);
		httpCode = http.POST(postRequest);   //发送请求
		payload = http.getString();                  //获取返回payload
		//测试用
		Serial.println(httpCode);   //
		Serial.println(payload);    //
		inputString += http.getString();
		String line = client.readStringUntil('\n');
		while (line.length() != 0) {
			Serial.println(line);
			line = client.readStringUntil('\n');
		}
		Serial.println(line);
		//http.end();  //关闭TCP 释放资源
		
	}
	else
	{
		Serial.println("WiFi Unconnected!!!!");
	}
	delay(1000); //获取时间间隔
	
}
void DataHandle()
{
	//GetTime();
	int jsonBeginAt = payload.indexOf("{");   //判断json数据完整性
	int jsonEndAt = payload.lastIndexOf("}");
	if (jsonBeginAt != -1 && jsonEndAt != -1)
	{
		if ( true)
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
			
			/*测试用
			//Serial.println(Date);
			//Serial.println(Year);
			//Serial.println(Second);
			*/
			
		}
		//return DateTime;
	}
	//return NULL;
}