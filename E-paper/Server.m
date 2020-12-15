/*
 
 
1 �ϵ��rom��ȡ�ϴ�WIFI��ַ
  �ɹ�-1��ҳ����  2MQTTͨ��
  ʧ��-������ҳģʽ���ȴ��µ�����
 
 
2
  ����ϵ�������wifi, �������ֱͨ�����������ҳ��֤��ֱ��+��ҳ����
*/
 
 
//------�Ƿ�����ӡ-----------------
#define Use_Serial Serial
 
 
//-----------------------------A-1 �����ÿ�ʼ-----------------//
#include <string.h>
#include <math.h>  // ��ѧ���߿�
#include <EEPROM.h>// eeprom  �洢��
 
// WIFI��
#include <ESP8266WiFi.h>
//-------�ͻ���---------
#include <ESP8266HTTPClient.h>
//---------������---------
#include <ESP8266WebServer.h>  
#include <FS.h> 
 
//Mqtt
//#include "Adafruit_MQTT.h"
//#include "Adafruit_MQTT_Client.h"
 
 
//ESP��ȡ����ID
#ifdef ESP8266
extern "C" {
#include "user_interface.h"   //����system_get_chip_id�����Ŀ�
}
#endif
 
//-----------------------------A-1 �����ý���-----------------//
 
 
 
 
//-----------------------------A-2 ����������ʼ-----------------//
int PIN_Led = D4;
bool PIN_Led_State=0;
ESP8266WebServer server ( 80 );  
//����SN��
String SN;
 
/*----------------WIFI�˺ź�����--------------*/
 char ssid[50] = "";    // Enter SSID here
 char password[50] = "";  //Enter Password here
  
#define DEFAULT_STASSID "lovelamp"
#define DEFAULT_STAPSW  "lovelamp"
 
// ���ڴ��ϴε�WIFI������
#define MAGIC_NUMBER 0xAA
struct config_type
{
  char stassid[50];
  char stapsw[50];
  uint8_t magic;
};
config_type config_wifi;
 
//------mqttͷ����-----------------
#define AIO_SERVER      "www.dongvdong.top"  //mqtt������
#define AIO_SERVERPORT  1883  //�˿�
#define AIO_USERNAME    ""
#define AIO_KEY         ""
WiFiClient mqttclient;
 
