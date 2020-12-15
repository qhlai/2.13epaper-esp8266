#include "WiFiConfig.h"
ESP8266WebServer server(80);
//ESP8266WiFiMulti WiFiMulti;
WiFiSetting wifi_config;
/*----------------WIFI�˺ź�����--------------*/

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
	loadConfig();// ��ȡ��Ϣ WIFI

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
	            // NodeMCU��ͨ�����ڼ����������
	Serial.print(WiFi.SSID());   
	Serial.println(" Connected");           // ���ӵ�WiFI����
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
 *3-/2 ���������EEPROM
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
 * ��EEPROM���ز���
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
	//�����Դ�
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
// 3 - 3ESP8266���������ȵ�
void SET_AP() {

	// ��������
	IPAddress softLocal(192, 168, 1, 0);   // 1 ��������WIFI IP��ַ
	IPAddress softGateway(192, 168, 1, 0);
	IPAddress softSubnet(255, 255, 255, 0);
	WiFi.softAPConfig(softLocal, softGateway, softSubnet);

	String apName = ("ESP_" + (String)ESP.getChipId());  // 2 ����WIFI����
	const char *softAPName = apName.c_str();

	WiFi.softAP(softAPName, "esp12345");      // 3����wifi  ���� +���� adminadmin

	Serial.print("softAPName: ");  // 5���WIFI ����
	Serial.println(apName);

	IPAddress myIP = WiFi.softAPIP();  // 4���������WIFI IP��ַ
	Serial.print("AP IP address: ");
	Serial.println(myIP);

}
//3-4ESP������ҳ������
void Server_int() {

	server.on("/", handleMain); // �󶨡�/����ַ��handleMain�������� ----  ������ҳ�� һ������ҳ�� 
	//server.on("/pin", HTTP_GET, handlePin); // �󶨡�/pin����ַ��handlePin��������  ---- ���ص����� 
	server.on("/wifi", HTTP_GET, handleWifi); // �󶨡�/wifi����ַ��handlePWIFI��������  --- ������������
	//server.on("/wifi_wangye", HTTP_GET, handleWifi_wangye);
	//server.on("/test", HTTP_GET, handleTest);
	//server.onNotFound(handleNotFound); // NotFound����
	server.begin();

	Serial.println("HTTP server started");

}
//3-5-1 ��ҳ��������ҳ
void handleMain() {

	/* ������Ϣ���������״̬�룬Content-type�� ���ݣ�

	 * �����Ƿ��ʵ�ǰ�豸ipֱ�ӷ���һ��String

	 */
	Serial.println("handleMain");
	File file = SPIFFS.open("/index.html", "r");
	size_t sent = server.streamFile(file, "text/html");
	file.close();

	return;

}
////3-5-2 ��ҳ��������
///* ���Ÿ��Ĵ���
//
// * ���ʵ�ַΪhtp://192.162.xxx.xxx/pin?a=XXX��ʱ�����a��ֵ�����ж�Ӧ�Ĵ���
//
// */
//
//void handlePin() {
//
//	if (server.hasArg("a")) { // �������Ƿ������a�Ĳ���  
//
//		String action = server.arg("a"); // ���a������ֵ  
//
//		if (action == "on") { // a=on  
//
//			digitalWrite(2, LOW); // ����8266�ϵ���ɫled��led�ǵ͵�ƽ��������Ҫ���Ͳ�����  
//
//			server.send(200, "text/html", "Pin 2 has turn on"); return; // ��������  
//
//		}
//		else if (action == "off") { // a=off  
//
//			digitalWrite(2, HIGH); // Ϩ�����led  
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
////3-5-3 ��ҳ�޸���ͨ��ͥWIFI�����˺�����
///* WIFI���Ĵ���
//
// * ���ʵ�ַΪhtp://192.162.xxx.xxx/wifi?config=on&name=Testwifi&pwd=123456
//  ����wifi���� WIFI���ݴ�����
//  ����config��ֵ������ on
//  ����name��ֵ������  wifi���ִ���
//  ����pwd��ֵ������   wifi���봫��
//
//
// */
//
//
void handleWifi() {

	Serial.println("handleWifi");
	if (server.hasArg("config")) { // �������Ƿ������a�Ĳ���  

		String config = server.arg("config"); // ���a������ֵ  
		String wifiname;
		String wifipwd;

		if (config == "on") { // a=on  
			if (server.hasArg("name")) { // �������Ƿ������a�Ĳ���  
				wifiname = server.arg("name"); // ���a������ֵ
			}

			if (server.hasArg("pwd")) { // �������Ƿ������a�Ĳ���  
				wifipwd = server.arg("pwd"); // ���a������ֵ   
			}

			String backtxt = "Wifiname: " + wifiname + "/r/n wifipwd: " + wifipwd;// ���ڴ��ں���ҳ������Ϣ

			Serial.println(backtxt); // ���ڴ�ӡ������

			server.send(200, "text/html", backtxt); // ��ҳ���ظ��ֻ���ʾ
			 // wifi���ӿ�ʼ

			wifiname.toCharArray(wifi_config.stassid, 50);    // ����ҳ�õ��� WIFI��
			wifipwd.toCharArray(wifi_config.stapsw, 50);  //����ҳ�õ��� WIFI����              

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
////3-5-������ ��ҳû�ж�Ӧ������δ���
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
//// ����������ļ�
///**
//
// * �����ļ���׺��ȡhtmlЭ��ķ�����������
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