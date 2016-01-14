#define PIN_LED 16

void setup() {
  pinMode(PIN_LED, OUTPUT);
  analogWrite(PIN_LED, 0);//模拟信号输出
}

void loop() {
  for (int i = 0; i < 1024; i++)
  {//电平升高，从明到暗
    analogWrite(PIN_LED, i);
    delay(2);
  }
  for (int i = 1024; i >= 0; i--)
  {//电平降低，从暗到明
    analogWrite(PIN_LED, i);
    delay(2);
  }
}

