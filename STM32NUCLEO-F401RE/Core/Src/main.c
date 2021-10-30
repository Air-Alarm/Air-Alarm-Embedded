/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd16x2_i2c.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */



//세그먼트 변수
int Seg_Out = 525; //세그먼트에 표시될 숫자
char second = 0; //세그먼트용 60초 카운트
uint8_t ms = 0; //세그먼트3ms 카운트
int segdig = 0;//세그먼트 출력 자릿수 지정

//Uart 변수
uint8_t rx2_data;
uint8_t buff[10];//uart 입력 버퍼
char Uart_Loop_Time = 0;//Uart 10초 카운트


uint8_t rx6_data[14];

//LCD 변수
char Line1[17];
char Line2[17];
uint8_t lcd = -3; //LCD 시간 카운트


//온습도

float temp_Humi[2] = {0.0, 0.0};
float temp = 0;
float humi = 0;
char DHT22_Loop_Time = -1;//온습도 5초마다 측정
char DHT22_Elapsed_Time = 0;
char DHT22_Stat_Check = 0;



//Co2
int C; //CO2 level
char CO2_Pin_State= 0;//현재 핀 상태
char OLD_CO2_Pin_State = 0;//이전 핀 상태
uint32_t CO2ms = 0; //CO2PWM 주기 카운트 시간
int rising_time;//라이징 엣지 시간
int falling_time;// 폴링엣지 시간
int rerising_time;//다음 주기 라이징 엣지 시간
char rising_check = 0;  //타이머에서 어떤거 체크할 순서인지 기록
char falling_check = 0;
char rerising_check = 0;
uint32_t TH; // High 시간
uint32_t TL; //Low 시간


//먼지 센서
char Dust_time = 0;

//디버그, 개발용 변수
//int checkms = 0;//메인 루프 시간 측정용
int Old_Loop_Count = 0;
int Loop_Count;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM11_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


void Seg_Off(void){//다음 세그먼트 출력전 모두 꺼서 잔상 없애기
	HAL_GPIO_WritePin(GPIOA, A_Pin|B_Pin|C_Pin|D_Pin|E_Pin|F_Pin|G_Pin, 0); //세그먼트 구성 핀 모두 끄기
	HAL_GPIO_WritePin(GPIOB, Dig1_Pin|Dig2_Pin|Dig3_Pin|Dig4_Pin, 1); //세그먼트 Dgit핀 모두 끄기
}


void Segment() {//세그먼트 숫자 출력
	unsigned char List_Of_Segment_Info[10] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99,
	 		  0x92, 0x82, 0xD8, 0x80, 0x98 };
	int addr[4];
	Seg_Off(); // 모든 세그먼트 끄기


	if(Seg_Out % 100 == 60){//60분, 24시간 카운트
		Seg_Out = Seg_Out + 40;
	}
	else if(Seg_Out >= 2400){
		Seg_Out = 0;
	}

	addr[0] = Seg_Out / 1000;
	addr[1] = Seg_Out % 1000 / 100;
	addr[2] = Seg_Out % 100 / 10;
	addr[3] = Seg_Out % 10;


	uint16_t i[4] = {1,2,4,8}; //세그먼트 Dgit 조정
	HAL_GPIO_WritePin(GPIOB, i[segdig], 0);

	uint16_t j = 0;//출력 핀과 입력 값의 비트연산을 위한 변수

	j |= (~(List_Of_Segment_Info[addr[segdig]]&0xFF))<<4; //PA4 부터로 옮기고시프트 4로 변경
	HAL_GPIO_WritePin(GPIOA, j, 1);

	segdig++;
	if (segdig == 4){
		segdig = 0;
	}

}

void check_Dust(){
//	int test[10] = {0x11, 0x02, 0x0B, 0x07, 0xDB, 0x11+0x02+0x0B+0x07+0xDB};
//	HAL_UART_Transmit(&huart6, (uint8_t*)test, sizeof(test)/4, 0xFF);
	char test[10] = {0x11, 0x01, 0x1F, 0xCF};

	HAL_UART_Transmit(&huart6, (uint8_t*)test, 4, 0xFF);


}


void check_CO2(){

	TH =  falling_time - rising_time;
	TL = rerising_time - falling_time;
	C = 2000*(TH-2)/(TH+TL-4)+250;

	rising_time = 0;
	falling_time  = 0;
	rerising_time = 0;

}


