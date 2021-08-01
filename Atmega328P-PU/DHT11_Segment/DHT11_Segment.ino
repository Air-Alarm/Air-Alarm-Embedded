/*
  Showing number 0-9 on a Common Anode 7-segment LED display
  Displays the numbers 0-9 on the display, with one second inbetween.
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D
  This example code is in the public domain.
 */
#include "DHT.h"
#define DHTPIN A0 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int D1 = 9;
int D2 = 10;
int D3 = 11;
int D4 = 12;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
                  
  // initialize the digital pins as outputs.
  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);  
}

void Segment(char SegmentPin,char PrintNumx16){
  if(SegmentPin == 1){
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
  }
  if(SegmentPin == 2){
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
  }
  if(SegmentPin == 3){
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
  }
  if(SegmentPin == 4){
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
  }
  
  if(PrintNumx16  == 0){
    digitalWrite(pinA, LOW);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, LOW);   
    digitalWrite(pinE, LOW);   
    digitalWrite(pinF, LOW);   
    digitalWrite(pinG, HIGH); 

  }

    if(PrintNumx16  == 1){
    digitalWrite(pinA, HIGH);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, HIGH);   
    digitalWrite(pinE, HIGH);   
    digitalWrite(pinF, HIGH);   
    digitalWrite(pinG, HIGH); 

  }

  if(PrintNumx16  == 2){
    digitalWrite(pinA, LOW);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, HIGH);   
    digitalWrite(pinD, LOW);   
    digitalWrite(pinE, LOW);   
    digitalWrite(pinF, HIGH);   
    digitalWrite(pinG, LOW); 

  }
  if(PrintNumx16  == 3){
    digitalWrite(pinA, LOW);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, LOW);   
    digitalWrite(pinE, HIGH);   
    digitalWrite(pinF, HIGH);   
    digitalWrite(pinG, LOW); 

  }

  if(PrintNumx16  == 4){
    digitalWrite(pinA, HIGH);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, HIGH);   
    digitalWrite(pinE, HIGH);   
    digitalWrite(pinF, LOW);   
    digitalWrite(pinG, LOW); 
 
  }
  if(PrintNumx16  == 5){
    digitalWrite(pinA, LOW);   
    digitalWrite(pinB, HIGH);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, LOW);   
    digitalWrite(pinE, HIGH);   
    digitalWrite(pinF, LOW);   
    digitalWrite(pinG, LOW); 

  }

  if(PrintNumx16  == 6){
    digitalWrite(pinA, LOW);   
    digitalWrite(pinB, HIGH);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, LOW);   
    digitalWrite(pinE, LOW);   
    digitalWrite(pinF, LOW);   
    digitalWrite(pinG, LOW); 
 
  }
  if(PrintNumx16  == 7){
    digitalWrite(pinA, LOW);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, HIGH);   
    digitalWrite(pinE, HIGH);   
    digitalWrite(pinF, LOW);   
    digitalWrite(pinG, HIGH); 

  }

  if(PrintNumx16  == 8){
    digitalWrite(pinA, LOW);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, LOW);   
    digitalWrite(pinE, LOW);   
    digitalWrite(pinF, LOW);   
    digitalWrite(pinG, LOW); 

  }

  if(PrintNumx16  == 9){
    digitalWrite(pinA, LOW);   
    digitalWrite(pinB, LOW);   
    digitalWrite(pinC, LOW);   
    digitalWrite(pinD, LOW);   
    digitalWrite(pinE, HIGH);   
    digitalWrite(pinF, LOW);   
    digitalWrite(pinG, LOW); 

  }
}
int input;
int loopcount = 0;
// the loop routine runs over and over again forever:
void loop() {
  float h;
  float t;
  if (loopcount % 100 == 0){
    h = 23.1;//dht.readHumidity();// 습도를 측정합니다.
    t = 23.1;//dht.readTemperature();// 온도를 측정합니다.
//    Serial.print("H: ");
//    Serial.print(h);
//    Serial.print("/");
//    Serial.print("T: ");
//    Serial.print(t);
//    Serial.println(" *C ");
  }

  

  char addr[4] = {0,0,0,0};
  input = t*10;
  
  
  addr[0] = input / 1000;
  addr[1] = input % 1000 / 100;
  addr[2] = input % 100 / 10;
  addr[3] = input % 10;
 
  //0
  Segment(1,addr[0]);
  delay(1);
  Segment(2,addr[1]);
  delay(1);
  Segment(3,addr[2]);
  delay(1);
  Segment(4,addr[3]);
  delay(1);
  loopcount++;

}
