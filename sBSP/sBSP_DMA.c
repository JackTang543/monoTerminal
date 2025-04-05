#include "sBSP_DMA.h"





void sBSP_DMA_Init(){
    __HAL_RCC_DMA2_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
}







