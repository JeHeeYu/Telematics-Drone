/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"

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
#define LED_1_Pin LL_GPIO_PIN_0
#define LED_1_GPIO_Port GPIOC
#define LED_2_Pin LL_GPIO_PIN_1
#define LED_2_GPIO_Port GPIOC
#define LED_3_Pin LL_GPIO_PIN_2
#define LED_3_GPIO_Port GPIOC
#define BUZZER_TIMER_3_Pin LL_GPIO_PIN_1
#define BUZZER_TIMER_3_GPIO_Port GPIOB
#define BNO080_CS_Pin LL_GPIO_PIN_12
#define BNO080_CS_GPIO_Port GPIOB
#define BNO080_SCK_Pin LL_GPIO_PIN_13
#define BNO080_SCK_GPIO_Port GPIOB
#define BNO080_MISO_Pin LL_GPIO_PIN_14
#define BNO080_MISO_GPIO_Port GPIOB
#define BNO080_MOSI_Pin LL_GPIO_PIN_15
#define BNO080_MOSI_GPIO_Port GPIOB
#define BNO080_INT_Pin LL_GPIO_PIN_8
#define BNO080_INT_GPIO_Port GPIOC
#define BNO080_RST_Pin LL_GPIO_PIN_9
#define BNO080_RST_GPIO_Port GPIOC
#define BNO080_WAKE_Pin LL_GPIO_PIN_8
#define BNO080_WAKE_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
