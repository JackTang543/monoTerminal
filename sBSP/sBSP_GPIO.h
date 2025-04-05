#pragma once
#ifdef __cplusplus
extern "C"{
#endif


#include <stdbool.h>
#include "stm32f4xx_hal.h"




void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


bool sBSP_GPIO_GetEncoder1KEY();
bool sBSP_GPIO_GetEnterKEY();
bool sBSP_GPIO_GetBackKEY();





#ifdef __cplusplus
}
#endif

