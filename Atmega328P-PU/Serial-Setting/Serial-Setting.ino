int time = 0;
void setup() {
  Serial.begin(9600);
  
}

//W:시간 T:온도 H:습도 D:먼지 C:Co2
void loop() {
  if (time % 100 == 60) {
    time += 100;
    time -= 60;
  }
  if (time > 2359) {
    time = 0;
  }

  
  Serial.print("W:");
  if (time < 10){
    Serial.print("000");
  }
  else if(time < 100){
    Serial.print("00");
  }
  else if(time < 1000){
    Serial.print("0");
  }

  Serial.print(time);
  Serial.println(",T:11.5,H:41.5,D:51,C:501,");
  delay(10000);
  time = time+1;

}
