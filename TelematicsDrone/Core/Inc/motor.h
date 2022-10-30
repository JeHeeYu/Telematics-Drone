#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"
#include "usart.h"
#include "stm32f4xx_it.h"

#include "common.h"
#include "debug.h"


void TimerPulseSetting(unsigned int width);
void TimerPulseCalculate(unsigned int width);

#endif /* INC_MOTOR_H_ */
