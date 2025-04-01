#pragma once
#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"


#define SDRV_EC11_1_ID         1

#define SDRV_EC11_1A_CLK_EN    __GPIOA_CLK_ENABLE
#define SDRV_EC11_1A_PIN       GPIO_PIN_8
#define SDRV_EC11_1A_PORT      GPIOA

#define SDRV_EC11_1B_CLK_EN    __GPIOA_CLK_ENABLE
#define SDRV_EC11_1B_PIN       GPIO_PIN_9
#define SDRV_EC11_1B_PORT      GPIOA






void sDRV_EC11_Init();
void sDRV_EC11_TrigProcess();

int16_t sDRV_EC11_GetEncoder();


#ifdef __cplusplus
}
#endif

