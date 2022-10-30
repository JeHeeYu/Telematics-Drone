/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "common.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t uart6RXFlag = 0;
uint8_t uart6RXData = 0;

uint8_t uart5RXFlag = 0;
uint8_t uart5RXData = 0;

uint8_t uart4RXFlag = 0;
uint8_t uart4RXData = 0;

uint8_t m8nRXData[36];
uint8_t m8nRXCpltFlag = 0;

uint8_t iBusRXData[32];
uint8_t iBusRXCpltFlag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */
	static unsigned char cnt = 0;

	if(LL_USART_IsActiveFlag_RXNE(UART4)) {

		LL_USART_ClearFlag_RXNE(UART4);
		uart4RXData = LL_USART_ReceiveData8(UART4);
		uart4RXFlag = 1;

		LL_USART_TransmitData8(USART6, uart4RXData);

		/* ?ÔøΩÔøΩ?ÔøΩÔøΩ?ÔøΩÔøΩ ?ÔøΩÔøΩ?ÔøΩÔøΩ?ÔøΩÔøΩ ?ÔøΩÔøΩÔø???
		 0 : SYNC_CHAR_1
		 1 : SYNC_CHAR_2
		 35 : Checksum
		 default : Class, ID, Payload, Length ?ÔøΩÔøΩ
		*/
		switch(cnt) {
		case 0:
			if(uart4RXData == SYNC_CHAR_1) {
				m8nRXData[cnt] = uart4RXData;
				cnt++;
			}
			else {
#if DEBUG
				printf("[%s] SYNC_CHAR_1 Data is Not Same : %d\n", __func__, uart4RXData);
#endif
			}
			break;
		case 1:
			if(uart4RXData == SYNC_CHAR_2) {
				m8nRXData[cnt] = uart4RXData;
				cnt++;
			}
			else {
#if DEBUG
				printf("[%s] SYNC_CHAR_2 Data is Not Same : %d\n", __func__, uart4RXData);
#endif
				cnt = 0;
			}
			break;
		case 35:
#if DEBUG
			printf("[%s] Get Data is Checksum : %d", __func__, uart4RXData);
#endif
			m8nRXData[cnt] = uart4RXData;
			cnt = 0;
			m8nRXCpltFlag = 1;
			break;

		default:
			m8nRXData[cnt] = uart4RXData;
			cnt++;
			break;
		}
	}

  /* USER CODE END UART4_IRQn 0 */
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */
	static unsigned char cnt = 0;

	if(LL_USART_IsActiveFlag_RXNE(UART5)) {

		LL_USART_ClearFlag_RXNE(UART5);
		uart5RXData = LL_USART_ReceiveData8(UART5);
		uart5RXFlag = 1;

		switch(cnt) {
		case 0:
			if(uart5RXData == 0x20) {
				iBusRXData[cnt] = uart5RXData;
				cnt++;
			}
			break;

		case 1:
			if(uart5RXData == 0x40) {
				iBusRXData[cnt] = uart5RXData;
				cnt++;
			}
			else {
				cnt = 0;
			}
			break;

		case 31:
			iBusRXData[cnt] = uart5RXData;
			cnt = 0;
			iBusRXCpltFlag = 1;
			break;

		default:
			iBusRXData[cnt] = uart5RXData;
			cnt++;
			break;
		}

		// ?ÔøΩÔøΩ?ÔøΩÔøΩ Î≤ÑÌçºÔø?? ÎπÑÏñ¥ ?ÔøΩÔøΩ?ÔøΩÔøΩÔø?? ?ÔøΩÔøΩ?ÔøΩÔøΩ
		while(!LL_USART_IsActiveFlag_TXE(USART6));
		LL_USART_TransmitData8(USART6, uart5RXData);
	}
  /* USER CODE END UART5_IRQn 0 */
  /* USER CODE BEGIN UART5_IRQn 1 */

  /* USER CODE END UART5_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */
	if(LL_USART_IsActiveFlag_RXNE(USART6)) {

		LL_USART_ClearFlag_RXNE(USART6);
		uart6RXData = LL_USART_ReceiveData8(USART6);
		uart6RXFlag = 1;

		// ?ÔøΩÔøΩ?ÔøΩÔøΩ Î≤ÑÌçºÔø?? ÎπÑÏñ¥ ?ÔøΩÔøΩ?ÔøΩÔøΩÔø?? ?ÔøΩÔøΩ?ÔøΩÔøΩ
//		while(!LL_USART_IsActiveFlag_TXE(USART6));
//		LL_USART_TransmitData8(UART4, uart6RXData);
	}
  /* USER CODE END USART6_IRQn 0 */
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
