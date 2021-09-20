/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define A_Pin GPIO_PIN_3
#define A_GPIO_Port GPIOA
#define B_Pin GPIO_PIN_4
#define B_GPIO_Port GPIOA
#define C_Pin GPIO_PIN_5
#define C_GPIO_Port GPIOA
#define D_Pin GPIO_PIN_6
#define D_GPIO_Port GPIOA
#define E_Pin GPIO_PIN_7
#define E_GPIO_Port GPIOA
#define Dig1_Pin GPIO_PIN_0
#define Dig1_GPIO_Port GPIOB
#define Dig2_Pin GPIO_PIN_1
#define Dig2_GPIO_Port GPIOB
#define F_Pin GPIO_PIN_8
#define F_GPIO_Port GPIOA
#define G_Pin GPIO_PIN_9
#define G_GPIO_Port GPIOA
#define DotT_Pin GPIO_PIN_10
#define DotT_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define Dig3_Pin GPIO_PIN_3
#define Dig3_GPIO_Port GPIOB
#define Dig4_Pin GPIO_PIN_4
#define Dig4_GPIO_Port GPIOB
#define DHT22_Pin GPIO_PIN_3
#define DHT22_GPIO_Port GPIOH
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
