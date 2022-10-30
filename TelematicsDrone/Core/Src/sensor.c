#include "sensor.h"

float q[4];
float quatRadianAccuracy;

void SensorInit()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	// BNO080 Sensor Init
	BNO080_Initialization();
	BNO080_enableRotationVector(BNO080_ROTATION_VECTOR);

	// ICM20602 Sensor Init
	ICM20602_Initialization();

	// LPS2HH Sensor Init
	LPS22HH_Initialization();

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

void GetBNO080Data()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	if(BNO080_dataAvailable() == HIGH) {
		q[0] = BNO080_getQuatI();
		q[1] = BNO080_getQuatJ();
		q[2] = BNO080_getQuatK();
		q[3] = BNO080_getQuatReal();
		quatRadianAccuracy = BNO080_getQuatRadianAccuracy();

		Quaternion_Update(&q[0]);

#if DEBUG
		printf("[%s] Roll : %d, Pitch : %d, Yaw : %d\n", __func__, (int)(BNO080_Roll * 100), (int)(BNO080_Pitch), (int)(BNO080_Yaw * 100));
		LL_GPIO_ResetOutputPin(GPIOC, LED_1_Pin);
#endif
	}

	else {
#if DEBUG
		printf("BNO080 Data Available is False\n");
		LL_GPIO_SetOutputPin(GPIOC, LED_1_Pin);
#endif
	}

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

void GetICM20602Data()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	if(ICM20602_DataReady() == HIGH) {
		ICM20602_Get3AxisGyroRawData(&ICM20602.gyro_x_raw);

		ICM20602.gyro_x = ICM20602.gyro_x_raw * 2000 / 32768.f;
		ICM20602.gyro_y = ICM20602.gyro_y_raw * 2000 / 32768.f;
		ICM20602.gyro_z = ICM20602.gyro_z_raw * 2000 / 32768.f;

#if DEBUG
		printf("[%s] x : %d, y : %d, z : %d\n", __func__, (int)(ICM20602.gyro_x), (int)(ICM20602.gyro_y), (int)(ICM20602.gyro_z));
		LL_GPIO_ResetOutputPin(GPIOC, LED_1_Pin);
#endif
	}

	else {
#if DEBUG
		printf("ICM20602 is Not Ready\n");
		LL_GPIO_SetOutputPin(GPIOC, LED_1_Pin);
#endif
	}

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

void GetLPS22HHData()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	if(LPS22HH_DataReady() == HIGH) {
		LPS22HH_GetPressure(&LPS22HH.pressure_raw);
		LPS22HH_GetTemperature(&LPS22HH.temperature_raw);

		// 필터링 및 온도 보정 적용된 값
		LPS22HH.baroAlt = getAltitude2(LPS22HH.pressure_raw / 4096.f, LPS22HH.temperature_raw / 100.f);

		// IIR Filter 적용
		LPS22HH.baroAltFilt = (LPS22HH.baroAltFilt * IIR_FILETER_VALUE) + (LPS22HH.baroAlt * (1.0f - IIR_FILETER_VALUE));

#if DEBUG
		printf("[%s] %d cm, %d cm\n", __func__, (int)(LPS22HH.baroAlt * 100), (int)(LPS22HH.baroAltFilt * 100));
		LL_GPIO_ResetOutputPin(GPIOC, LED_1_Pin);
#endif
	}

	else {
#if DEBUG
		printf("LPS22HH is Not Ready\n");
		LL_GPIO_SetOutputPin(GPIOC, LED_1_Pin);
#endif
	}

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}
