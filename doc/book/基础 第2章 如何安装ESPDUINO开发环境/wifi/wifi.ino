#include <ESP8266WiFi.h> //这是支持Wifi基本功能的头文件
#include <time.h> //时间处理功能的头文件

const char* ssid = "Doit";  //所接入的无线网络名称
const char* password = "doit3305";  
//访问密码，这两个信息在后面章节会频繁用到

int timezone = 3;
int dst = 0;

//ESPDUINO开发版的初始函数
void setup() {
  Serial.begin(115200); //设定串口速率为115200B/s
  Serial.setDebugOutput(true); //将串口设为调试输出模式

  WiFi.mode(WIFI_STA);  //将此Wifi芯片设为站点模式
  WiFi.begin(ssid, password); //接入到无线环境热点
  Serial.println("\nConnecting to WiFi");
 
 //如果Wifi状态不是WL_CONNECTED，则表示连接失败
  while (WiFi.status() != WL_CONNECTED) {  
    Serial.print("."); //在串口窗口输出.，表示连接进度信息
    delay(1000); //延时1秒
  }

  //设置时间格式以及时间服务器的url网址
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  //串口信息输出，此处不建议用中文，因为寻址方式的原因，会出乱码
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

//进入事件主循环
void loop() {
  time_t now = time(nullptr); //获取当前时间
  Serial.println(ctime(&now));//打印并换行
  delay(1000); //延迟一秒（间隔）
}
