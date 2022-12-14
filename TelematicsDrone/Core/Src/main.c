/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>

#include "common.h"
#include "sensor.h"
#include "M8N.h"
#include "FS-IA6B.h"
#include "motor.h"
#include "eeprom.h"
#include "PID control.h"

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

/* USER CODE BEGIN PV */
extern IBusMessageStruct iBus;

extern uint8_t uart1RXData;
uint8_t telemetryTXData[40];
uint8_t telemetryRXData[40];
uint8_t telemetryRXCpltFlag;

extern timer7Flag;
extern timer7GlobalFlag;

unsigned char motorFlag = 0;

unsigned short ccr1, ccr2, ccr3, ccr4;
unsigned short iBusSwAPrev = 0;

float batteryVolt;

float yaw_heading_reference;

uint8_t timer71msFlag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void BNO080_Calibration(void);
void EncodeMessage(unsigned char* telemetryTxData);
void EncodeMessageAHRS(unsigned char* telemetryTxData);
void EncodeMessagePIDGain(unsigned char* telemetryTxData, unsigned char id, float p, float i, float d);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	MX_DMA_Init();
	MX_TIM3_Init();
	MX_USART6_UART_Init();
	MX_SPI2_Init();
	MX_SPI1_Init();
	MX_SPI3_Init();
	MX_UART4_Init();
	MX_UART5_Init();
	MX_TIM5_Init();
	MX_I2C1_Init();
	MX_ADC1_Init();
	MX_USART1_UART_Init();
	MX_TIM7_Init();
	/* USER CODE BEGIN 2 */

	// EEPROM Init
	//EEPROMInit();

	// Buzzer Timer Init
	LL_TIM_EnableCounter(TIM3);

	// Motor ESC Timer Init
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH2);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH4);

	// Battery ADC Check
	HAL_ADC_Start_DMA(&hadc1, &batteryVolt, 1);

	// Telemetry UART
	HAL_UART_Receive_IT(&huart1, &uart1RXData, 1);

	// Global Timer
	LL_TIM_EnableCounter(TIM7);
	LL_TIM_EnableIT_UPDATE(TIM7);

	// Debug USART Init
	LL_USART_EnableIT_RXNE(USART6);

	// M8N UART Init
	LL_USART_EnableIT_RXNE(UART4);

	// FS-IA6B UART Init
	LL_USART_EnableIT_RXNE(UART5);

	SensorInit();
	M8NInit();
	FSIA6BUart5Init();

	ICM20602GyroOffset();

	if(BNO080_Initialization() != LOW) {
		BuzzerOn(HIGH);

		TIM3->PSC = 1000;
		HAL_Delay(100);
		TIM3->PSC = 1500;
		HAL_Delay(100);
		TIM3->PSC = 2000;
		HAL_Delay(100);
		BuzzerOn(LOW);
	}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		if(timer71msFlag == HIGH) {
			timer71msFlag = LOW;

			// PID Cal
			Double_Roll_Pitch_PID_Calculation(&pitch, (iBus.RV - 1500) * 0.1f, BNO080_Pitch, ICM20602.gyro_x);
			Double_Roll_Pitch_PID_Calculation(&roll, (iBus.RH - 1500) * 0.1f, BNO080_Roll, ICM20602.gyro_y);

			// PID Reset
			if(iBus.LV < 1030 || motorFlag == LOW) {
				Reset_All_PID_Integrator();
			}

			if(iBus.LH < 1485 || iBus.LH > 1515) {

				yaw_heading_reference = BNO080_Yaw;
				Single_Yaw_Rate_PID_Calculation(&yaw_rate, (iBus.LH - 1500), ICM20602.gyro_z_raw);

				// Timer CCR Resgister Cal
				ccr1 = 10500 + 500 + (iBus.LV - 1000) * 10 - pitch.in.pid_result + roll.in.pid_result - yaw_rate.pid_result;
				ccr2 = 10500 + 500 + (iBus.LV - 1000) * 10 - pitch.in.pid_result + roll.in.pid_result - yaw_rate.pid_result;
				ccr3 = 10500 + 500 + (iBus.LV - 1000) * 10 - pitch.in.pid_result + roll.in.pid_result - yaw_rate.pid_result;
				ccr4 = 10500 + 500 + (iBus.LV - 1000) * 10 - pitch.in.pid_result + roll.in.pid_result - yaw_rate.pid_result;
			}

			// Axis Cal
			else {
				Single_Yaw_Heading_PID_Calculation(&yaw_heading, yaw_heading_reference, BNO080_Yaw, ICM20602.gyro_z);

				ccr1 = 10500 + 500 + (iBus.LV - 1000) * 10 - pitch.in.pid_result + roll.in.pid_result - yaw_heading.pid_result;
				ccr2 = 10500 + 500 + (iBus.LV - 1000) * 10 - pitch.in.pid_result + roll.in.pid_result - yaw_heading.pid_result;
				ccr3 = 10500 + 500 + (iBus.LV - 1000) * 10 - pitch.in.pid_result + roll.in.pid_result - yaw_heading.pid_result;
				ccr4 = 10500 + 500 + (iBus.LV - 1000) * 10 - pitch.in.pid_result + roll.in.pid_result - yaw_heading.pid_result;
			}

		}

		if(iBus.SwA == 2000 && iBusSwAPrev != 2000) {
			if(iBus.LV < 1010) {
				motorFlag = HIGH;
				yaw_heading_reference = BNO080_Yaw;
			}
			else {
				while(IsIBusReceived() == LOW || iBus.SwA == 2000) {
					BuzzerOn(HIGH);

					TIM3->PSC = 1000;
					HAL_Delay(70);
					BuzzerOn(LOW);
					HAL_Delay(70);
				}
			}
		}

		// Get Sensor Data
		GetBNO080Data();
		GetICM20602Data();
		GetLPS22HHData();
		M8NGetUBXMessageSuccess();

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
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
void BNO080_Calibration(void)
{
	//Resets BNO080 to disable All output
	BNO080_Initialization();

	//BNO080/BNO085 Configuration
	//Enable dynamic calibration for accelerometer, gyroscope, and magnetometer
	//Enable Game Rotation Vector output
	//Enable Magnetic Field output
	BNO080_calibrateAll(); //Turn on cal for Accel, Gyro, and Mag
	BNO080_enableGameRotationVector(20000); //Send data update every 20ms (50Hz)
	BNO080_enableMagnetometer(20000); //Send data update every 20ms (50Hz)

	//Once magnetic field is 2 or 3, run the Save DCD Now command
	printf("Calibrating BNO080. Pull up FS-i6 SWC to end calibration and save to flash\n");
	printf("Output in form x, y, z, in uTesla\n\n");

	//while loop for calibration procedure
	//Iterates until iBus.SwC is mid point (1500)
	//Calibration procedure should be done while this loop is in iteration.
	while(iBus.SwC == 1500)
	{
		if(BNO080_dataAvailable() == 1)
		{
			//Observing the status bit of the magnetic field output
			float x = BNO080_getMagX();
			float y = BNO080_getMagY();
			float z = BNO080_getMagZ();
			unsigned char accuracy = BNO080_getMagAccuracy();

			float quatI = BNO080_getQuatI();
			float quatJ = BNO080_getQuatJ();
			float quatK = BNO080_getQuatK();
			float quatReal = BNO080_getQuatReal();
			unsigned char sensorAccuracy = BNO080_getQuatAccuracy();

			printf("%f,%f,%f,", x, y, z);
			if (accuracy == 0) printf("Unreliable\t");
			else if (accuracy == 1) printf("Low\t");
			else if (accuracy == 2) printf("Medium\t");
			else if (accuracy == 3) printf("High\t");

			printf("\t%f,%f,%f,%f,", quatI, quatI, quatI, quatReal);
			if (sensorAccuracy == 0) printf("Unreliable\n");
			else if (sensorAccuracy == 1) printf("Low\n");
			else if (sensorAccuracy == 2) printf("Medium\n");
			else if (sensorAccuracy == 3) printf("High\n");

			//Turn the LED and buzzer on when both accuracy and sensorAccuracy is high
			if(accuracy == 3 && sensorAccuracy == 3)
			{
				LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2);
				TIM3->PSC = 65000; //Very low frequency
				LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
			}
			else
			{
				LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2);
				LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
			}
		}

		IsIBusReceived(); //Refreshes iBus Data for iBus.SwC
		HAL_Delay(100);
	}

	//Ends the loop when iBus.SwC is not mid point
	//Turn the LED and buzzer off
	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2);
	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

	//Saves the current dynamic calibration data (DCD) to memory
	//Sends command to get the latest calibration status
	BNO080_saveCalibration();
	BNO080_requestCalibrationStatus();

	//Wait for calibration response, timeout if no response
	int counter = 100;
	while(1)
	{
		if(--counter == 0) break;
		if(BNO080_dataAvailable())
		{
			//The IMU can report many different things. We must wait
			//for the ME Calibration Response Status byte to go to zero
			if(BNO080_calibrationComplete() == 1)
			{
				printf("\nCalibration data successfully stored\n");
				LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
				TIM3->PSC = 2000;
				HAL_Delay(300);
				TIM3->PSC = 1500;
				HAL_Delay(300);
				LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
				HAL_Delay(1000);
				break;
			}
		}
		HAL_Delay(10);
	}
	if(counter == 0)
	{
		printf("\nCalibration data failed to store. Please try again.\n");
		LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
		TIM3->PSC = 1500;
		HAL_Delay(300);
		TIM3->PSC = 2000;
		HAL_Delay(300);
		LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
		HAL_Delay(1000);
	}

	//BNO080_endCalibration(); //Turns off all calibration
	//In general, calibration should be left on at all times. The BNO080
	//auto-calibrates and auto-records cal data roughly every 5 minutes

	//Resets BNO080 to disable Game Rotation Vector and Magnetometer
	//Enables Rotation Vector
	BNO080_Initialization();
	BNO080_enableRotationVector(2500); //Send data update every 2.5ms (400Hz)
}

