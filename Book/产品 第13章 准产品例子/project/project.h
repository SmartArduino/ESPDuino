#ifndef _MAIN_H__
#define _MAIN_H__


extern "C" {
#include "user_interface.h"
#include "smartconfig.h"
}
struct config_type
{
  char stassid[32];
  char stapsw[64];
  char id[64];
  char key[64];
  uint8_t magic;
};

void startTCPClient();
void doTCPClientTick();
void startUDPServer(int);
void doUdpServerTick();
void sendTCP(char *);
void sendUDP(char *);
void initParseData();
void parseTCPPackage(char*);
void parseUDPPackage(char*);
void parseUartPackage(char*);

void initHttpServer();
void doHttpServerTick();
void delayRestart(float);
void printHelp();

//常量
#define VER             "V 1.0"
#define DEFAULT_APSSID  "Doit_Product_Demo"
#define DEFAULT_STASSID ""
#define DEFAULT_STAPSW  ""
#define PINLED          16
#define PINKEY          0
#define HOST_NAME       "DoitWiFi_Device"

//产品相关
#define DEFAULT_ID    "device_demo"
#define DEFAULT_KEY    "123456"

const char apssid[]=DEFAULT_APSSID;
const char serverAddr[] = "book.doit.am";
const uint16_t serverPort = 5588;
const uint16_t udpPort = 8089;

unsigned long lastWiFiCheckTick = 0;
bool ledState = 0;

config_type config;

#endif
