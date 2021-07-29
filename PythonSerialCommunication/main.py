


# W:시간 T:온도 H:습도 D:먼지 C:Co2
import serial
RX = serial.Serial(port='/dev/tty.usbmodem101', baudrate=9600)#맥일때 포트 형식
# RX = serial.Serial('/dev/ttyACM0', baudrate=9600)#리눅스 일때



def Slicing(read):
    Time = ""
    Temp = ""
    Humi = ""
    Dust = ""
    Co2  = ""
    if read[:1] != b"W" or read[len(read)-1:] != b"\n":#시작이 W가 아니거나 끝이 n이 아닌경우 에러 출력하고 종료
        print("ERROR")                              #에러일경우 DB에 어찌 처리할지는 너가 생각해보세염
        return 0;

    for i in range(0,len(read)): #시간 넣기


        if chr(read[i]) == "W":
            Time = Time + chr(read[i + 2])
            Time = Time + chr(read[i + 3])
            Time = Time + chr(read[i + 4])
            Time = Time + chr(read[i + 5])

        if chr(read[i]) == "T":      #온도 넣기
            Temp = Temp + chr(read[i + 2])
            Temp = Temp + chr(read[i + 3])
            Temp = Temp + chr(read[i + 4])
            if chr(read[i+5]) != ",":  #온도가 10도 이하(문자열 3자리)인 경우를 대비
                Temp = Temp + chr(read[i + 5])

        if chr(read[i]) == "H":  # 습도 넣기
            Humi = Humi + chr(read[i + 2])
            Humi = Humi + chr(read[i + 3])
            Humi = Humi + chr(read[i + 4])
            if chr(read[i + 5]) != ",":  # 습도가 10 이하(문자열 3자리)인 경우를 대비
                Humi = Humi + chr(read[i + 5])


        if chr(read[i]) == "D":  # 먼지 넣기 한자리수면 하나 두자릿수면 둘 세자리수면 셋...
            Dust = Dust + chr(read[i + 2])
            if chr(read[i + 3]) != ",":
                Dust = Dust + chr(read[i + 3])
            if chr(read[i + 4]) != ",":
                Dust = Dust + chr(read[i + 4])

        if chr(read[i]) == "C":  # 먼지 넣기 한자리수면 하나 두자릿수면 둘 세자리수면 셋 네자리수면 넷
            Co2 = Co2 + chr(read[i + 2])
            if chr(read[i + 3]) != ",":
                Co2 = Co2 + chr(read[i + 3])
            if chr(read[i + 4]) != ",":
                Co2 = Co2 + chr(read[i + 4])
            if chr(read[i + 5]) != ",":
                Co2 = Co2 + chr(read[i + 5])


    #여기까지 오면 각 변수에 삽입 완료상태
    print("Time: ",Time)
    print("Temp: ", Temp)
    print("Humi: ", Humi)
    print("Dust: ", Dust)
    print("Co2: ",Co2)





while(True):


    RX.write(b't') #아스키코드로 t 시리얼 통신 보내기
    read = RX.readline()
    # read = b'W:1201,T:11.5,H:41.5,D:51,C:501,\r\n' #테스트용 코드
    print(read)  # 읽은 내용 출력
    Slicing(read) #읽은 내용 함수로 보내기
