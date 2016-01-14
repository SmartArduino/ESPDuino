#include "ESP8266WiFi.h"

void setup() {
  Serial.begin(115200);
 //设定WiFi为STA模式，如果先前已连接上AP，则与之断开
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");
}

void loop() {
  Serial.println("scan start");
  // WiFi.scanNetworks可以返回扫描到的无线网络数目
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
 /*在串口输出所发现的每一个无线网络的SSID和所接收的信号强度RSSI */
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
//判断WiFi网络是否加密，并输出结果     
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

 //连续扫描的时间间隔
  delay(5000);
}
