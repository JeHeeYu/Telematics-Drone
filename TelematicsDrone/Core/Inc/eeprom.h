#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "main.h"
#include "usart.h"
#include "i2c.h"

#include "debug.h"
#include "common.h"

typedef union _Parser
{
	unsigned char byte[4];
	float f;
} Parser;

void EEPROMInit();
void AT24C08PageWrite(unsigned char page, unsigned char* data, unsigned char length);
void AT24C08PageRead(unsigned char page, unsigned char* data, unsigned char length);
void EPPIDGainWrite(unsigned char id, float pGain, float iGain, float dGain);
unsigned char EPPIDGainRead(unsigned char id, float* pGain, float* iGain, float* dGain);

#endif /* INC_EEPROM_H_ */
