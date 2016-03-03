/*
  0， 上电：Doit微信开发板上电，前5秒等待配置阶段LED灯0.1s亮，0.1s灭; 5秒后1秒亮1秒灭尝试连接到sta
  1,  smartconfig配置
  前5s内，LED灯快闪（0.1s亮，0.1s灭），此时短按Flash或者key键进入smartconfig配置模式，led灯0.3秒亮0.3秒灭
  在这种情况下可使用微信“四博智联研发部”公众号，wifi配置功能可实现wifi配置上网
  或者使用ESP-touch app软件进行配置。wifi配置功能可实现wifi配置上网。
  如果超过3分钟没有配置成功，模块重启
  2，  Webserver配置：Doit微信开发板上电5s后发出ap信号，启动httpserver
  在httpserver启动后，开发板发出wifi，ssid为：“Doit_ESP_Car_xxxxxx”，其中xxxxxx为芯片mac地址后六位
  使用笔记本或者手机连接该ssid，密码为空，然后使用浏览器设置，地址为192.168.4.1
  3,  Doit微信开发板上LED灯常亮表示开发板已经连接上无线路由器
  4，OTA升级固件
  使用webserver可以对开发板的固件进行OTA升级。地址为：http;//xxx.xxx.xxx.xxx:8080，或者在web配置页面连接过去。
  5，恢复出厂设置
  启动后，长按Flash或者Key键超过3秒钟，恢复出厂设置
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Ticker.h>
#include "project.h"

#define MAGIC_NUMBER 0xAA

/*
 * 保存参数到EEPROM
*/
void saveConfig()
{
  Serial.println("Save config!");
  Serial.print("stassid:");
  Serial.println(config.stassid);
  Serial.print("stapsw:");
  Serial.println(config.stapsw);
  Serial.print("id:");
  Serial.println(config.id);
  Serial.print("key:");
  Serial.println(config.key);

  EEPROM.begin(1024);
  uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
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
  EEPROM.begin(1024);
  uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
  {
    *(p + i) = EEPROM.read(i);
  }
  EEPROM.commit();
  if (config.magic != MAGIC_NUMBER)
  {
    strcpy(config.stassid, DEFAULT_STASSID);
    strcpy(config.stapsw, DEFAULT_STAPSW);
    strcpy(config.id, DEFAULT_ID);
    strcpy(config.key, DEFAULT_KEY);
    config.magic = MAGIC_NUMBER;
    saveConfig();
   // system_restore();
    Serial.println("Restore config!");
  }
  Serial.println("-----Read config-----");
  Serial.print("stassid:");
  Serial.println(config.stassid);
  Serial.print("stapsw:");
  Serial.println(config.stapsw);
  Serial.print("id:");
  Serial.println(config.id);
  Serial.print("key:");
  Serial.println(config.key);
  Serial.println("-------------------");
}

/*
 * 开启smartconfig，等待配置完成，若超时，延时重启
*/
void doSmartconfig()
{
  Serial.println("[doSmartconfig]");
  WiFi.mode(WIFI_STA);
  WiFi.stopSmartConfig();
  WiFi.beginSmartConfig();
  int cnt = 0;
  bool flag_ok = false;
  while (true) {
    ESP.wdtFeed();
    delay(300);
    ledState = !ledState; digitalWrite(PINLED, ledState);
    if (flag_ok == true) continue;
    if (WiFi.smartConfigDone()) {
      Serial.println("SmartConfig Success");
      /*Serial.println("SSID: "+WiFi.SSID());
        Serial.println("PSW: "+WiFi.psk());*/
      strcpy(config.stassid, WiFi.SSID().c_str());
      strcpy(config.stapsw, WiFi.psk().c_str());
      Serial.printf("SSID:%s\r\n", config.stassid);
      Serial.printf("PSW:%s\r\n", config.stapsw);
      saveConfig();
      delayRestart(10);
      flag_ok = true;
    }
    cnt++;
    if (cnt >= 600) {
      Serial.println("Time out, restart!");
      delayRestart(1);
    }
  }
}
/* 
 * 恢复出厂设置
*/
void restoreFactory()
{
  Serial.println("[restoreFactory]");
  config.magic = 0x00;
  saveConfig();
  delayRestart(1);
  while (1) {
    ESP.wdtFeed();
    delay(100);
  }
}

