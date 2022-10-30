#include "main.h"
#include "usart.h"
#include "stm32f4xx_it.h"

#include "debug.h"
#include "common.h"

typedef struct _IBusMessageStruct
{
	unsigned short RH;
	unsigned short RV;
	unsigned short LV;
	unsigned short LH;
	unsigned short SwA;
	unsigned short SwB;
	unsigned short SwC;
	unsigned short SwD;
	unsigned short VrA;
	unsigned short VrB;

	unsigned char faileSafe;

}IBusMessageStruct;

extern IBusMessageStruct iBus;

unsigned char IBusCheckChecksum(unsigned char* data, unsigned char length);
void IBusDataParsing(unsigned char* data, IBusMessageStruct* ibus);
void FSIA6BUart5Init();
void GetIBusMessage();
unsigned char IBusIsActiveFailSafe(IBusMessageStruct* iBus);
