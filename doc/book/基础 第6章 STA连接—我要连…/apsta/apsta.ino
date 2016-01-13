#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
WiFiUDP udp;
IPAddress sip(192, 168, 1, 29);//本地IP
IPAddress sip1(192, 168, 1, 1);//本地网关
IPAddress sip2(255, 255, 255, 0);//本地子网掩码
IPAddress xip(192, 168,2, 2);//下位远程IP
IPAddress lxip(192, 168,2, 1);//AP端IP
IPAddress lxip1(192, 168,2, 1);//AP端网关
IPAddress lxip2(255, 255,255, 0);//AP端子网掩码
IPAddress Serverip(192, 168, 1, 4);//上位机远程IP
unsigned int localPort = 9999;//本地端口
unsigned int remoteport = 9999;//远程端口
const char *ssid = "Netcore_wsn";
const char *password = "99325408322";
const char *ssid1 = "Netcore_wsn1";//AP
const char *password1 = "99325408322";//AP
char packetBuffer[255];//收发缓冲区
void setup() {Serial.begin(115200);//初始化串口波特率
delay(5000);//延时5S
WiFi.mode(WIFI_AP_STA);//设置模式为AP+STA
WiFi.softAPConfig(lxip,lxip1,lxip2);//设置AP网络参数
WiFi.softAP(ssid1,password1,1);//设置AP账号密码
WiFi.begin(ssid,password);//连接指定路由
WiFi.config(sip,sip1,sip2);//设置本地网络参数
Serial.print("Is connection routing, please wait");
while(WiFi.status()!=WL_CONNECTED)//等待路由连接
{

delay(500);
Serial.print(".");
}
Serial.println(" ");
udp.begin(localPort);//监听指定端口
Serial.print("ip:");
Serial.println(WiFi.localIP());
Serial.print("apip:");
Serial.println(WiFi.softAPIP());
}
void loop()
{
if(udp.parsePacket())
{
udp.read(packetBuffer,255);//读取数据
udp.beginPacket(Serverip,remoteport);
udp.write(packetBuffer,255);
udp.endPacket();
Serial.println(packetBuffer);
udp.beginPacket(xip,remoteport);
udp.write(packetBuffer,255);
udp.endPacket();
memset(packetBuffer, 0, 255);//清除缓冲器数值
}
}