/*
 * 启动前5秒检查用户是否按键，如果有按键判断是短按或者长按，分别进入smartconfig模式或者恢复出厂
*/
void waitKey()
{
  pinMode (PINLED, OUTPUT);
  pinMode (PINKEY, INPUT);
  digitalWrite(PINLED, 0);
  Serial.println("Short press key: smartconfig mode\r\nlong press key:factory restore mode");
  char keyCnt = 0;
  unsigned long preTick = millis();
  unsigned long preTick2 = millis();
  int num = 0;
  while (1)
  {
    ESP.wdtFeed();
    if (millis() - preTick < 10 ) continue;
    preTick = millis();
    num++;
    if (num % 10 == 0)
    {
      ledState = !ledState;
      digitalWrite(PINLED, ledState);
      Serial.print(".");
    }
    if (keyCnt >= 200)
    { //长按
      keyCnt = 0;
      Serial.println("\r\nLong Press key");
      restoreFactory();
    }
    else if (keyCnt >= 5 && digitalRead(PINKEY) == 1)
    { //短按
      keyCnt = 0;
      Serial.println("\r\nShort Press key");
      doSmartconfig();
    }
    if (digitalRead(PINKEY) == 0) keyCnt++;
    else
      keyCnt = 0;

    if (millis() - preTick2 > 5 * 1000) break;
  }
  digitalWrite(PINLED, 0);
  pinMode (PINKEY, OUTPUT);
}

/*
*初始化WiFi
*/
void initWiFi()
{
  char temp[32];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  sprintf(temp, "%s_%02X%02X%02X", HOST_NAME, mac[3], mac[4], mac[5]);
  Serial.printf("\r\nMAC address:%02X-%02X-%02X-%02X-%02X-%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  WiFi.hostname(temp);

  sprintf(temp, "%s_%02X%02X%02X", apssid, mac[3], mac[4], mac[5]);

  if (strcmp(config.stassid, DEFAULT_STASSID) != 0)
  {
    WiFi.mode(WIFI_AP_STA);
    WiFi.disconnect();
    delay(100);
    WiFi.begin(config.stassid, config.stapsw);
  }
  else {
    Serial.println("\r\nNo sta is set");
    WiFi.disconnect();
    delay(100);
    WiFi.mode(WIFI_AP);
  }
  WiFi.softAP(temp);
}

/*
*WiFiTick
*检查是否需要初始化WiFi
*检查WiFi是否连接上，若连接成功启动TCP Client
*控制指示灯
*/
void wifiTick()
{
  static bool wifiInitFlag=false;  
  static bool taskStarted = false;
  static bool ledTurnon = false;
  if(!wifiInitFlag){
    wifiInitFlag = true;
    initWiFi();
    initHttpServer();
    startUDPServer(udpPort);
    printHelp();
    Serial.printf("Heap size:%d\r\n",ESP.getFreeHeap());
  }
  
  if ( WiFi.status() != WL_CONNECTED ) {
    if (millis() - lastWiFiCheckTick > 1000) {
      lastWiFiCheckTick = millis();
      ledState = !ledState; digitalWrite(PINLED, ledState);
      ledTurnon = false;
    }
  }
  else {
    if (ledTurnon == false) {
      ledTurnon = true;
      digitalWrite(PINLED, 0);
    }
    if (taskStarted == false) {
      taskStarted = true;
      Serial.print("\r\nGet IP Address: ");
      Serial.println(WiFi.localIP());
      startTCPClient();
    }
  }
}
/*
*检查串口是否有数据
*/
void doUartTick()
{
  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    parseUartPackage((char*)sbuf);
  }
}
/*
*初始化
*/
void setup() {
  Serial.begin (115200);
  Serial.println("\r\n\r\nStart ESP8266");
  Serial.print("Firmware Version:");
  Serial.println(VER);
  Serial.print("SDK Version:");
  Serial.println(ESP.getSdkVersion());
  wifi_station_set_auto_connect(0);//关闭自动连接  
  ESP.wdtEnable(5000);
  loadConfig();
  initParseData();
  waitKey();
}
/*
*主循环
*/
void loop() {
  ESP.wdtFeed();
  wifiTick();
  doTCPClientTick();
  doUdpServerTick();
  doHttpServerTick();
  doUartTick();
}