//------mqttͨ��-----------------
Adafruit_MQTT_Client mqtt(&mqttclient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/test", MQTT_QOS_1);
 
 
 
  
//--------------HTTP����------------------
struct http_request { 
  String  Referer;
  char host[20];
  int httpPort=80;
  String host_ur ;
   
  String usr_name;//�˺�
  String usr_pwd;//����
  
  String postDate;
 
  };
 
String http_html;
//-----------------------------A-2 ������������-----------------//
 
 
//-----------------------------A-3 ����������ʼ-----------------//
void wifi_Init();
void saveConfig();
void loadConfig();
int hdulogin(struct http_request ruqest) ;
void http_wifi_xidian();
String getContentType(String filename);
void handleNotFound();
void handleMain();
void handlePin();
void handleWifi();
void http_jiexi(http_request ruqest);
void handleWifi_wangye();
void handleTest();
void MQTT_connect();
void onoffcallback(char *data, uint16_t len) ;
//---------------------------A-3 ������������------------------------//
 
 
//-----------------------------A-3 ����-----------------//
//3-1�ܽų�ʼ��
void get_espid(){
   SN = (String )system_get_chip_id();
  Use_Serial.println(SN);
   
  }
void LED_Int(){
   
   pinMode(D4, OUTPUT); 
   
  }
//3-2WIFI��ʼ��
void wifi_Init(){
  Use_Serial.println("Connecting to ");
 
    
 
  Use_Serial.println(config_wifi.stassid);
 
  WiFi.begin(config_wifi.stassid, config_wifi.stapsw);
 int retries=3;
  while (WiFi.status() != WL_CONNECTED) {
  server.handleClient();  
  // retries--;
 // if (retries == 0) {
  //      Use_Serial.println("WIFI connect 3 times fail!");
     //   return;
      // }
        
  ESP.wdtFeed();
  delay(500);
   
  Use_Serial.print(".");
  PIN_Led_State = !PIN_Led_State;
  digitalWrite(PIN_Led, PIN_Led_State);
  
  }
 
  Use_Serial.println("--------------WIFI CONNECT!-------------  ");
  Use_Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
  Use_Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
  Use_Serial.println("----------------------------------------  ");
     
  
  }
 
 
 
/*
 *3-/2 ���������EEPROM
*/
void saveConfig()
{
  Serial.println("Save config!");
  Serial.print("stassid:");
  Serial.println(config_wifi.stassid);
  Serial.print("stapsw:");
  Serial.println(config_wifi.stapsw);
  EEPROM.begin(1024);
  uint8_t *p = (uint8_t*)(&config_wifi);
  for (int i = 0; i < sizeof(config_wifi); i++)
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
  EEPROM.begin(1024);
  uint8_t *p = (uint8_t*)(&config_wifi);
  for (int i = 0; i < sizeof(config_wifi); i++)
  {
    *(p + i) = EEPROM.read(i);
  }
  EEPROM.commit();
  //�����Դ�
  if (config_wifi.magic != MAGIC_NUMBER)
  {
    strcpy(config_wifi.stassid, DEFAULT_STASSID);
    strcpy(config_wifi.stapsw, DEFAULT_STAPSW);
    config_wifi.magic = MAGIC_NUMBER;
    saveConfig();
    Serial.println("Restore config!");
  }
  Serial.println(" ");
  Serial.println("-----Read config-----");
  Serial.print("stassid:");
  Serial.println(config_wifi.stassid);
  Serial.print("stapsw:");
  Serial.println(config_wifi.stapsw);
  Serial.println("-------------------");
 
   //  ssid=String(config.stassid);
   //  password=String(config.stapsw);
}
  
   
//3-3ESP8266���������ȵ�
void SET_AP(){
   
  // ��������
  IPAddress softLocal(192,168,4,1);   // 1 ��������WIFI IP��ַ
  IPAddress softGateway(192,168,4,1);
  IPAddress softSubnet(255,255,255,0);
  WiFi.softAPConfig(softLocal, softGateway, softSubnet);
    
  String apName = ("ESP8266_"+(String)ESP.getChipId());  // 2 ����WIFI����
  const char *softAPName = apName.c_str();
    
  WiFi.softAP(softAPName, "admin");      // 3����wifi  ���� +���� adminadmin
 
  Use_Serial.print("softAPName: ");  // 5���WIFI ����
  Use_Serial.println(apName);
 
  IPAddress myIP = WiFi.softAPIP();  // 4���������WIFI IP��ַ
  Use_Serial.print("AP IP address: ");    
  Use_Serial.println(myIP);
   
  }
//3-4ESP������ҳ������
void Server_int(){
   
   server.on ("/", handleMain); // �󶨡�/����ַ��handleMain�������� ----  ������ҳ�� һ������ҳ�� 
  server.on ("/pin", HTTP_GET, handlePin); // �󶨡�/pin����ַ��handlePin��������  ---- ���ص����� 
   server.on ("/wifi", HTTP_GET, handleWifi); // �󶨡�/wifi����ַ��handlePWIFI��������  --- ������������
    server.on ("/wifi_wangye", HTTP_GET, handleWifi_wangye);
    server.on ("/test", HTTP_GET, handleTest);
    server.onNotFound ( handleNotFound ); // NotFound����
   server.begin(); 
    
   Use_Serial.println ( "HTTP server started" );
   
  }
//3-5-1 ��ҳ��������ҳ
void handleMain() {  
 
  /* ������Ϣ���������״̬�룬Content-type�� ���ݣ� 
 
   * �����Ƿ��ʵ�ǰ�豸ipֱ�ӷ���һ��String 
 
   */  
 
  Serial.print("handleMain");  
 
  File file = SPIFFS.open("/index.html", "r");  
 
  size_t sent = server.streamFile(file, "text/html");  
 
  file.close();  
 
  return;  
 
}  
//3-5-2 ��ҳ��������
/* ���Ÿ��Ĵ��� 
 
 * ���ʵ�ַΪhtp://192.162.xxx.xxx/pin?a=XXX��ʱ�����a��ֵ�����ж�Ӧ�Ĵ��� 
 
 */  
 
void handlePin() {  
 
  if(server.hasArg("a")) { // �������Ƿ������a�Ĳ���  
 
    String action = server.arg("a"); // ���a������ֵ  
 
    if(action == "on") { // a=on  
 
      digitalWrite(2, LOW); // ����8266�ϵ���ɫled��led�ǵ͵�ƽ��������Ҫ���Ͳ�����  
 
      server.send ( 200, "text/html", "Pin 2 has turn on"); return; // ��������  
 
    } else if(action == "off") { // a=off  
 
      digitalWrite(2, HIGH); // Ϩ�����led  
 
      server.send ( 200, "text/html", "Pin 2 has turn off"); return;  
 
    }  
 
    server.send ( 200, "text/html", "unknown action"); return;  
 
  }  
 
  server.send ( 200, "text/html", "action no found");  
 
}  
 
//3-5-3 ��ҳ�޸���ͨ��ͥWIFI�����˺�����
/* WIFI���Ĵ��� 
 
 * ���ʵ�ַΪhtp://192.162.xxx.xxx/wifi?config=on&name=Testwifi&pwd=123456
  ����wifi���� WIFI���ݴ�����
  ����config��ֵ������ on
  ����name��ֵ������  wifi���ִ���
  ����pwd��ֵ������   wifi���봫��
 
 
 */  
 
 
void handleWifi(){
   
   
   if(server.hasArg("config")) { // �������Ƿ������a�Ĳ���  
 
    String config = server.arg("config"); // ���a������ֵ  
        String wifiname;
        String wifipwd;
 
      
         
    if(config == "on") { // a=on  
          if(server.hasArg("name")) { // �������Ƿ������a�Ĳ���  
        wifiname = server.arg("name"); // ���a������ֵ
 
          }
           
    if(server.hasArg("pwd")) { // �������Ƿ������a�Ĳ���  
         wifipwd = server.arg("pwd"); // ���a������ֵ   
           }
                   
          String backtxt= "Wifiname: "+ wifiname  +"/r/n wifipwd: "+ wifipwd ;// ���ڴ��ں���ҳ������Ϣ
           
          Use_Serial.println ( backtxt); // ���ڴ�ӡ������
           
          server.send ( 200, "text/html", backtxt); // ��ҳ���ظ��ֻ���ʾ
           // wifi���ӿ�ʼ
 
         wifiname.toCharArray(config_wifi.stassid, 50);    // ����ҳ�õ��� WIFI��
         wifipwd.toCharArray(config_wifi.stapsw, 50);  //����ҳ�õ��� WIFI����              
          
         saveConfig();
         wifi_Init();
  
               
      
        
          return;         
            
 
    } else if(config == "off") { // a=off  
                server.send ( 200, "text/html", "config  is off!");
        return;
 
    }  
 
    server.send ( 200, "text/html", "unknown action"); return;  
 
  }  
 
  server.send ( 200, "text/html", "action no found");  
   
  }
 
 
//3-5-4  ��ҳ��֤��������-- ����
void http_wifi_xidian(){
 
////  ͨ��������֤�����Լ��޸�postDate�е�ѧ�ź�����
 // ���磺
  /*��ҳ��֤����post*/
//  String  Referer="http://10.255.44.33/srun_portal_pc.php?ac_id=1&";
//  char* host = "10.255.44.33";
//  int httpPort = 80;
//  String host_ur = "srun_portal_pc.php";
 
/*��ҳ��֤����ģʽ */
// String usr_name;//�˺�
// String usr_pwd;//����
//  String postDate = String("")+"action=login&ac_id=1&user_ip=&nas_ip=&user_mac=&url=&username=+"+usr_name+"&password="+usr_pwd;
   
  // ��֤����
  http_request ruqest;
  ruqest.Referer="http://10.255.44.33/srun_portal_pc.php?ac_id=1&";
  String("10.255.44.33").toCharArray( ruqest.host , 20);
  ruqest.httpPort = 80;
  ruqest.host_ur = "srun_portal_pc.php";
  ruqest.usr_name="1601120383";//�˺�  �������޸� Ҫ+1
  ruqest.usr_pwd="mimaHENFuza";//����  �������޸� Ҫ-1
  ruqest.postDate = String("")+"action=login&ac_id=1&user_ip=&nas_ip=&user_mac=&url=&username=+"+ ruqest.usr_name+"&password="+ruqest.usr_pwd;
      
 
   if (hdulogin(ruqest) == 0) {
     
      Use_Serial.println("WEB Login Success!");
    }
    else {
       
      Use_Serial.println("WEB Login Fail!");
    }
   
  }
 
 
//3-5-5-1 ��ҳ��֤����
void handleWifi_wangye(){
  if(server.hasArg("config")) { // �������Ƿ������a�Ĳ���  
// 1 ���������Ƿ�����
    String config = server.arg("config"); // ���a������ֵ  
        String wifi_wname,wifi_wpwd,wifi_wip,wifi_postdata;
           
  if(config == "on") { // a=on  
        if(server.hasArg("wifi_wname")) { // �������Ƿ������a�Ĳ���  
        wifi_wname = server.arg("wifi_wname"); // ���a������ֵ
           if(wifi_wname.length()==0){           
             server.send ( 200, "text/html", "please input WIFI����!"); // ��ҳ���ظ��ֻ���ʾ
            return;}
          }
        if(server.hasArg("wifi_wpwd")) { // �������Ƿ������a�Ĳ���  
        wifi_wpwd = server.arg("wifi_wpwd"); // ���a������ֵ
                
          }
     if(server.hasArg("wifi_wip")) { // �������Ƿ������a�Ĳ���  
         wifi_wip = server.arg("wifi_wip"); // ���a������ֵ   
          if(wifi_wip.length()==0){           
             server.send ( 200, "text/html", "please input ��½��ַ!"); // ��ҳ���ظ��ֻ���ʾ
            return;}
 
           }
        if(server.hasArg("wifi_postdata")) { // �������Ƿ������a�Ĳ���  
 
            String message=server.arg(3);
            for (uint8_t i=4; i<server.args(); i++){
               message += "&" +server.argName(i) + "=" + server.arg(i) ;
             }
   
         wifi_postdata = message; // ���a������ֵ
            if(wifi_postdata.length()==0){           
             server.send ( 200, "text/html", "please input ������Ϣ!"); // ��ҳ���ظ��ֻ���ʾ
            return;}   
           }
        
//--------------------------------- wifi����-------------------------------------------------
         wifi_wname.toCharArray(config_wifi.stassid, 50);    // ����ҳ�õ��� WIFI��
         wifi_wpwd.toCharArray(config_wifi.stapsw, 50);  //����ҳ�õ��� WIFI����              
          
         saveConfig();
       
         // wifi���ӿ�ʼ
         wifi_Init();
  
  //--------------------------------- ��֤����-------------------------------------------------     
 
 
   
// 2 ������֤
                 
// ruqest.Referer="http://10.255.44.33/srun_portal_pc.php";
  http_request ruqest_http;
  ruqest_http.httpPort = 80;
  ruqest_http.Referer=wifi_wip;// ��¼��ַ
  ruqest_http.postDate=wifi_postdata;
    
   Use_Serial.println (wifi_wip);
   Use_Serial.println (wifi_postdata);
   
 
     
  http_jiexi(ruqest_http);
           
           return; 
  } else if(config == "off") { // a=off  
                server.send ( 200, "text/html", "config  is off!");
        return;
  }  
    server.send ( 200, "text/html", "unknown action"); return;  
  }  
      server.send ( 200, "text/html", "action no found");  
 
  }
 
//3-5-5-2 ��ҳ����IP��Ϣ
void http_jiexi(http_request ruqest){
   
    int datStart ; int datEnd;
 
    // 1 ����ip
   //   http://10.255.44.33/
    char h[]="http://";  char l[]="/";
    datStart  =  ruqest.Referer.indexOf(h)+ strlen(h);
    datEnd= ruqest.Referer.indexOf(l,datStart);
    String host=ruqest.Referer.substring(datStart, datEnd);
    host.toCharArray( ruqest.host , 20);
     
    Use_Serial.println ( ruqest.host); // ���ڴ�ӡ������
 
    // 2 �����������ҳ�ļ�-��¼ҳ��
   //   srun_portal_pc.php?ac_id&";
    char s[] ="?";
    datStart  = ruqest.Referer.indexOf(l,datStart)+strlen(l);
   // datEnd = ruqest.Referer.indexOf(s,datStart)-strlen(s)+1;
   if(ruqest.Referer.indexOf(s,datStart)==-1)
    {datEnd = ruqest.Referer.length();}
    else{   
       datEnd = ruqest.Referer.indexOf(s,datStart)-1;
      }
    ruqest.host_ur  = String(ruqest.Referer.substring(datStart, datEnd));
    Use_Serial.println ( ruqest.host_ur); // ���ڴ�ӡ������
 
  
    Use_Serial.println( ruqest.postDate);
      
   if (hdulogin(ruqest) == 0) {  
      Use_Serial.println("WEB Login Success!");
    }
    else { 
      Use_Serial.println("WEB Login Fail!");
    }   
          return; 
   
  }
 
//3-5-5-3 ��ҳ��֤����HTTP����
/*---------------------------------------------------------------*/
int hdulogin(struct http_request ruqest) {
  WiFiClient client;
 
  if (!client.connect(ruqest.host, ruqest.httpPort)) {
    Use_Serial.println("connection failed");
    return 1;
  }
  delay(10);
  
  if (ruqest.postDate.length() && ruqest.postDate != "0") {
    String data = (String)ruqest.postDate;
    int length = data.length();
 
    String postRequest =
                         (String)("POST ") + "/"+ruqest.host_ur+" HTTP/1.1\r\n" +
                         "Host: " +ruqest.host + "\r\n" +
                         "Connection: Keep Alive\r\n" +
                         "Content-Length: " + length + "\r\n" +
                         "Accept:text/html,application/xhtml+xml,application/xml;*/*\r\n" +
                         "Origin: http://"+ruqest.host+"\r\n" +
                          "Upgrade-Insecure-Requests: 1"+"\r\n" +
                         "Content-Type: application/x-www-form-urlencoded;" + "\r\n" +
                         "User-Agent: zyzandESP8266\r\n" +
                          "Accept-Encoding: gzip, deflate"+"\r\n" +
                          "Accept-Language: zh-CN,zh;q=0.9"+"\r\n" +                    
                         "\r\n" +
                         data + "\r\n";
//  String postDate = String("")+"action=login&ac_id=1&user_ip=&nas_ip=&user_mac=&url=&username=+"+usr_name+"&password="+usr_pwd;
    client.print(postRequest);
    delay(600);
    //��������Ϣ
    String line = client.readStringUntil('\n');
    Use_Serial.println(line);
    while (client.available() > 0) {
    Use_Serial.println(client.readStringUntil('\n'));
    line += client.readStringUntil('\n');
     
   // line += client.readStringUntil('\n');
    }
 
    http_html=line;
     
    client.stop();
     
    if (line.indexOf("ʱ��") != -1 || line.indexOf("��½") != -1) { //��֤�ɹ�
      return 0;
     
    }
    else {
      return 2;
    }
 
  }
  client.stop();
  return 2;
}
 
//3-5-6 ��ҳ����
void handleTest(){
     server.send(200, "text/html", http_html);
  }
 
//3-5-������ ��ҳû�ж�Ӧ������δ���
void handleNotFound() {  
 
  String path = server.uri();  
 
  Serial.print("load url:");  
 
  Serial.println(path);  
 
  String contentType = getContentType(path);  
 
  String pathWithGz = path + ".gz";  
 
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){  
 
    if(SPIFFS.exists(pathWithGz))  
 
      path += ".gz";  
 
    File file = SPIFFS.open(path, "r");  
 
    size_t sent = server.streamFile(file, contentType);  
 
    file.close(); 
 
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message); 
 
    return;  
 
  }  
 
  String message = "File Not Found\n\n";  
 
  message += "URI: ";  
 
  message += server.uri();  
 
  message += "\nMethod: ";  
 
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";  
 
  message += "\nArguments: ";  
 
  message += server.args();  
 
  message += "\n";  
 
  for ( uint8_t i = 0; i < server.args(); i++ ) {  
 
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";  
 
  }  
 
  server.send ( 404, "text/plain", message );  
 
}  
// ����������ļ�
/** 
 
 * �����ļ���׺��ȡhtmlЭ��ķ����������� 
 
 */  
 
