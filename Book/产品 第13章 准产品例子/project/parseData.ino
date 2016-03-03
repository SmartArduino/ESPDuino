
#include "project.h"

#define LEDR 5
#define LEDB 4
#define LEDG 2

/*
 * 初始化
*/
void initParseData()
{//初始化在这里
  Serial.println("[initParseData]");
}
/*
 * 处理接收到的udp数据
*/
void parseUDPPackage(char *p)
{
  Serial.print("[UDP parseData:]");
  Serial.println(p);
  //ping命令
  if(strstr(p,"cmd=ping") !=NULL){
    IPAddress ip = WiFi.localIP();
    char t[128];
    sprintf(t,"cmd=pong&sta_ip=%d.%d.%d.%d&host_ip=192.168.4.1",ip[0],ip[1],ip[2],ip[3]);
    if(ip[0]==0&&ip[1]==0&&ip[2]==0&&ip[3]==0)
    strcpy(t,"cmd=pong&sta_ip=&host_ip=192.168.4.1");
    sendUDP(t);
  }
  //设置ssid和密码 cmd=config&ssid=Doit&ps=doit3305
  else if(strstr(p,"cmd=config") !=NULL){
    char *s = strstr(p,"ssid=");if(s==NULL) return;
    s = strstr(s,"=");if(s==NULL) return;
    char *e = strstr(s,"&");if(e==NULL) return;
    char ssid[64];memset(ssid,0x00,64);
    memcpy(ssid,s+1,e-s-1);
    s=strstr(e,"=");if(s==NULL) return;
    e=strstr(s,"&");//if(e==NULL) return;
    char psw[64];memset(psw,0x00,64);
    if(e!=NULL) memcpy(psw,s+1,e-s-1);
    else strcpy(psw,s+1);
    Serial.print("SSID: "); Serial.println(ssid);
    Serial.print("PSW: ");  Serial.println(psw);
    Serial.println("Module will restart after 2s...");
    strcpy(config.stassid,ssid);
    strcpy(config.stapsw,psw);
    sendUDP((char*)"CMD OK");
    saveConfig();
    delay(2000);
    ESP.restart();
  }
}
/*
 * 设备注册
*/
void device_regist(void)
{
  char temp[64];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  sprintf(temp,"#0#%s_%02X%02X%02X#%s\r\n",config.id,mac[3], mac[4], mac[5],config.key);
  sendTCP(temp);
}
/*
 * 设备登陆
*/
void device_login(void)
{
  char temp[64];
  uint8_t mac[6];
  WiFi.macAddress(mac);  
  sprintf(temp,"#1#%s_%02X%02X%02X#%s\r\n",config.id,mac[3], mac[4], mac[5],config.key);
  sendTCP(temp);
}
/*
 * 设备注销
*/
void device_logout(void)
{
  char temp[64];
  sprintf(temp,"#3#\r\n");
  sendTCP(temp);
}
/*
 * 数据转发
*/
void device_dataTrans(void)
{//send mstick to server, then server send it back
  char temp[64];
  uint8_t mac[6];
  WiFi.macAddress(mac);  
  sprintf(temp,"#2#%s_%02X%02X%02X#%d\r\n",config.id,mac[3], mac[4], mac[5],millis());
  sendTCP(temp);
}
/*
 * 获取服务器时间
*/
void device_getTime(void)
{
  char temp[64];
  sprintf(temp,"#4#\r\n");
  sendTCP(temp);
}
/*
 * 保持心跳
*/
void device_keep(void)
{
  char temp[64];
  sprintf(temp,"#5#\r\n");
  sendTCP(temp);
}
/*
 * 打印帮助信息
*/
void printHelp(void)
{
  Serial.println("\r\n----------Doit Product demo----------");
  Serial.println(" 'h' : print this help");
  Serial.printf(" 'r' : regist product.name:%s_xxxxxx,pass:%s,response:#0#1:regist ok; #0#0:exist yet.\r\n",config.id,config.key);
  Serial.printf(" 'l' : login.name:%s_xxxxxx,pass:%s;Response:#1#1:ok; #1#0:Password Error;#1#2:usernmae not exist.\r\n",config.id,config.key);
  Serial.println(" 'e' : logout.");
  Serial.println(" 'd' : send data.send ESPduino timetick to server");
  Serial.println(" 't' : get server time.");
  Serial.println(" 'k' : send a keepalive package.");
  Serial.println(" Visit:http://book.doit.am/online.php to check");
  Serial.println("-------------------------------------");
}
/*
 * 处理tcp信息
*/
void parseTCPPackage(char *p)
{
  Serial.print("[TCP parseData]");
  Serial.println(p);
}
/*
 * 处理串口收到的数据
*/
void parseUartPackage(char *p)
{
  Serial.println("[Uart parseData]");
  switch(p[0]){
    case 'h': Serial.println(p[0]);printHelp();break;
    case 'r': Serial.println(p[0]);device_regist();break;
    case 'l': Serial.println(p[0]);device_login();break;
    case 'e': Serial.println(p[0]);device_logout();break;
    case 'd': Serial.println(p[0]);device_dataTrans();break;
    case 't': Serial.println(p[0]);device_getTime();break;
    case 'k': Serial.println(p[0]);device_keep();break;
    default: Serial.println(p);Serial.println("Invalid cmd");break;
  }
}

