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
char addr[4] = {1,2,3,4};
int save[10] = {0,0,0,0};
char List_Of_Segments[4] = { 0x01, 0x02, 0x04, 0x08 };
char List_Of_Segment_Info[10] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x98 };
                                                                      
void setup() {
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
    digitalWrite(12,0);
  } else {
    digitalWrite(12,1);
  }

  if(SegmentNumx16 & 0x04){
    digitalWrite(11,0);
  } else {
    digitalWrite(11,1);
  }

  if(SegmentNumx16 & 0x02){
    digitalWrite(10,0);
  } else {
    digitalWrite(10,1);
  }

  if(SegmentNumx16 & 0x01){
    digitalWrite(9,0);
  } else {
    digitalWrite(9,1);
  }

/////////////////////////////////

  if(PrintNumx16 & 0x40){
    digitalWrite(8, 0);
  } else {
    digitalWrite(8, 1);
  }

  if(PrintNumx16 & 0x20){
    digitalWrite(7, 0);
  } else {
    digitalWrite(7, 1);
  }  

  if(PrintNumx16 & 0x10){
    digitalWrite(6, 0);
  } else {
    digitalWrite(6, 1);
  }
  
  if(PrintNumx16 & 0x08){
    digitalWrite(5, 0);
  } else {
    digitalWrite(5, 1);
  }

  if(PrintNumx16 & 0x04){
    digitalWrite(4, 0);
  } else {
    digitalWrite(4, 1);
  }

  if(PrintNumx16 & 0x02){
    digitalWrite(3, 0);
  } else {
    digitalWrite(3, 1);
  }

  if(PrintNumx16 & 0x01){
    digitalWrite(2, 0);
  } else {
    digitalWrite(2, 1);
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
  

  input = 1234;
  addr[0] = input / 1000;
  addr[1] = input % 1000 / 100;
  addr[2] = input % 100 / 10;
  addr[3] = input % 10;
  delay(1000);
  


  

}
