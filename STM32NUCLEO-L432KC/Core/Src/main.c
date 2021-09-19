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
  Nucleo-L432KC <--> RaspberryPi
			    Uart2


  Atmega328P-PU <--> Nucleo-L432KC
			    Uart1


  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
int Seg_Out = 5279; //세그먼트에 표시될 숫자
uint8_t rx1_data;
uint8_t buff[10];//uart 입력 버퍼
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM6_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int buffcount = 9;
char T[5] = "000000";
void Check_Buff(void){
	for(int i = 9; i >= 0; i--){
		if (buff[i] == 'T'){
			if(buff[i-1] == ':'){
				if('0' < buff[i-2] && buff[i-2] < '9'){
					T[0] = buff[i-2];
				}
			}

		}
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if(huart->Instance == USART1){
		HAL_UART_Receive_IT(&huart1, &rx1_data, 1);
		buff[buffcount] = rx1_data;
		buffcount--;

		if (rx1_data == ','){
			Check_Buff();

		};

		if (buffcount == 0){
			buffcount = 9;
		}
	    HAL_UART_Transmit(&huart2, &rx1_data, 1, 10);

	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//타이머 인터럽트
{

  if(htim->Instance == TIM6){//타이머6 인터럽트 실행(1초)
	  HAL_GPIO_TogglePin(GPIOB, DotT_Pin);
	  HAL_GPIO_TogglePin(GPIOB, DotB_Pin);

//	  Seg_Out++;

  }

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}

void Seg_Off(void){

	HAL_GPIO_WritePin(GPIOA, DP_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, G_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, F_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, E_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, D_Pin, 0);
	HAL_GPIO_WritePin(GPIOA, C_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, B_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, A_Pin, 0);





}
void Num_Select(unsigned char PrintNumx16) {

	if (PrintNumx16 & 0x40) {
		HAL_GPIO_WritePin(GPIOA, G_Pin, 0);
	} else {
		HAL_GPIO_WritePin(GPIOA, G_Pin, 1);
	}
	if (PrintNumx16 & 0x20) {
		HAL_GPIO_WritePin(GPIOB, F_Pin, 0);
	} else {
		HAL_GPIO_WritePin(GPIOB, F_Pin, 1);
	}
	if (PrintNumx16 & 0x10) {
		HAL_GPIO_WritePin(GPIOA, E_Pin, 0);
	} else {
		HAL_GPIO_WritePin(GPIOA, E_Pin, 1);
	}
	if (PrintNumx16 & 0x08) {
		HAL_GPIO_WritePin(GPIOA, D_Pin, 0);
	} else {
		HAL_GPIO_WritePin(GPIOA, D_Pin, 1);
	}
	if (PrintNumx16 & 0x04) {
		HAL_GPIO_WritePin(GPIOA, C_Pin, 0);
	} else {
		HAL_GPIO_WritePin(GPIOA, C_Pin, 1);
	}
	if (PrintNumx16 & 0x02) {
		HAL_GPIO_WritePin(GPIOB, B_Pin, 0);
	} else {
		HAL_GPIO_WritePin(GPIOB, B_Pin, 1);
	}
	if (PrintNumx16 & 0x01) {
		HAL_GPIO_WritePin(GPIOB, A_Pin, 0);
	} else {
		HAL_GPIO_WritePin(GPIOB, A_Pin, 1);
	}

}

void Segment_Select(unsigned char SegmentNum, unsigned char PrintNumx16) {
	//출력할 세그먼트 결정

	Seg_Off();

	HAL_GPIO_WritePin(GPIOB, Dig1_Pin, 1);
	HAL_GPIO_WritePin(GPIOA, Dig2_Pin, 1);
	HAL_GPIO_WritePin(GPIOA, Dig3_Pin, 1);
	HAL_GPIO_WritePin(GPIOA, Dig4_Pin, 1);



	if (SegmentNum == 0) {
		HAL_GPIO_WritePin(GPIOB, Dig1_Pin, 0);
	}
	else if (SegmentNum == 1) {
		HAL_GPIO_WritePin(GPIOA, Dig2_Pin, 0);
	}
	else if (SegmentNum == 2) {
		HAL_GPIO_WritePin(GPIOA, Dig3_Pin, 0);
	}
	else //if (SegmentNum == 3)
	{
		HAL_GPIO_WritePin(GPIOA, Dig4_Pin, 0);
	}
	Num_Select(PrintNumx16);

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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);//타이머 인터럽트 시작
  HAL_UART_Receive_IT(&huart1, &rx1_data, 1);//uart1 인터럽트실행을 위한 기능
  unsigned char List_Of_Segment_Info[10] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99,
 		  0x92, 0x82, 0xD8, 0x80, 0x98 };
  int addr[4];
  HAL_GPIO_WritePin(GPIOB, DotB_Pin, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


	  addr[0] = Seg_Out / 1000;
	  addr[1] = Seg_Out % 1000 / 100;
	  addr[2] = Seg_Out % 100 / 10;
	  addr[3] = Seg_Out % 10;

	  for (int i = 0; i<4; i++){
		  Segment_Select(i, List_Of_Segment_Info[addr[i]]);
		  HAL_Delay(1);
	  }

	  //Uart출력(채널, 출력변수주소, 변수크기, 타임아웃)


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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 9999;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 3199;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Dig4_Pin|G_Pin|C_Pin|DP_Pin
                          |D_Pin|E_Pin|Dig2_Pin|Dig3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, F_Pin|DotB_Pin|DotT_Pin|B_Pin
                          |A_Pin|Dig1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Dig4_Pin G_Pin C_Pin DP_Pin
                           D_Pin E_Pin Dig2_Pin Dig3_Pin */
  GPIO_InitStruct.Pin = Dig4_Pin|G_Pin|C_Pin|DP_Pin
                          |D_Pin|E_Pin|Dig2_Pin|Dig3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : F_Pin DotB_Pin DotT_Pin B_Pin
                           A_Pin Dig1_Pin */
  GPIO_InitStruct.Pin = F_Pin|DotB_Pin|DotT_Pin|B_Pin
                          |A_Pin|Dig1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
