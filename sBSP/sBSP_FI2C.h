#pragma once
#ifdef __cplusplus
extern "C"{
#endif



#include "stm32f4xx_hal.h"
#include "stdbool.h"



int sBSP_FI2C1_Init();
int sBSP_FI2C1M_SendByte(uint8_t addr,uint8_t byte);
int sBSP_FI2C1M_SendBytes(uint8_t addr,const uint8_t* data,uint16_t len);
uint8_t sBSP_FI2C1M_RecvByte(uint8_t addr);
int sBSP_FI2C1M_RecvBytes(uint8_t addr,uint8_t* data,uint16_t len);





#ifdef __cplusplus
}
#endif

