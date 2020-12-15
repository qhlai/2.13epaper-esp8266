#include "WiFiConfig.h"
ESP8266WebServer server(80);
//ESP8266WiFiMulti WiFiMulti;
WiFiSetting wifi_config;
/*----------------WIFI账号和密码--------------*/

String IpAddress2String(const IPAddress& ipAddress)
{
	return String(ipAddress[0]) + String(".") + \
		String(ipAddress[1]) + String(".") + \
		String(ipAddress[2]) + String(".") + \
		String(ipAddress[3]);
}
//boolean connectioWasAlive = false;

int WiFiInit()
{
	loadConfig();// 读取信息 WIFI

	Serial.print("Connecting to ");	
	Serial.println(wifi_config.stassid);

	WiFi.begin(wifi_config.stassid, wifi_config.stapsw);   //WiFi connection
	//WiFi.begin();   //WiFi connection
	//WiFi.begin();   //WiFi connection
 //   WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

	u8_t count = 0;
	while (WiFi.status() != WL_CONNECTED)   //Wait for the WiFI connection completion
	{ 
		delay(500);
		Serial.print(".");
		count++; 
		if (count ==30)//10s
		{
			Serial.println("WiFi Connect Failed");
			return 1;
			
			//return String("WiFi Connect Failed");
		}
		//   display.print(".");  
	}
	            // NodeMCU将通过串口监视器输出。
	Serial.print(WiFi.SSID());   
	Serial.println(" Connected");           // 连接的WiFI名称
	Serial.println("");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	return 0;
	//return  String("IP:") + IpAddress2String(WiFi.localIP());
}
int ResetWiFi()
{
	//WiFiMulti.cleanAPlist();
	Serial.println("WiFi Reset");
	ESP.wdtFeed();
	server.handleClient();
	SET_AP();
	SPIFFS.begin();
	Server_int();
	while (1)
	{
		server.handleClient();
	}
}

