#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"


void sBSP_QSPI_Init();
uint32_t sBSP_QSPI_ReadID();



#ifdef __cplusplus
}
#endif

