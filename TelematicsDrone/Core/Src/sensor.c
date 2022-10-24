#include "sensor.h"

float q[4];
float quatRadianAccuracy;

void SensorInit()
{
	  BNO080_Initialization();
	  BNO080_enableRotationVector(BNO080_ROTATION_VECTOR);

	  ICM20602_Initialization();
}

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
		printf("[%s] Roll : %d, Pitch : %d, Yaw : %d\n", __func__, (int)BNO080_Roll * 100, (int)BNO080_Pitch, (int)BNO080_Yaw * 100);
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

void GetICM20602Data()
{
	if(ICM20602_DataReady() == 1) {
		ICM20602_Get3AxisGyroRawData(&ICM20602.gyro_x_raw);

		ICM20602.gyro_x = ICM20602.gyro_x_raw * 2000 / 32768.f;
		ICM20602.gyro_y = ICM20602.gyro_y_raw * 2000 / 32768.f;
		ICM20602.gyro_z = ICM20602.gyro_z_raw * 2000 / 32768.f;

#if DEBUG
		printf("[%s] x : %d, y : %d, z : %d\n", __func__, (int)ICM20602.gyro_x, (int)ICM20602.gyro_y, (int)ICM20602.gyro_z);
		LL_GPIO_ResetOutputPin(GPIOC, LED_1_Pin);
#endif
	}

	else {
#if DEBUG
		printf("ICM20602 is Not Ready");
		LL_GPIO_SetOutputPin(GPIOC, LED_1_Pin);
#endif
	}
}