String getContentType(String filename){  
 
  if(server.hasArg("download")) return "application/octet-stream";  
 
  else if(filename.endsWith(".htm")) return "text/html";  
 
  else if(filename.endsWith(".html")) return "text/html";  
 
  else if(filename.endsWith(".css")) return "text/css";  
 
  else if(filename.endsWith(".js")) return "application/javascript";  
 
  else if(filename.endsWith(".png")) return "image/png";  
 
  else if(filename.endsWith(".gif")) return "image/gif";  
 
  else if(filename.endsWith(".jpg")) return "image/jpeg";  
 
  else if(filename.endsWith(".ico")) return "image/x-icon";  
 
  else if(filename.endsWith(".xml")) return "text/xml";  
 
  else if(filename.endsWith(".pdf")) return "application/x-pdf";  
 
  else if(filename.endsWith(".zip")) return "application/x-zip";  
 
  else if(filename.endsWith(".gz")) return "application/x-gzip";  
 
  return "text/plain";  
 
}  
 
 
//3-6-1 MQTT��ʼ��  -----------------------------------
 
void Mqtt_int(){
   //MQTT�󶨳�ʼ��
    onoffbutton.setCallback(onoffcallback);
    mqtt.subscribe(&onoffbutton);
  }
 
 
//------mqtt�ص�����-----------------
void onoffcallback(char *data, uint16_t len) {
  Serial.print("Hey we're in a onoff callback, the button value is: ");
  Serial.println(data);
}
 
