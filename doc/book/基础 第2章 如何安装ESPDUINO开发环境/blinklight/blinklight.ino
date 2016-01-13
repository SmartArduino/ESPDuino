#define PIN_LED 16
#define KEY_FLASH 0

void setup() {
  pinMode(PIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(KEY_FLASH, INPUT);
  digitalWrite(PIN_LED,LOW);//点亮LED灯
}
void keydo(){
 if(digitalRead(KEY_FLASH)==0)
  digitalWrite(PIN_LED,HIGH);
  else
  digitalWrite(PIN_LED,LOW);
}

// the loop function runs over and over again forever
void loop() {
  
  digitalWrite(PIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  delay(500);                      // Wait for a second
  keydo();
  digitalWrite(PIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(500);                      // Wait for two seconds (to demonstrate the active low LED)
  
  
}
