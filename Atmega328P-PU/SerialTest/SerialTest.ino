volatile boolean state = false;
 
void setup(){
    pinMode(2,OUTPUT);
    Serial.begin(9600);
    Serial.println("Serial Port Connected!");
}
 
void loop(){
//  digitalWrite(2,HIGH);
//  Serial.print("T");
//  delay(10);
//  digitalWrite(2,LOW);
//  delay(1000);
 
}

void serialEvent(){
  char cTemp = Serial.read();
    Serial.print(cTemp);
}
