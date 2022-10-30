#include "motor.h"


void TimerPulseSetting(unsigned int width)
{
	TIM5->CCR1 = width;
	TIM5->CCR2 = width;
	TIM5->CCR3 = width;
	TIM5->CCR4 = width;
}


