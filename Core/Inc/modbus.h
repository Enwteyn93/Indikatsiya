//--------------------------------------------------
#ifndef MODBUS_H_
#define MODBUS_H_
//--------------------------------------------------
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
//--------------------------------------------------
#define Slave_ID  0x0F
#define WriteCommandCode 0x06
//--------------------------------------------------
unsigned int GetCRC16(unsigned char *ptr,  unsigned char len);
void ModBus_data(uint8_t *data, uint8_t size);
//--------------------------------------------------

#endif /* MODBUS_H_ */
