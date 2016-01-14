#define PIN_LED 16 //定义LED灯：PIN_LED所对应的引脚为16号
#define KEY_FLASH 0 //定义KEY_FLASH 对应0号引脚，其实就是FLASH按键位置

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(PIN_LED, OUTPUT); //初始化PIN_LED引脚模式为输出
  pinMode(KEY_FLASH, INPUT);//初始化Flash按键模式为输入
  digitalWrite(PIN_LED,HIGH);//初始LED灯置为低电平，表示点亮
  delay(1000); //延时1秒
}
//自定义按键函数
void keydo(){
 //读出当前按键状态，0表示抬起断开，1为按下接通
 int k;
 k=digitalRead(KEY_FLASH);
 Serial.println("\r\n keydo:");
 Serial.write(k);
 Serial.println();
 if(k==0) 
  digitalWrite(PIN_LED,HIGH); //高电平表示灯灭
  else
  digitalWrite(PIN_LED,LOW);
}

void loop() {
  //digitalWrite(PIN_LED, LOW);  
  delay(500);                      
  keydo();
  //digitalWrite(PIN_LED, HIGH);  
  delay(500);                      
}
