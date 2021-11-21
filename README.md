# Air-Alarm Embedded

## 개발환경

- Mac OS Big Sur 11.6
- STM32 F401RE
- STM32CUBEIDE
- EasyEDA
- CLion

## H/W 구성
- DHT22
- MH-Z19B
- GP2Y1023AU0F
- 16x2 I2C LCD Display
- S-5462CSR2/C(14Pin SevenSegment)


## 파일/폴더 안내
- Air-Alarm Gerber_PCB : PCB 거버 파일
- Atmega328P-PU : ARM 개발 단계 이전 AVR 환경의 테스트 파일
- DataSheet : 사용한 개발보드, MCU, 센서 데이터시트 파일
- PythonSerialCommunication : 센서서버의 Uart 통신 프로토콜 파일
- STM32NUCLEO-F401RE : 개발 보드 프로젝트 파일
    - /Core : 개발한 소스코드 파일

## 사진
### 회로도
![Schematic_Air-Alarm_2021-10-19](https://user-images.githubusercontent.com/29862835/137789778-72d794f4-bb01-4017-8e08-25e47408e736.png)
 
### PCB ArtWork
![PCB_PCB_Air-Alarm_5_2021-10-18](https://user-images.githubusercontent.com/29862835/137663251-5535261c-13d4-4c27-870f-81da1563aeed.png)<img width="330" alt="스크린샷 2021-10-18 오후 4 55 48" src="https://user-images.githubusercontent.com/29862835/137690789-eb2feafe-ffb0-4929-830c-cf17b60b148e.png">

## 참조
- I2C LCD 제어 [MYaqoobEmbedded](https://github.com/MYaqoobEmbedded/STM32-Tutorials/tree/master/Tutorial%2011%20-%20LCD16x2)