//3-6-2------mqtt���Ӽ��-----------------
void MQTT_reconnect() {
  int8_t ret;
 
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Use_Serial.print("Connecting to MQTT... ");
 
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
     
       server.handleClient();  
        
       Use_Serial.println(mqtt.connectErrorString(ret));
       Use_Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
      retries--;
     if (retries == 0) {
         // basically die and wait for WDT to reset me
         //while (1);
         Use_Serial.println("Retrying MQTT connection 3 fali...");
         return;
      }
  }
  Use_Serial.println("MQTT Connected!");
 
    
}
 
////3-6-3 MQTT������
void Mqtt_Domain(){
   if(WiFi.status() == WL_CONNECTED){
     MQTT_reconnect();
   mqtt.processPackets(2000);
  if(! mqtt.ping()) {
       mqtt.disconnect();
  }
     }
  else {
     wifi_Init();
  
   }
  }
 
//------------------------------------------- void setup() ------------------------------------------
 
void setup() {
    Use_Serial.begin(115200); 
    get_espid();
    LED_Int(); 
      
    loadConfig();// ��ȡ��Ϣ WIFI
    wifi_Init(); // 3�γ���
    
    SET_AP(); // ����WIFI
    SPIFFS.begin();
    Server_int();
    Mqtt_int();
}
 
 
//------------------------------------------- void void loop()  ------------------------------------------
 
void loop()
{
 
   server.handleClient();  
   Mqtt_Domain();
  
 
}
