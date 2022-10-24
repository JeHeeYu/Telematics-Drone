#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "main.h"
#include "usart.h"
#include "time.h"

#include "debug.h"


int _write(int file, char* p, int len)
{
	for(int i = 0; i < len; i++) {
		LL_USART_TransmitData8(USART6, *(p + i));
		HAL_Delay(1);
	}

	return len;
}

void BuzzerOn(int flag)
{
	if(flag == 1) {
		LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	}
	else {
		LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	}
}
