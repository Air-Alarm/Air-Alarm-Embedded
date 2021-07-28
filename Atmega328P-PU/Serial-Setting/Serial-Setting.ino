void setup() {
  Serial.begin(9600);
}

void loop() {
  
  Serial.println("Time:1201,Temp:11.5,Humi:41.5,Dust:41");
  delay(1000);
  Serial.println("Time:1202,Temp:12.5,Humi:42.5,Dust:42");
  delay(1000);
  Serial.println("Time:1203,Temp:13.5,Humi:43.5,Dust:43");
  delay(1000);
  Serial.println("Time:1204,Temp:14.5,Humi:44.5,Dust:44");
  delay(1000);

}
