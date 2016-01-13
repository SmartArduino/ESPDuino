/*  WiFiTelnetToSerial - 使用通用异步收发传输器（Universal Asynchronous 
Receiver/Transmitter，UART）与ESPDUINO进行TCP透传的例子 */
#include <ESP8266WiFi.h>

//定义可连接的客户端数目最大值
#define MAX_SRV_CLIENTS 1
const char* ssid = "Doit";
const char* password = "doit3305";

WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){ //超时（20x500=10000,10秒钟），提示连接失败
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }
  //启动UART传输和服务器
  server.begin();
  server.setNoDelay(true);
  
  Serial.print("Ready! Use 'telnet ");
  Serial.print(WiFi.localIP());//获得服务器本地IP
  Serial.println(" 23' to connect");
}

void loop() {
  uint8_t i;
  //检测服务器端是否有活动的客户端连接
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //查找空闲或者断开连接的客户端，并置为可用
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.println(i);
        continue;
      }
    }
    //若没有可用客户端，则停止连接
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //检查客户端的数据
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        //从Telnet客户端获取数据，并推送到URAT端口
        while(serverClients[i].available()) Serial.write(serverClients[i].read());
      }
    }
  }
  //检查UART端口数据
  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //将UART端口数据推送到所有已连接的telnet客户端，实现双向通信
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }
}
