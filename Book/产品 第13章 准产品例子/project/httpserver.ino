#include "project.h"
#include "webPage.h"

ESP8266WebServer httpServer(80);
ESP8266WebServer uploadServer(8080);
Ticker delayTimer;

/*
 * 延时启动
*/
void delayRestart(float t) {
  Serial.print("Restart after ");
  Serial.print(t);  Serial.println("s");
  delayTimer.attach(t, []() {
    Serial.println("\r\nRestart now!");
    ESP.restart();
  });
}

/*
 * 发送主页面
*/
void handleRoot()
{
  httpServer.send(200, "text/html", mainPageString);
  httpServer.client().stop();
}

/*
 * 发送参数到网页
*/
void handleConfig()
{
  Serial.println("handleConfig");
  char str[256];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  IPAddress ip = WiFi.localIP();
  sprintf(str, "%d.%d.%d.%d^$&%02X-%02X-%02X-%02X-%02X-%02X^$&%s^$&%s^$&%s^$&%s^$&", ip[0], ip[1], ip[2], ip[3], mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], config.id, config.key, config.stassid, config.stapsw);
  httpServer.send(200, "text/plain", str);
  httpServer.client().stop();
}
/*
 * 扫描ssid并发送
*/
void handleAPList()
{
  Serial.println("handleAPList");
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" networks found");
  String str;
  for (int i = 0; i < n; ++i)
  {
    str = str + WiFi.SSID(i) + "^$&";
  }
  httpServer.send(200, "text/plain", str);
  httpServer.client().stop();
}
/*
 * 转向固件页面
*/
void handleUpdate()
{
  Serial.println("handleUpdate");
  Serial.println("Uri:" + httpServer.uri());
  Serial.println(httpServer.client().remoteIP());
  char strIPRemote[32], strIPLocal[32], strIPLocalSTA[32];
  IPAddress ipRemote = httpServer.client().remoteIP();
  sprintf(strIPRemote, "%d.%d.%d", ipRemote[0], ipRemote[1], ipRemote[2]);
  IPAddress iplocal = WiFi.softAPIP();
  IPAddress iplocalSTA = WiFi.localIP();
  sprintf(strIPLocal, "%d.%d.%d", iplocal[0], iplocal[1], iplocal[2]);
  char sendStr[128];
  if (strcmp(strIPLocal, strIPRemote) == 0)
    sprintf(sendStr, "<script>window.location=\"http://%d.%d.%d.%d:8080\";</script>", iplocal[0], iplocal[1], iplocal[2], iplocal[3]);
  else
    sprintf(sendStr, "<script>window.location=\"http://%d.%d.%d.%d:8080\";</script>", iplocalSTA[0], iplocalSTA[1], iplocalSTA[2], iplocalSTA[3]);
  httpServer.send(200, "text/html", sendStr );
}
/*
 * 处理post参数
*/
void handlePost()
{ //httpServer.args()  httpServer.argName(i)  arg(i)
  //Arg number: 1
  //Arg Name: ssid
  //Arg : 'Doit',psw='doit3305',device_id='DOIT-SN-851A-73814',device_key='e14b22b5b29bceba4e8389f2a1182763',
  char str[512];
  char *s = NULL, *e = NULL;
  /*
    Serial.println("handdlePost");
    Serial.println(httpServer.args());
    Serial.print("Arg number: ");
    Serial.println(httpServer.args());
    Serial.print("Arg Name: ");
    Serial.println(httpServer.argName(0));
    Serial.print("Arg : ");
    Serial.println(httpServer.arg(0));*/
  if (httpServer.argName(0) != "ssid" ) {
    goto Exit;
  }
  strcpy(str, httpServer.arg(0).c_str());
  s = strstr(str, "'");
  if (s == NULL) goto Exit;
  e = strstr(s + 1, "'");
  if (e == NULL) goto Exit;
  char ssid[64]; memset(ssid, 0x00, 64);
  memcpy(ssid, s + 1, e - s - 1);

  s = strstr(e + 1, "'");
  if (s == NULL) goto Exit;
  e = strstr(s + 1, "'");
  if (e == NULL) goto Exit;
  char psw[64]; memset(psw, 0x00, 64);
  memcpy(psw, s + 1, e - s - 1);

  s = strstr(e + 1, "'");
  if (s == NULL) goto Exit;
  e = strstr(s + 1, "'");
  if (e == NULL) goto Exit;
  char id[64]; memset(id, 0x00, 64);
  memcpy(id, s + 1, e - s - 1);

  s = strstr(e + 1, "'");
  if (s == NULL) goto Exit;
  e = strstr(s + 1, "'");
  if (e == NULL) goto Exit;
  char key[64]; memset(key, 0x00, 64);
  memcpy(key, s + 1, e - s - 1);

  strcpy(config.stassid, ssid);
  strcpy(config.stapsw, psw);
  strcpy(config.id, id);
  strcpy(config.key, key);
  saveConfig();
  httpServer.send(200, "text/plain", "Save success, Module will restart!");
  Serial.printf("\r\nSave Config Success\r\nRebooting after 3s.\r\n");
  delayRestart(1);
  return;
Exit:
  httpServer.send(200, "text/plain", "Arguments is wrong!");
  return;
}
/*
 * OTA结束
*/
void uplaodFinish() {
  uploadServer.sendHeader("Connection", "close");
  uploadServer.sendHeader("Access-Control-Allow-Origin", "*");
  uploadServer.send(200, "text/html",  (Update.hasError()) ? updatePageString_ResultFailed : updatePageString_ResultOK);
}
/*
 * 初始化OTA服务
*/
void initUploadServer() {
  uploadServer.onNotFound([]() {
    uploadServer.sendHeader("Connection", "close");
    uploadServer.sendHeader("Access-Control-Allow-Origin", "*");
    uploadServer.send(200, "text/html", updatePageString);
  });
  uploadServer.on("/", HTTP_GET, []() {
    uploadServer.sendHeader("Connection", "close");
    uploadServer.sendHeader("Access-Control-Allow-Origin", "*");
    uploadServer.send(200, "text/html", updatePageString);
  });
  uploadServer.on("/update", HTTP_POST, []() {
    uplaodFinish();
  }, []() {
    HTTPUpload& upload = uploadServer.upload();
    if (upload.status == UPLOAD_FILE_START) {
      if (strlen(upload.filename.c_str()) == 0) {
        Serial.printf("No filed choosed abort!");
        Update.end(true);
        return;
      }
      Serial.setDebugOutput(true);
      WiFiUDP::stopAll();
      Serial.printf("Update: %s\r\n", upload.filename.c_str());
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
      else
      {
        Serial.print(".");
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("\r\nUpdate Success: %u\r\nRebooting after 3s.\r\n", upload.totalSize);
        delayRestart(1);
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
    }
    yield();
  });
  uploadServer.begin();
  Serial.println("\r\nUpload server started at port 8080");
}
/*
 * 初始化http服务器
*/
void initHttpServer()
{
  initUploadServer();
  httpServer.on("/", HTTP_GET, handleRoot);
  httpServer.on("/para_list.do", HTTP_GET, handleConfig);
  httpServer.on("/ap_list.do", HTTP_GET, handleAPList);
  httpServer.on("/update.do", HTTP_GET, handleUpdate);
  httpServer.on("/submit", HTTP_POST, handlePost);
  httpServer.onNotFound(handleRoot);
  httpServer.begin();
}
/*
 * http服务器tick
*/
void doHttpServerTick()
{
  httpServer.handleClient();
  uploadServer.handleClient();
}

