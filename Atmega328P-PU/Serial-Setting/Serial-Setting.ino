
int time = 0;
int temp = 0;
int humi = 0;
int dust = 0;
int co2 = 0;
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
  Serial.print(",T:");
  temp = random(0,99);
  Serial.print(temp);

  Serial.print(".5,H:");
  humi = random(0,99);
  Serial.print(humi);



  Serial.print(".5,D:");
  dust = random(0,150);
  Serial.print(dust);
  
 
  Serial.print(",C:");
  co2 = random(400,6000);
  Serial.print(co2);



  Serial.println(",");
  delay(10000);

  
  time = time+1;

}