// Telematric Receive
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static unsigned char cnt = 0;

	if(huart->Instance == USART1) {
		HAL_UART_Receive_IT(&huart1, &uart1RXData, 1);

		switch(cnt) {
		case 0:
			if(uart1RXData == 0x47) {
				telemetryTXData[cnt] = uart1RXData;
				cnt++;
			}
			break;

		case 1:
			if(uart1RXData == 0x53) {
				telemetryTXData[cnt] = uart1RXData;
				cnt++;
			}
			else {
				cnt = 0;
			}
			break;

		case 19:
			telemetryTXData[cnt] = uart1RXData;
			cnt = 0;
			telemetryRXCpltFlag = 1;
			break;

		default:
			telemetryTXData[cnt] = uart1RXData;
			cnt++;
			break;
		}
	}

}

void EncodeMessageAHRS(unsigned char* telemetryTxData)
{
	// SYNC_CHAR Header Data
	telemetryTxData[0] = 0x46;
	telemetryTxData[1] = 0x43;

	// ID
	telemetryTxData[2] = 0x10;

	// BNO080 Data
	telemetryTxData[3] = (short)(BNO080_Roll * 100);
	telemetryTxData[4] = ((short)(BNO080_Roll * 100)) >> 8;

	telemetryTxData[5] = (short)(BNO080_Pitch * 100);
	telemetryTxData[6] = ((short)(BNO080_Pitch * 100)) >> 8;

	telemetryTxData[7] = (unsigned short)(BNO080_Yaw * 100);
	telemetryTxData[8] = ((unsigned short)(BNO080_Yaw * 100)) >> 8;

	telemetryTxData[9] = (short)(LPS22HH.baroAltFilt * 10);
	telemetryTxData[10] = ((short)(LPS22HH.baroAltFilt * 10)) >> 8;

	telemetryTxData[11] = (short)((iBus.RH - 1500) * 0.1f * 100);
	telemetryTxData[12] = ((short)((iBus.RH - 1500) * 0.1f * 100)) >> 8;

	telemetryTxData[13] = (short)((iBus.RV - 1500) * 0.1f * 100);
	telemetryTxData[14] = ((short)((iBus.RV - 1500) * 0.1f * 100)) >> 8;

	telemetryTxData[15] = (unsigned short)((iBus.LH - 1000) * 0.36f * 100);
	telemetryTxData[16] = ((unsigned short)((iBus.LH - 1000) * 0.36f * 100)) >> 8;

	telemetryTxData[17] = 0x00;
	telemetryTxData[18] = 0x00;

	telemetryTxData[19] = 0xff;

	for(int i = 0; i < 19; i++) {
		telemetryTxData[19] = telemetryTxData[19] - telemetryTxData[i];
	}
}

