#include <MHZ19PWM.h>
MHZ19PWM mhz(3, MHZ_DELAYED_MODE);
#include "DHT.h"
#define DHTPIN 2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
unsigned long pulse = 0;                                   // pulse를 0으로 설정합니다.
float ugm3 = 0;                                                 // ugm3을 0으로 설정합니다.
int GP2Y1023 = 8;  
void setup() {
  Serial.begin(9600);
  pinMode(GP2Y1023, INPUT);  
  dht.begin();
  lcd.init();
  lcd.backlight();
  mhz.useLimit(5000);
}
float c;
void loop() {
  float d;
  float humi_Sum = 0; 
  for (int i = 0; i <= 100; i++){
      humi_Sum = humi_Sum + dht.readHumidity();
  }
  float h = humi_Sum/100;

  float temp_Sum = 0; 
  for (int i = 0; i <= 100; i++){
      temp_Sum = temp_Sum + dht.readTemperature();
  }
  float t = temp_Sum/100;

  pulse = pulseIn(GP2Y1023, LOW, 20000); // pulse에 GP2Y1023에서 LOW 신호를 받을 때까지 걸리는 시간을 받아옵니다.
  ugm3 = pulse2ugm3(pulse);                   // ugm3를 pulse2ugm3()함수에 pulse를 넣어 반환된 값을 저장합니다.
  if (ugm3 > 1) {     
    d = ugm3;
    Serial.print(ugm3, 4);                         // ugm3 변수의 소숫점 4자리수까지 출력합니다.
    Serial.println(" ug/m3");
  }

  
  
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(t);
  lcd.print(" D:");
  lcd.print(d);
  lcd.setCursor(0,1);
  lcd.print("H:"); 
  lcd.print(h);   
  lcd.print(" C:");
  lcd.print(c);
  delay(1000);      
  CO2_Check();
//  lcd.clear();
}

void CO2_Check()
{
  c = mhz.getCO2();
}


float pulse2ugm3(unsigned long pulse) {
  float value = (pulse - 1400) / 14.0;                // pulse에 -1400을 하고 14.0을 나누어 value에 저장합니다.
  if (value > 300) {                                        // value가 300보다 크면
    value = 0;                                                // value이 0으로 저장합니다.
  }
  return value;                                             // value 값을 반환합니다.
}
