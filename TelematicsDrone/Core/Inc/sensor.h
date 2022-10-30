
#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "main.h"

#include "BNO080.h"
#include "Quaternion.h"
#include "ICM20602.h"
#include "LPS22HH.h"

#include "debug.h"
#include "common.h"

#define IIR_FILETER_VALUE	0.90f

extern float q[4];
extern float quatRadianAccuracy;

void SensorInit();
void GetBNO080Data();
void GetICM20602Data();
void ICM20602GyroOffset();
void GetLPS22HHData();

#endif /* INC_SENSOR_H_ */
