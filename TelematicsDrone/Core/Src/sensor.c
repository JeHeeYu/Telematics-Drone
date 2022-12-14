#include "sensor.h"

float q[4];
float quatRadianAccuracy;

short gyroXOffset = 3;
short gyroYOffset = -25;
short gyroZOffset = -3;

/**********************************************************************
 * SensorInit
 * Description : 센서 Initialize
 * Parameter : void
 * Return : void
***********************************************************************/

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

/**********************************************************************
 * GetBNO080Data
 * Description : BNO080 센서 데이터 Get
 * Parameter : void
 * Return : void
***********************************************************************/

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
		printf("[%s] Roll : %.2f, Pitch : %.2f, Yaw : %.2f\n", __func__, BNO080_Roll, BNO080_Pitch, BNO080_Yaw * 100);
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

/**********************************************************************
 * GetICM20602Data
 * Description : ICM20602 센서 데이터 Get
 * Parameter : void
 * Return : void
***********************************************************************/

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

/**********************************************************************
 * ICM20602GyroOffset
 * Description : ICM20602 x, y, z 값 Offset 추가
 * Parameter : void
 * Return : void
***********************************************************************/

void ICM20602GyroOffset()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	ICM20602_Writebyte(0x13, (gyroXOffset * -2) >> 8);
	ICM20602_Writebyte(0x14, (gyroXOffset * -2));

	ICM20602_Writebyte(0x15, (gyroYOffset * -2) >> 8);
	ICM20602_Writebyte(0x16, (gyroYOffset * -2));

	ICM20602_Writebyte(0x17, (gyroZOffset * -2) >> 8);
	ICM20602_Writebyte(0x18, (gyroZOffset * -2));

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

/**********************************************************************
 * GetLPS22HHData
 * Description : LPS22HH 센서 데이터 Get
 * Parameter : void
 * Return : void
***********************************************************************/

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
