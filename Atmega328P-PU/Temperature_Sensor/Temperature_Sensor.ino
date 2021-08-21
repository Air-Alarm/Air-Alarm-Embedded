/*
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D

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
int D4 = 12;int pinA = 2;
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
 */
#include "DHT.h"
#include <MsTimer2.h>
#define DHTPIN A0 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int input;
int loopcount = 0;
float h;
float t;
char addr[4] = {0,0,0,0};

char List_Of_Segments[4] = { 0x01, 0x02, 0x04, 0x08 };
char List_Of_Segment_Info[10] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x98 };
volatile boolean state = true;
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
  
  // initialize the digital pins as outputs.

  for (int i = 2; i < 13; i++){
    pinMode(i, OUTPUT); 
  } 
  pinMode(A1, OUTPUT);
  MsTimer2::set(2, Segmentpush);
  MsTimer2::start();
}

void Segment(char SegmentNumx16, char PrintNumx16){
  if(SegmentNumx16 & 0x08){
    digitalWrite(12,1);
  } else {
    digitalWrite(12,0);
  }

  if(SegmentNumx16 & 0x04){
    digitalWrite(11,1);
  } else {
    digitalWrite(11,0);
  }

  if(SegmentNumx16 & 0x02){
    digitalWrite(10,1);
  } else {
    digitalWrite(10,0);
  }

  if(SegmentNumx16 & 0x01){
    digitalWrite(9,1);
  } else {
    digitalWrite(9,0);
  }

/////////////////////////////////

  if(PrintNumx16 & 0x40){
    digitalWrite(8, 1);
  } else {
    digitalWrite(8,0);
  }

  if(PrintNumx16 & 0x20){
    digitalWrite(7, 1);
  } else {
    digitalWrite(7,0);
  }  

  if(PrintNumx16 & 0x10){
    digitalWrite(6, 1);
  } else {
    digitalWrite(6,0);
  }
  
  if(PrintNumx16 & 0x08){
    digitalWrite(5, 1);
  } else {
    digitalWrite(5,0);
  }

  if(PrintNumx16 & 0x04){
    digitalWrite(4, 1);
  } else {
    digitalWrite(4,0);
  }

  if(PrintNumx16 & 0x02){
    digitalWrite(3, 1);
  } else {
    digitalWrite(3,0);
  }

  if(PrintNumx16 & 0x01){
    digitalWrite(2, 1);
  } else {
    digitalWrite(2, 0);
  }
  
}

int i = 0;
void Segmentpush(){
  Segment(List_Of_Segments[i], List_Of_Segment_Info[addr[i]]);
  i++;
  if (i==4){
    i = 0;
  }
}



// the loop routine runs over and over again forever:
void loop() {
  t = dht.readTemperature();// 온도를 측정합니다.
  digitalWrite(A1,HIGH);
  Serial.print("T: ");
  Serial.print(t);
  Serial.println("/");
  digitalWrite(A1,LOW);
  input = t*10;
  addr[0] = input / 1000;
  addr[1] = input % 1000 / 100;
  addr[2] = input % 100 / 10;
  addr[3] = input % 10;
  delay(2000);

  


}
