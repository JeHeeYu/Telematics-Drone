
#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#include "main.h"

#include "BNO080.h"
#include "Quaternion.h"
#include "ICM20602.h"

#include "debug.h"
#include "common.h"

extern float q[4];
extern float quatRadianAccuracy;

void SensorInit();
void GetBNO080Data();
void GetICM20602Data();

#endif /* INC_SENSOR_H_ */
