#pragma once
#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>



#include "stm32f4xx_hal.h"





/*2.9"*/
#define SDRV_ST7305_H  168
#define SDRV_ST7305_W  384



void sDRV_ST7305_Init();
void sDRV_ST7305_SetInvShowMode(bool is_inv);
void sDRV_ST7305_SetAll(uint8_t data);
void sDRV_ST7305_FillImg(uint16_t x,uint16_t y,uint16_t img_height,uint16_t img_weight,const uint8_t* img);
void sDRV_ST7305_UpdateScreen(uint8_t* gram,uint32_t len_bytes);


#ifdef __cplusplus
}
#endif

