#define PIN_LED 16
#define KEY_FLASH 0
volatile int state = LOW;// 定义默认输入状态
void setup() {
  pinMode(PIN_LED, OUTPUT);    
  pinMode(KEY_FLASH, INPUT);
  digitalWrite(PIN_LED,LOW);//初始LED灯置为低电平，表示点亮
  // 监视中断输入按键的变化，此函数为中断函数入口句柄
  attachInterrupt(KEY_FLASH,stateChange,CHANGE);
}
//中断函数
void stateChange(){
 //LED灯状态翻转，实现发生外部按键事件时对LED灯状态的改变
 state=!state; 
 digitalWrite(PIN_LED,state);
}
void loop() {  
 //delay(1000);
 //digitalWrite(PIN_LED,HIGH);
 //delay(1000);
 //digitalWrite(PIN_LED,LOW);
 }
