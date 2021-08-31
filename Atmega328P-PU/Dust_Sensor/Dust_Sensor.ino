#include <MsTimer2.h>
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

int input;
int loopcount = 0;
char addr[4] = {0,0,0,0};
int save[10] = {0,0,0,0};
char List_Of_Segments[4] = { 0x01, 0x02, 0x04, 0x08 };
char List_Of_Segment_Info[10] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x98 };
volatile boolean state = true;

unsigned long pulse = 0;                               



float ugm3 = 0;
                                          
int GP2Y1023 = A0;                                            
void setup() {
  pinMode(GP2Y1023, INPUT); 
  pinMode(13, OUTPUT);                     
  Serial.begin(9600); 
  for (int i = 2; i < 13; i++){
    pinMode(i, OUTPUT);       
  }
  Serial.print("Start"); 
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

void loop() {
 
  pulse = pulseIn(GP2Y1023, LOW, 20000); 
  ugm3 = pulse2ugm3(pulse); 
 
  
  
//  pulse = pulseIn(GP2Y1023, LOW, 20000); 
//  ugm3 = pulse2ugm3(pulse);                  
//  if (ugm3 > 1){
//    oldugm3 = ugm3;
//  }
//  else{
//    ugm3 = oldugm3;
//  }
  if (ugm3 > 1){
    input = ugm3;
  }
  addr[0] = input / 1000;
  addr[1] = input % 1000 / 100;
  addr[2] = input % 100 / 10;
  addr[3] = input % 10;


  
  
  delay(5000);
}

void serialEvent(){
  char cTemp = Serial.read();
    if(cTemp == 'D'||cTemp == 'd'){
      
      digitalWrite(13,HIGH);
      Serial.print("D: ");                                 
      Serial.print(input);                       
      Serial.println("/");
      delay(14);
      digitalWrite(13,LOW);
    }

}

float pulse2ugm3(unsigned long pulse) {
  float value = (pulse - 1400) / 14.0;              
  if (value > 300) {                                      
    value = 0;                                              
  }
  
  return value;                                          
}
