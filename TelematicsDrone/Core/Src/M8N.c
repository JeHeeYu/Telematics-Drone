#include <M8N.h>

extern uint8_t m8nRXData[36];
extern uint8_t m8nRXCpltFlag;

UBXMessageStruct posllh;

const unsigned char UBX_CFG_PRT[] = {
		0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00,
		0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79
};

const unsigned char UBX_CFG_MSG[] = {
		0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x02, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x13, 0xBE
};

const unsigned char UBX_CFG_RATE[] = {
		0xB5, 0x62, 0x06, 0x08, 0x00, 0xC8, 0x00, 0x01, 0x00,
		0x01, 0x00, 0xDE, 0x6A
};

const unsigned char UBX_CFG_CFG[] = {
		0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x31,
		0xBF
};


/**********************************************************************
 * M8NUART4Init
 * Description : UBX Message를 전송할 UART4번 Initialize
 * Parameter void
 * Return : void
***********************************************************************/
void M8NUART4Init()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	  LL_USART_InitTypeDef USART_InitStruct = {0};

	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* Peripheral clock enable */
	  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);

	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	  /**UART4 GPIO Configuration
	  PC10   ------> UART4_TX
	  PC11   ------> UART4_RX
	  */
	  GPIO_InitStruct.Pin = M8N_TX_Pin|M8N_RX_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  /* UART4 interrupt Init */
	  NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	  NVIC_EnableIRQ(UART4_IRQn);

	  /* USER CODE BEGIN UART4_Init 1 */

	  /* USER CODE END UART4_Init 1 */
	  USART_InitStruct.BaudRate = 9600;
	  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	  LL_USART_Init(UART4, &USART_InitStruct);
	  LL_USART_ConfigAsyncMode(UART4);
	  LL_USART_Enable(UART4);
	  /* USER CODE BEGIN UART4_Init 2 */

	  /* USER CODE END UART4_Init 2 */

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

void M8NInit()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	M8NUART4Init();

	M8NTransmitData(&UBX_CFG_PRT[0], sizeof(UBX_CFG_PRT));
	HAL_Delay(100);
	M8NTransmitData(&UBX_CFG_MSG[0], sizeof(UBX_CFG_MSG));
	HAL_Delay(100);
	M8NTransmitData(&UBX_CFG_RATE[0], sizeof(UBX_CFG_RATE));
	HAL_Delay(100);
	M8NTransmitData(&UBX_CFG_CFG[0], sizeof(UBX_CFG_CFG));

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

/**********************************************************************
 * M8NTransmitData
 * Description : UBX Message를 UART6번 채널로 전송
 * Parameter
 * 		data : UBX Message Data
 * 		length : UBX Message Length
 * Return : Checksum이 같을 경우 1, 아니면 0
***********************************************************************/
void M8NTransmitData(unsigned char* data, unsigned char length)
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	for(int i = 0; i < length; i++) {
		while(!LL_USART_IsActiveFlag_TXE(USART6));
		LL_USART_TransmitData8(USART6, *(data + i));
	}

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

/**********************************************************************
 * M8NGetUBXMessageSuccess
 * Description : UBX Message를 받고 있는지 확인하여 받고 있으면 데이터 저장
 * Parameter void
 * Return : void
***********************************************************************/
void M8NGetUBXMessageSuccess()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	if(m8nRXCpltFlag == HIGH) {
		m8nRXCpltFlag = 0;
		if(M8NUBXCheckSum(&m8nRXData[0], UBX_MESSAGE_LENGTH) == HIGH) {
			LL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
			M8NUBXMessageParsing(&m8nRXData[0], &posllh);

#if DEBUG
			printf("[%s] Lat : %ld, Lon : %ld, Height : %ld\n", __func__, posllh.lat, posllh.lon, posllh.height);
#endif
		}
#if DEBUG
		printf("[%s] Get UBX Message Success\n", __func__);
		LL_GPIO_ResetOutputPin(GPIOC, LED_1_Pin);
#endif

	}
	else {
		printf("[%s] BNO080 Data Available is False\n", __func__);
		LL_GPIO_SetOutputPin(GPIOC, LED_1_Pin);
	}

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

/**********************************************************************
 * M8NUBXCheckSum
 * Description : UBX Message에서 Checksum 확인
 * Parameter
 * 		data : UBX Message Data
 * 		length : UBX Message Length
 * Return : Checksum이 같을 경우 1, 아니면 0
***********************************************************************/
unsigned char M8NUBXCheckSum(unsigned char* data, unsigned char length)
{
#if DEBUG
	printf("[%s] Start!\n");
#endif

	unsigned char CK_A = 0, CK_B = 0;

	for(int i = 2; i < length - 2; i++) {
		CK_A = CK_A + data[i];
		CK_B = CK_B + CK_A;
	}

	return (CK_A == data[length - 2] && CK_B == data[length - 1]);

#if DEBUG
	printf("[%s] End!\n");
#endif
}

/**********************************************************************
 * M8NUBXMessageParsing
 * Description : UBX Message에서 필요한 데이터 저장
 * Parameter
 * 		data : UBX Message Data
 * 		posllh : UBX Message 데이터를 담을 구조체
 * Return : void
***********************************************************************/
void M8NUBXMessageParsing(unsigned char* data, UBXMessageStruct* posllh)
{
#if DEBUG
	printf("[%s] Start!\n");
#endif

	posllh->class = data[2];
	posllh->id = data[3];
	posllh->length = data[4] | data[5] << 8;

	posllh->iTOW = data[6] | data[7] << 8 | data[8] << 16 | data[9] << 24;
	posllh->lon = data[10] | data[11] << 8 | data[12] << 16 | data[13] << 24;
	posllh->lat = data[14] | data[15] << 8 | data[16] << 16 | data[17] << 24;
	posllh->height = data[18] | data[19] << 8 | data[20] << 16 | data[21] << 24;
	posllh->hMSL = data[22] | data[23] << 8 | data[24] << 16 | data[25] << 24;
	posllh->hAcc = data[26] | data[25] << 8 | data[26] << 16 | data[27] << 24;
	posllh->vAcc = data[30] | data[31] << 8 | data[32] << 16 | data[33] << 24;

//	posllh->lonf = posllh->lon / 10000000.;
//	posllh->latf = posllh->lat / 10000000.;

#if DEBUG
	printf("[%s] End!\n");
#endif
}
