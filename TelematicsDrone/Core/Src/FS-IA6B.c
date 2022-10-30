#include "FS-IA6B.h"

IBusMessageStruct iBus;

extern uint8_t iBusRXData[32];
extern uint8_t iBusRXCpltFlag;

/**********************************************************************
 * IBusCheckChecksum
 * Description : i-bus Message에서 Checksum 확인
 * Parameter
 * 		data : i-bus Message Data
 * 		length : i-bus Message Length
 * Return : Checksum이 같을 경우 1, 아니면 0
***********************************************************************/
unsigned char IBusCheckChecksum(unsigned char* data, unsigned char length)
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	unsigned short checksum = 0xffff;

	for(int i = 0; i < length - 2; i++) {
		checksum = checksum - data[i];
	}

	return ((checksum & 0x00ff) == data[30]) && ((checksum >> 8) == data[31]);

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}


/**********************************************************************
 * IBusDataParsing
 * Description : i-bus Message 데이터 저장
 * Parameter
 * 		data : i-bus Message Data
 * 		i-bus : i-bus Message 데이터를 담을 구조체
 * Return : void
***********************************************************************/
void IBusDataParsing(unsigned char* data, IBusMessageStruct* iBus)
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	iBus->RH = (data[2] | data[3] << 8)  & (0xffff);
	iBus->RV = (data[4] | data[5] << 8)  & (0xffff);
	iBus->LV = (data[6] | data[7] << 8)  & (0xffff);
	iBus->LH = (data[8] | data[9] << 8)  & (0xffff);
	iBus->SwA = (data[10] | data[11] << 8)  & (0xffff);
	iBus->SwC = (data[12] | data[13] << 8)  & (0xffff);
	iBus->SwD = (data[14] | data[15] << 8)  & (0xffff);

	// Faile Safe
//	iBus->RH = (data[2] | data[3] << 8);
//	iBus->RV = (data[4] | data[5] << 8);
//	iBus->LV = (data[6] | data[7] << 8);
//	iBus->LH = (data[8] | data[9] << 8);
//	iBus->SwA = (data[10] | data[11] << 8);
//	iBus->SwB = (data[12] | data[13] << 8);

	iBus->faileSafe = (iBus->SwD == 1500);


#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}


/**********************************************************************
 * FSIA6BUart5Init
 * Description : i-bus Message를 전송할 UART5번 Initialize
 * Parameter void
 * Return : void
***********************************************************************/
void FSIA6BUart5Init()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	  LL_USART_InitTypeDef USART_InitStruct = {0};

	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* Peripheral clock enable */
	  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);

	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
	  /**UART5 GPIO Configuration
	  PC12   ------> UART5_TX
	  PD2   ------> UART5_RX
	  */
	  GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	  /* UART5 interrupt Init */
	  NVIC_SetPriority(UART5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	  NVIC_EnableIRQ(UART5_IRQn);

	  /* USER CODE BEGIN UART5_Init 1 */

	  /* USER CODE END UART5_Init 1 */
	  USART_InitStruct.BaudRate = 115200;
	  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_RX;
	  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	  LL_USART_Init(UART5, &USART_InitStruct);
	  LL_USART_ConfigAsyncMode(UART5);
	  LL_USART_Enable(UART5);

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

/**********************************************************************
 * GetIBusMessage
 * Description : I-Bus Interface로 얻은 Message 출력
 * Parameter : void
 * Return : 정상 수신 시 0, 아니면 1 Return
***********************************************************************/

int GetIBusMessage()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	if(iBusRXCpltFlag == 1) {
		iBusRXCpltFlag = 0;
		if(IBusCheckChecksum(&iBusRXData[0], IBUS_MESSAGE_LENGTH) == HIGH) {
			IBusDataParsing(iBusRXData[0], &iBus);
			if(iBus.LV < 1010) {
				return 1;
			}


#if DEBUG
			printf("[%s] RH : %d, RV : %d, LV : %d, LH : %d, SwA : %d, SwC : %d, SwD : %d, failSafe : %d\n", __func__, iBus.RH, iBus.RV, iBus.LV, iBus.LH, iBus.SwA, iBus.SwC, iBus.SwD, iBus.faileSafe);
#endif
		}
	}

	else {
#if DEBUG
		printf("[%s] IBus is Not Ready\n", __func__);
		LL_GPIO_SetOutputPin(GPIOC, LED_1_Pin);
#endif
	}

	return 0;

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

/**********************************************************************
 * IBusIsActiveFailSafe
 * Description : i-bus Faile safe 발동 여부
 * Parameter iBus : Faile Safe를 확인할 iBus 구조체
 * Return : File Safe가 발동 되었을 때 1 리턴
***********************************************************************/
unsigned char IBusIsActiveFailSafe(IBusMessageStruct* iBus)
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	return iBus->faileSafe != 0;

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}


/**********************************************************************
 * IsIBusReceived
 * Description : i-bus 데이터가 수신 되었는지 확인
 * Parameter : void
 * Return : 데이터가 정상적으로 수신 되었을 경우 1, 아니면 0
***********************************************************************/
int IsIBusReceived()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	if(iBusRXCpltFlag == HIGH) {
		iBusRXCpltFlag = 0;
		if(IBusCheckChecksum(&iBusRXData[0], IBUS_MESSAGE_LENGTH) == HIGH) {
			IBusDataParsing(&iBusRXData[0], &iBus);

			return 1;
		}
	}

	return 0;

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}
