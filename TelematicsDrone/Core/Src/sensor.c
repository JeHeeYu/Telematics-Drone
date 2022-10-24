#include "sensor.h"

float q[4];
float quatRadianAccuracy;

void GetBNO080Data()
{
	if(BNO080_dataAvailable() == 1) {
		q[0] = BNO080_getQuatI();
		q[1] = BNO080_getQuatJ();
		q[2] = BNO080_getQuatK();
		q[3] = BNO080_getQuatReal();
		quatRadianAccuracy = BNO080_getQuatRadianAccuracy();

		Quaternion_Update(&q[0]);

#if DEBUG
		printf("Roll : %d, Pitch : %d, Yaw : %d\n", (int)(BNO080_Roll * 100), (int)(BNO080_Pitch), (int)(BNO080_Yaw * 100));
		LL_GPIO_ResetOutputPin(GPIOC, LED_1_Pin);
#endif
	}

	else {
#if DEBUG
		printf("BNO080 Data Available is False\n");
		LL_GPIO_SetOutputPin(GPIOC, LED_1_Pin);
#endif
	}
}
