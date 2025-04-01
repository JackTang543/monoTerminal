#pragma once
#ifdef __cplusplus
extern "C"{
#endif


#include <stdbool.h>
#include "stm32f4xx_hal.h"



void sBSP_TIM_BuzzerInit();
void sBSP_TIM_BuzzerSetFreq(uint32_t freq_hz);
void sBSP_TIM_BuzzerSetEN(bool is_en);
void sBSP_TIM_BuzzerSet(float percent);



#ifdef __cplusplus
}
#endif