/*
 *3-/2 保存参数到EEPROM
*/
void saveConfig()
{
	Serial.println("Save config!");
	Serial.print("stassid:");
	Serial.println(wifi_config.stassid);
	Serial.print("stapsw:");
	Serial.println(wifi_config.stapsw);
	EEPROM.begin(256);
	uint8_t *p = (uint8_t*)(&wifi_config);
	for (int i = 0; i < sizeof(wifi_config); i++)
	{
		EEPROM.write(i, *(p + i));
	}
	EEPROM.commit();
}
/*
 * 从EEPROM加载参数
*/
void loadConfig()
{
	EEPROM.begin(256);
	uint8_t *p = (uint8_t*)(&wifi_config);
	for (int i = 0; i < sizeof(wifi_config); i++)
	{
		*(p + i) = EEPROM.read(i);
	}
	EEPROM.commit();
	//出厂自带
	if (wifi_config.magic != MAGIC_NUMBER)
	{
		strcpy(wifi_config.stassid, DEFAULT_STASSID);
		strcpy(wifi_config.stapsw, DEFAULT_STAPSW);
		wifi_config.magic = MAGIC_NUMBER;
		saveConfig();
		Serial.println("Restore config!");
	}
	Serial.println(" ");
	Serial.println("-----Read config-----");
	Serial.print("stassid:");
	Serial.println(wifi_config.stassid);
	Serial.print("stapsw:");
	Serial.println(wifi_config.stapsw);
	Serial.println("-------------------");

	//  ssid=String(config.stassid);
	//  password=String(config.stapsw);
}
// 3 - 3ESP8266建立无线热点
void SET_AP() {

	// 设置内网
	IPAddress softLocal(192, 168, 1, 0);   // 1 设置内网WIFI IP地址
	IPAddress softGateway(192, 168, 1, 0);
	IPAddress softSubnet(255, 255, 255, 0);
	WiFi.softAPConfig(softLocal, softGateway, softSubnet);

	String apName = ("ESP_" + (String)ESP.getChipId());  // 2 设置WIFI名称
	const char *softAPName = apName.c_str();

	WiFi.softAP(softAPName, "esp12345");      // 3创建wifi  名称 +密码 adminadmin

	Serial.print("softAPName: ");  // 5输出WIFI 名称
	Serial.println(apName);

	IPAddress myIP = WiFi.softAPIP();  // 4输出创建的WIFI IP地址
	Serial.print("AP IP address: ");
	Serial.println(myIP);

}
//3-4ESP建立网页服务器
void Server_int() {

	server.on("/", handleMain); // 绑定‘/’地址到handleMain方法处理 ----  返回主页面 一键配网页面 
	//server.on("/pin", HTTP_GET, handlePin); // 绑定‘/pin’地址到handlePin方法处理  ---- 开关灯请求 
	server.on("/wifi", HTTP_GET, handleWifi); // 绑定‘/wifi’地址到handlePWIFI方法处理  --- 重新配网请求
	//server.on("/wifi_wangye", HTTP_GET, handleWifi_wangye);
	//server.on("/test", HTTP_GET, handleTest);
	//server.onNotFound(handleNotFound); // NotFound处理
	server.begin();

	Serial.println("HTTP server started");

}
//3-5-1 网页服务器主页
void handleMain() {

	/* 返回信息给浏览器（状态码，Content-type， 内容）

	 * 这里是访问当前设备ip直接返回一个String

	 */
	Serial.println("handleMain");
	File file = SPIFFS.open("/index.html", "r");
	size_t sent = server.streamFile(file, "text/html");
	file.close();

	return;

}
////3-5-2 网页控制引脚
///* 引脚更改处理
//
// * 访问地址为htp://192.162.xxx.xxx/pin?a=XXX的时候根据a的值来进行对应的处理
//
// */
//
//void handlePin() {
//
//	if (server.hasArg("a")) { // 请求中是否包含有a的参数  
//
//		String action = server.arg("a"); // 获得a参数的值  
//
//		if (action == "on") { // a=on  
//
//			digitalWrite(2, LOW); // 点亮8266上的蓝色led，led是低电平驱动，需要拉低才能亮  
//
//			server.send(200, "text/html", "Pin 2 has turn on"); return; // 返回数据  
//
//		}
//		else if (action == "off") { // a=off  
//
//			digitalWrite(2, HIGH); // 熄灭板载led  
//
//			server.send(200, "text/html", "Pin 2 has turn off"); return;
//
//		}
//
//		server.send(200, "text/html", "unknown action"); return;
//
//	}
//
//	server.send(200, "text/html", "action no found");
//
//}
//
////3-5-3 网页修改普通家庭WIFI连接账号密码
///* WIFI更改处理
//
// * 访问地址为htp://192.162.xxx.xxx/wifi?config=on&name=Testwifi&pwd=123456
//  根据wifi进入 WIFI数据处理函数
//  根据config的值来进行 on
//  根据name的值来进行  wifi名字传输
//  根据pwd的值来进行   wifi密码传输
//
//
// */
//
//
void handleWifi() {

	Serial.println("handleWifi");
	if (server.hasArg("config")) { // 请求中是否包含有a的参数  

		String config = server.arg("config"); // 获得a参数的值  
		String wifiname;
		String wifipwd;

		if (config == "on") { // a=on  
			if (server.hasArg("name")) { // 请求中是否包含有a的参数  
				wifiname = server.arg("name"); // 获得a参数的值
			}

			if (server.hasArg("pwd")) { // 请求中是否包含有a的参数  
				wifipwd = server.arg("pwd"); // 获得a参数的值   
			}

			String backtxt = "Wifiname: " + wifiname + "/r/n wifipwd: " + wifipwd;// 用于串口和网页返回信息

			Serial.println(backtxt); // 串口打印给电脑

			server.send(200, "text/html", backtxt); // 网页返回给手机提示
			 // wifi连接开始

			wifiname.toCharArray(wifi_config.stassid, 50);    // 从网页得到的 WIFI名
			wifipwd.toCharArray(wifi_config.stapsw, 50);  //从网页得到的 WIFI密码              

			saveConfig();
			WiFiInit();
			ESP.reset();
			return;
		}
		else if (config == "off") { // a=off  
			server.send(200, "text/html", "config  is off!");
			return;
		}
		server.send(200, "text/html", "unknown action"); return;
	}

	server.send(200, "text/html", "action no found");

}
//
////3-5-。。。 网页没有对应请求如何处理
//void handleNotFound() {
//
//	String path = server.uri();
//
//	Serial.print("load url:");
//
//	Serial.println(path);
//
//	String contentType = getContentType(path);
//
//	String pathWithGz = path + ".gz";
//
//	if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
//
//		if (SPIFFS.exists(pathWithGz))
//
//			path += ".gz";
//
//		File file = SPIFFS.open(path, "r");
//
//		size_t sent = server.streamFile(file, contentType);
//
//		file.close();
//
//		String message = "File Not Found\n\n";
//		message += "URI: ";
//		message += server.uri();
//		message += "\nMethod: ";
//		message += (server.method() == HTTP_GET) ? "GET" : "POST";
//		message += "\nArguments: ";
//		message += server.args();
//		message += "\n";
//		for (uint8_t i = 0; i < server.args(); i++) {
//			message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//		}
//		server.send(404, "text/plain", message);
//
//		return;
//
//	}
//
//	String message = "File Not Found\n\n";
//
//	message += "URI: ";
//
//	message += server.uri();
//
//	message += "\nMethod: ";
//
//	message += (server.method() == HTTP_GET) ? "GET" : "POST";
//
//	message += "\nArguments: ";
//
//	message += server.args();
//
//	message += "\n";
//
//	for (uint8_t i = 0; i < server.args(); i++) {
//
//		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//
//	}
//
//	server.send(404, "text/plain", message);
//
//}
//// 解析请求的文件
///**
//
// * 根据文件后缀获取html协议的返回内容类型
//
// */
//
//String getContentType(String filename) {
//
//	if (server.hasArg("download")) return "application/octet-stream";
//
//	else if (filename.endsWith(".htm")) return "text/html";
//
//	else if (filename.endsWith(".html")) return "text/html";
//
//	else if (filename.endsWith(".css")) return "text/css";
//
//	else if (filename.endsWith(".js")) return "application/javascript";
//
//	else if (filename.endsWith(".png")) return "image/png";
//
//	else if (filename.endsWith(".gif")) return "image/gif";
//
//	else if (filename.endsWith(".jpg")) return "image/jpeg";
//
//	else if (filename.endsWith(".ico")) return "image/x-icon";
//
//	else if (filename.endsWith(".xml")) return "text/xml";
//
//	else if (filename.endsWith(".pdf")) return "application/x-pdf";
//
//	else if (filename.endsWith(".zip")) return "application/x-zip";
//
//	else if (filename.endsWith(".gz")) return "application/x-gzip";
//
//	return "text/plain";
//
//}