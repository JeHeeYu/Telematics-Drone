#include <M8N.h>

extern uint8_t m8nRXData[36];
extern uint8_t m8nRXCpltFlag;

void M8NGetUBXMessageSuccess()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	if(m8nRXCpltFlag == 1) {
		m8nRXCpltFlag = 0;
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

	return (CK_A == data[len - 2] && CK_B == data[len - 1]);

#if DEBUG
	printf("[%s] End!\n");
#endif
}
