/*
 udp demo --upd server at port 8000
 version:1.0
 2016-01-05
 
 Copyright (c) 2015 www.doit.am All rights reserved.
 
 This file is part of the esp8266 core for Arduino environment.
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define MAX_PACKETSIZE 512
WiFiUDP udp;
const char* ssid = "Doit";
const char* password = "doit3305";
char buffUDP[MAX_PACKETSIZE];

void startUDPServer(int port)
{
  Serial.print("\r\nStartUDPServer at port:");
  Serial.println(port);
  udp.begin(port);
}
void sendUDP(char *p)
{
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.write(p);udp.endPacket();
}
void doUdpServerTick()
{
  int packetSize = udp.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = udp.remoteIP();
    for (int i = 0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) Serial.print(".");
    }
    Serial.print(", port ");
    Serial.println(udp.remotePort());
    memset(buffUDP,0x00,sizeof(buffUDP));
    udp.read(buffUDP, MAX_PACKETSIZE-1);
    udp.flush();
    Serial.println("Recieve:");
    Serial.println(buffUDP);
    sendUDP(buffUDP);//send back
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Started");
  WiFi.begin(ssid, password);
  //WiFi.softAP("ESP8266_WIFI");
  Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
  if(i == 21){
    Serial.print("Could not connect to"); Serial.println(ssid);
    while(1) delay(500);
  }
  Serial.println(WiFi.localIP());
  startUDPServer(8000);
}

void loop() {
  doUdpServerTick();
  delay(1);
}
