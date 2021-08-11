unsigned long pulse = 0;                               
float ugm3 = 0;                                              
int GP2Y1023 = 8;                                            
void setup() {
  pinMode(GP2Y1023, INPUT);                       
  Serial.begin(115200);                          
}
void loop() {
  pulse = pulseIn(GP2Y1023, LOW, 20000); 
  ugm3 = pulse2ugm3(pulse);                  
  if (ugm3 > 1) {                                   
    Serial.print(ugm3, 4);                       
    Serial.println(" ug/m3");
  }
  delay(100);
}
float pulse2ugm3(unsigned long pulse) {
  float value = (pulse - 1400) / 14.0;              
  if (value > 300) {                                      
    value = 0;                                              
  }
  return value;                                          
}