void EncodeMessageGPS(unsigned char* telemetryTxData)
{
	telemetryTxData[0] = 0x46;
	telemetryTxData[1] = 0x43;

	telemetryTxData[2] = 0x11;

	// GPS - Lat
	telemetryTxData[3] = posllh.lat;
	telemetryTxData[4] = posllh.lat >> 8;
	telemetryTxData[5] = posllh.lat >> 16;
	telemetryTxData[6] = posllh.lat >> 24;

	// GPS - Lon
	telemetryTxData[7] = posllh.lon;
	telemetryTxData[8] = posllh.lon >> 8;
	telemetryTxData[9] = posllh.lon >> 16;
	telemetryTxData[10] = posllh.lon >> 24;

	telemetryTxData[11] = (unsigned short)(batteryVolt * 100);
	telemetryTxData[12] = ((unsigned short)(batteryVolt * 100)) >> 8;

	telemetryTxData[13] = (iBus.SwA == 1000) ? 0 : 1;
	telemetryTxData[14] = (iBus.SwC == 1000) ? 0 : (iBus.SwC== 1500) ? 1 : 2;

	telemetryTxData[15] = IBusIsActiveFailSafe(&iBus);

	telemetryTxData[16] = 0x00;
	telemetryTxData[17] = 0x00;
	telemetryTxData[18] = 0x00;

	telemetryTxData[19] = 0xff;

	for(int i = 0; i < 19; i++) {
		telemetryTxData[19] = telemetryTxData[19] - telemetryTxData[i];
	}
}

void EncodeMessagePIDGain(unsigned char* telemetryTxData, unsigned char id, float p, float i, float d)
{
	telemetryTxData[0] = 0x46;
	telemetryTxData[1] = 0x43;

	telemetryTxData[2] = id;

	memcpy(&telemetryTxData[3], &p, 4);
	memcpy(&telemetryTxData[7], &i, 4);
	memcpy(&telemetryTxData[11], &d, 4);

	telemetryTxData[15] = 0x00;
	telemetryTxData[16] = 0x00;
	telemetryTxData[17] = 0x00;
	telemetryTxData[18] = 0x00;

	telemetryTxData[19] = 0xff;

	for(int i = 0; i < 19; i++) {
		telemetryTxData[19] = telemetryTxData[19] - telemetryTxData[i];
	}
}
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
