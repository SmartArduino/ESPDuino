/*
  Doit tcp test demos
  version:1.0
  2016-01-06

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


/*
  1, Visit "tcp.doit.am" in your brower to get an opened ip and port
  2, Change "serverIP" and "serverPort" in this sktch, compile and run in ESPduino
  3, Use a client such as TCPUDP Debugger to setup a tcp client using the ip and port
  4, Transparent transmisstion is setup between ESPduino and the tcp client.
*/

#include <ESP8266WiFi.h>

const char* ssid      = "Doit";    //change to your own ssid
const char* password  = "doit3305";//change to your own password
const char* serverIP  = "115.29.109.104";
int serverPort = 6535;
WiFiClient client;
bool bConnected = false;
char buff[512];
int nm = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  delay(1);
  if (bConnected == false)
  {
    if (!client.connect(serverIP, serverPort))
    {
      Serial.println("connection failed");
      delay(5000);
      return;
    }
    bConnected = true;
    Serial.println("connection ok");
  }
  else if (client.available())
  {
    Serial.println("Data is coming");
    while (client.available())
    {
      buff[nm++] = client.read();
      if (nm >= 511) break;
    }
    buff[nm] = 0x00;
    nm=0;
    Serial.print(buff);
    client.print(buff);
    client.flush();
  }
}

