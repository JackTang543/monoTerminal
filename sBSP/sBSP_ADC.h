#pragma once
#ifdef __cplusplus
extern "C"{
#endif


#include <stdbool.h>
#include "stm32f4xx_hal.h"




int sBSP_ADC_Init();

float sBSP_ADC_GetMCUTemp();
float sBSP_ADC_GetVbat();
float sBSP_ADC_Get5V();



#ifdef __cplusplus
}
#endif

