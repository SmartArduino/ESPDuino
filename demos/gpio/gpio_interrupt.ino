/*
 gpio interrupt demo
 version:1.0
 2016-01-04
 
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

int pin = 16;
volatile int state = LOW;
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Start");
  pinMode(pin, OUTPUT);
  digitalWrite(pin, state);
  attachInterrupt(0, blink, RISING);//`CHANGE`, `RISING`, `FALLING`.
}
 
void loop()
{
  digitalWrite(pin, state);
}
 
void blink()
{
  state = !state;
  Serial.println("Enter");
}
