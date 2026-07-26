/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "config_gpio.h"

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
#define LED_1_Pin GPIO_PIN_13
#define LED_1_GPIO_Port GPIOC
#define GPO_8_Pin GPIO_PIN_0
#define GPO_8_GPIO_Port GPIOA
#define LED_2_Pin GPIO_PIN_1
#define LED_2_GPIO_Port GPIOA
#define GPO_4_Pin GPIO_PIN_12
#define GPO_4_GPIO_Port GPIOB
#define GPO_3_Pin GPIO_PIN_13
#define GPO_3_GPIO_Port GPIOB
#define GPO_2_Pin GPIO_PIN_14
#define GPO_2_GPIO_Port GPIOB
#define GPO_1_Pin GPIO_PIN_15
#define GPO_1_GPIO_Port GPIOB
#define RELAY_3_Pin GPIO_PIN_7
#define RELAY_3_GPIO_Port GPIOC
#define RELAY_2_Pin GPIO_PIN_8
#define RELAY_2_GPIO_Port GPIOC
#define RELAY_1_Pin GPIO_PIN_9
#define RELAY_1_GPIO_Port GPIOC
#define RELAY_4_Pin GPIO_PIN_10
#define RELAY_4_GPIO_Port GPIOC
#define GPO_7_Pin GPIO_PIN_3
#define GPO_7_GPIO_Port GPIOB
#define GPO_6_Pin GPIO_PIN_4
#define GPO_6_GPIO_Port GPIOB
#define GPO_5_Pin GPIO_PIN_5
#define GPO_5_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