void LCD_Load_Print(){


	  sprintf(Line1, "Air-Alarm V1.0.0");
	  sprintf(Line2, "Air Monitor");
	  lcd16x2_i2c_clear();
	  lcd16x2_i2c_setCursor(0,0);
	  lcd16x2_i2c_printf(Line1);
	  lcd16x2_i2c_setCursor(1,0);
	  lcd16x2_i2c_printf(Line2);
	  lcd = 0;

}

void DHT_Startbit(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};//아웃풋 모드로 변경
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 1);
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 0);

}

char DHT_getData() {// 다음 측정주기까지1ms 이상 여유 있어야함.
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, 1);

	GPIO_InitTypeDef GPIO_InitStruct = {0};//인풋 모드로 변경
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	uint16_t timeout = 0;
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)) {
		timeout++;
		if (timeout > 10000) return 1;//10000회 반복동안 값 안들어오면 측정하지 않고 리턴
	}
	timeout = 0;

	while(!(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5))) {
		timeout++;
		if (timeout > 10000) return 1;
	}
	timeout = 0;

	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)) {
		timeout++;
		if (timeout > 10000) return 1;
	}

	uint8_t rawData[5] = {0,0,0,0,0};
	for(uint8_t a = 0; a < 5; a++) {
		for(uint8_t b = 7; b != 255; b--) {
			uint32_t hT = 0, lT = 0;

			while(!(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5))) lT++;

			timeout = 0;
			while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)) hT++;
			if(hT > lT) rawData[a] |= (1<<b);
		}
	}


	if((uint8_t)(rawData[0] + rawData[1] + rawData[2] + rawData[3]) == rawData[4]) {//오류 검사
		temp_Humi[1] = (float)(((uint16_t)rawData[0]<<8) | rawData[1])*0.1f;
		if(!(rawData[2] & (1<<7))) {

			temp_Humi[0] = (float)(((uint16_t)rawData[2]<<8) | rawData[3])*0.1f;
		}	else {
			rawData[2] &= ~(1<<7);
			temp_Humi[0] = (float)(((uint16_t)rawData[2]<<8) | rawData[3])*-0.1f;
		}
	}
	return 0;// 정상 종료
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//GPIO 인터럽트 콜백
{

	if (GPIO_Pin == 8192) {	//버튼 눌르면 1분 올리기

		int gap =
				Loop_Count < Old_Loop_Count ?
						Old_Loop_Count - Loop_Count :
						Loop_Count - Old_Loop_Count;

		if (gap > 100) {
			Seg_Out++;

		}
		Old_Loop_Count = Loop_Count;
	}

	else if(GPIO_Pin == 16384){

		int gap =
				Loop_Count < Old_Loop_Count ?
						Old_Loop_Count - Loop_Count :
						Loop_Count - Old_Loop_Count;

		if (gap > 100) {
			Seg_Out = Seg_Out + 100; //1시간 올리기

		}
		Old_Loop_Count = Loop_Count;


	}



  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
if(huart->Instance == USART6){

		HAL_UART_Receive_IT(&huart6, &rx6_data, 14);



	}



}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_USART6_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart6, &rx6_data, 14);
  HAL_TIM_Base_Start_IT(&htim10);//타이머 10 시작 1ms
  HAL_TIM_Base_Start_IT(&htim11);//타이머 11 시작 1s
  if(lcd16x2_i2c_init(&hi2c1)){//LCD init 하기
 	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, 1);
  }
  lcd16x2_i2c_clear();


  LCD_Load_Print();
  rising_check = 1;//Co2 라이징 포인트부터 잡기


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  unsigned char a = '2';
//
//	  HAL_UART_Transmit(&huart2, &a, 1, 10);


	  if (ms > 1){
	  	  		  Segment();//1ms마다 세븐세그먼트를 출력
	  	  		  ms = 0;
	  }

	  if (rising_time < falling_time && falling_time < rerising_time){
		  check_CO2();
	  }




	  if (Dust_time > 2){
		  check_Dust();
		  Dust_time = 0;
	  }






	  if (DHT22_Loop_Time >  5|| DHT22_Loop_Time == -1)//5초마다 온도 측정 스타트 비트 실행하고 진행
	  {
		  if (DHT22_Stat_Check == 0){
			  DHT_Startbit();
			  DHT22_Elapsed_Time = 0;
			  DHT22_Stat_Check = 1;
		  }
	  }

	  if (DHT22_Stat_Check && DHT22_Elapsed_Time > 17)//스타트 비트 출력 17ms 이상 경과한경우 읽기 시작
	  {
		  char DHT_Return = DHT_getData();
		  if (DHT_Return == 1){//타임아웃 리턴받은경우 다음 루프때 다시 측정하기
			  DHT22_Loop_Time = -1;
		  }
		  DHT22_Stat_Check = 0;
		  DHT22_Loop_Time = 0;
	  }




	  if (lcd > 10 || lcd == -1){

		  sprintf(Line1, "T: %2.1f  D: %d", temp_Humi[0], 15);
		  sprintf(Line2, "H: %2.1f  C: %d", temp_Humi[1], C);
		  lcd16x2_i2c_clear();
		  lcd16x2_i2c_setCursor(0,0);
		  lcd16x2_i2c_printf(Line1);
		  lcd16x2_i2c_setCursor(1,0);
		  lcd16x2_i2c_printf(Line2);
		  lcd = 0;
	  }


	  if(Uart_Loop_Time >= 10){
		char msg[40];
		sprintf(msg, "W:%d,T:%2.1f,H:%2.1f,D:%d,C:%d\n", Seg_Out, temp_Humi[0], temp_Humi[1],15,C);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFF);
		Uart_Loop_Time = 0;
	  }






    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM1_TRG_COM_TIM11_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
  /* EXTI15_10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 84-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xffff-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 10000-1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 8400-1;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 100-1;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 840-1;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, A_Pin|B_Pin|C_Pin|D_Pin
                          |E_Pin|F_Pin|G_Pin|DotT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Dig1_Pin|Dig2_Pin|Dig3_Pin|Dig4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : A_Pin B_Pin C_Pin D_Pin
                           E_Pin F_Pin G_Pin DotT_Pin */
  GPIO_InitStruct.Pin = A_Pin|B_Pin|C_Pin|D_Pin
                          |E_Pin|F_Pin|G_Pin|DotT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : Dig1_Pin Dig2_Pin Dig3_Pin Dig4_Pin */
  GPIO_InitStruct.Pin = Dig1_Pin|Dig2_Pin|Dig3_Pin|Dig4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Switch2_Pin Switch1_Pin */
  GPIO_InitStruct.Pin = Switch2_Pin|Switch1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : CO2_Pin */
  GPIO_InitStruct.Pin = CO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CO2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */


	if(htim->Instance == TIM10){//타이머6 인터럽트 실행(1초)

	  HAL_GPIO_TogglePin(GPIOA, DotT_Pin); //세그먼트 시계 중앙 도트 점멸
	  DHT22_Loop_Time++; //DHT 센서 타이머 카운트
	  if(second >= 60){//60초 마다 세븐세그먼트(시계) 출력값 ++
		  Seg_Out++;
		  second = 0;
	  }
	  lcd++; //LCD 출력 시간 카운트
	  second++;
	  Uart_Loop_Time++;
	  Dust_time++;

	}
	if(htim->Instance == TIM11){//타이머6 인터럽트 실행(1ms)
		Loop_Count++;
		ms++;
		CO2ms++;
		DHT22_Elapsed_Time++;

		//checkms++;
		CO2_Pin_State = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);

		if (rising_check == 1 && CO2_Pin_State != OLD_CO2_Pin_State && CO2_Pin_State == 1){ //라이징 엣지
			CO2ms = 0;
			rising_time = CO2ms;
			rising_check = 0;
			falling_time = 0;
			falling_check = 1;
		}

		if (falling_check && CO2_Pin_State != OLD_CO2_Pin_State && CO2_Pin_State == 0){ //폴링 엣지
			falling_check = 0;
			falling_time = CO2ms;
			rerising_time = 0;
			rerising_check = 1;
		}

		if (rerising_check == 1 && CO2_Pin_State != OLD_CO2_Pin_State && CO2_Pin_State == 1){ //다음 라이징 엣지
			rerising_time = CO2ms;
			rerising_check = 0;
			rising_check = 1;

		}



		OLD_CO2_Pin_State = CO2_Pin_State;


		}
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
