void setup() {
  Serial.begin(9600);
}

//W:시간 T:온도 H:습도 D:먼지 C:Co2
void loop() {
  Serial.println("W:1201,T:11.5,H:41.5,D:51,C:501,");
  delay(1000);
  Serial.println("W:1202,T:12.5,H:42.5,D:52,C:502,");
  delay(1000);
  Serial.println("W:1203,T:13.5,H:43.5,D:53,C:503,");
  delay(1000);
  Serial.println("W:1204,T:14.5,H:44.5,D:54,C:504,");
  delay(1000);
  Serial.println("W:1205,T:15.5,H:45.5,D:55,C:505,");
  delay(1000);
  Serial.println("W:1206,T:16.5,H:46.5,D:56,C:506,");
  delay(1000);

}
