#include "eeprom.h"

void EEPROMInit()
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif


#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

void AT24C08PageWrite(unsigned char page, unsigned char* data, unsigned char length)
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	unsigned char deviceAddress = (((page * 16) >> 8) << 1) | (0xA0);
	unsigned char wordAddress = (page * 16) & 0xff;

	LL_GPIO_ResetOutputPin(AT24C08_WP_GPIO_Port, AT24C08_WP_Pin);
	HAL_I2C_Mem_Write(&hi2c1, deviceAddress, wordAddress, I2C_MEMADD_SIZE_8BIT, &data[0], sizeof(data), 1);
	HAL_Delay(1);
	LL_GPIO_SetOutputPin(AT24C08_WP_GPIO_Port, AT24C08_WP_Pin);


#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

void AT24C08PageRead(unsigned char page, unsigned char* data, unsigned char length)
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	unsigned char deviceAddress = (((page * 16) >> 8) << 1) | (0xA0);
	unsigned char wordAddress = (page * 16) & 0xff;

	HAL_I2C_Mem_Read(&hi2c1, deviceAddress, wordAddress, I2C_MEMADD_SIZE_8BIT, &data[0], sizeof(data), 1);

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

void EPPIDGainWrite(unsigned char id, float pGain, float iGain, float dGain)
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	unsigned char writeData[16];

	Parser parser;

	writeData[0] = 0x45;
	writeData[1] = 0x50;
	writeData[2] = id;

	parser.f = pGain;
	writeData[3] = parser.byte[0];
	writeData[4] = parser.byte[1];
	writeData[5] = parser.byte[2];
	writeData[6] = parser.byte[3];

	parser.f = iGain;
	writeData[7] = parser.byte[0];
	writeData[8] = parser.byte[1];
	writeData[9] = parser.byte[2];
	writeData[10] = parser.byte[3];

	parser.f = dGain;
	writeData[11] = parser.byte[0];
	writeData[12] = parser.byte[1];
	writeData[13] = parser.byte[2];
	writeData[14] = parser.byte[3];

	unsigned char checksum = 0xff;

	for(int i = 0; i < 15; i++) {
		checksum -= writeData[i];
	}

	writeData[15] = checksum;

	switch(id) {
	case 0:
		AT24C08PageWrite(0, &writeData[0], 16);
	case 1:
		AT24C08PageWrite(1, &writeData[0], 16);
		break;
	case 2:
		AT24C08PageWrite(2, &writeData[0], 16);
		break;
	case 3:
		AT24C08PageWrite(3, &writeData[0], 16);
		break;
	case 4:
		AT24C08PageWrite(4, &writeData[0], 16);
		break;
	case 5:
		AT24C08PageWrite(5, &writeData[0], 16);
		break;
	default:
		break;
	}

#if DEBUG
	printf("[%s] End!\n", __func__);
#endif
}

unsigned char EPPIDGainRead(unsigned char id, float* pGain, float* iGain, float* dGain)
{
#if DEBUG
	printf("[%s] Start!\n", __func__);
#endif

	unsigned char readData[16];
	Parser parser;

	switch(id) {
	case 0:
		AT24C08PageRead(0, &readData[0], 16);
	case 1:
		AT24C08PageRead(1, &readData[0], 16);
		break;
	case 2:
		AT24C08PageRead(2, &readData[0], 16);
		break;
	case 3:
		AT24C08PageRead(3, &readData[0], 16);
		break;
	case 4:
		AT24C08PageRead(4, &readData[0], 16);
		break;
	case 5:
		AT24C08PageRead(5, &readData[0], 16);
		break;
	default:
		break;
	}

	unsigned char checksum = 0xff;

	for(int i = 0; i < 15; i++) {
		checksum -= readData[i];
	}

	if(readData[15] == checksum && readData[0] == 0x45 && readData[1] == 0x50) {
		parser.byte[0] = readData[3];
		parser.byte[1] = readData[4];
		parser.byte[2] = readData[5];
		parser.byte[3] = readData[6];

		*pGain = parser.f;

		parser.byte[0] = readData[7];
		parser.byte[1] = readData[8];
		parser.byte[2] = readData[9];
		parser.byte[3] = readData[10];

		*iGain = parser.f;

		parser.byte[0] = readData[11];
		parser.byte[1] = readData[12];
		parser.byte[2] = readData[13];
		parser.byte[3] = readData[14];

		*dGain = parser.f;

		return 0;
	}

	return 1;

#if DEBUG
printf("[%s] End!\n", __func__);
#endif
}
