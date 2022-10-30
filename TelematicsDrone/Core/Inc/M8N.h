#ifndef INC_M8N_H_
#define INC_M8N_H_

#include "main.h"
#include "usart.h"
#include "stm32f4xx_it.h"

#include "debug.h"
#include "common.h"


typedef struct _UBXMessageStruct
{
	// Clas ID
	unsigned char class;
	unsigned char id;
	unsigned short length;

	// Payload Data
	unsigned int iTOW;
	signed int lon;
	signed int lat;
	signed int height;
	signed int hMSL;
	unsigned int hAcc;
	unsigned int vAcc;

	// 실수형 GPS 정보
	double lonf;
	double latf;
} UBXMessageStruct;

extern UBXMessageStruct posllh;

void M8NUART4Init();
void M8NInit();
void M8NTransmitData(unsigned char* data, unsigned char length);
void M8NGetUBXMessageSuccess();
unsigned char M8NUBXCheckSum(unsigned char* data, unsigned char length);
void M8NUBXMessageParsing(unsigned char* data, UBXMessageStruct* posllh);

#endif /* INC_M8N_H_ */
