#pragma once
#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>



#include "stm32f4xx_hal.h"




void sBSP_SPI_LCDInit();
void sBSP_SPI_LCDSendByte(uint8_t byte);
void sBSP_SPI_LCDSendBytes(uint8_t *pData,uint16_t Size);





#ifdef __cplusplus
}
#endif

