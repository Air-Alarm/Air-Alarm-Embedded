volatile boolean state = false;
 
void setup(){
    pinMode(2,OUTPUT);
    Serial.begin(9600);
    Serial.println("Serial Port Connected!");
}
 
void loop(){
  Serial.print("Help!!!!"); 
  delay(100);
}

void serialEvent(){
  char cTemp = Serial.read();
    Serial.print(cTemp);
}
