#include "sBSP_FI2C.h"

#include "sAPP_Debug.h"

FMPI2C_HandleTypeDef hfmpi2c1;


int sBSP_FI2C1_Init(){
    hfmpi2c1.Instance = FMPI2C1;
    hfmpi2c1.Init.Timing = 0x00C0EAFF;
    hfmpi2c1.Init.OwnAddress1 = 0;
    hfmpi2c1.Init.AddressingMode = FMPI2C_ADDRESSINGMODE_7BIT;
    hfmpi2c1.Init.DualAddressMode = FMPI2C_DUALADDRESS_DISABLE;
    hfmpi2c1.Init.OwnAddress2 = 0;
    hfmpi2c1.Init.OwnAddress2Masks = FMPI2C_OA2_NOMASK;
    hfmpi2c1.Init.GeneralCallMode = FMPI2C_GENERALCALL_DISABLE;
    hfmpi2c1.Init.NoStretchMode = FMPI2C_NOSTRETCH_DISABLE;
    if (HAL_FMPI2C_Init(&hfmpi2c1) != HAL_OK){
        return -1;
    }

    if (HAL_FMPI2CEx_ConfigAnalogFilter(&hfmpi2c1, FMPI2C_ANALOGFILTER_ENABLE) != HAL_OK){
        return -2;
    }

    return 0;
}


int sBSP_FI2C1M_SendByte(uint8_t addr,uint8_t byte){
    if(HAL_FMPI2C_Master_Transmit(&hfmpi2c1,addr,&byte,1,100) != HAL_OK){
        return -1;
    }
    return 0;
}

int sBSP_FI2C1M_SendBytes(uint8_t addr,const uint8_t* data,uint16_t len){
    if(HAL_FMPI2C_Master_Transmit(&hfmpi2c1,addr,(uint8_t*)data,len,1000) != HAL_OK){
        return -1;
    }
    return 0;
}

uint8_t sBSP_FI2C1M_RecvByte(uint8_t addr){
    uint8_t ret = 0;
    HAL_FMPI2C_Master_Receive(&hfmpi2c1,addr,&ret,1,100);
    return ret;
}

int sBSP_FI2C1M_RecvBytes(uint8_t addr,uint8_t* data,uint16_t len){
    if(HAL_FMPI2C_Master_Receive(&hfmpi2c1,addr,data,len,1000) != HAL_OK){
        return -1;
    }
    return 0;
}


void HAL_FMPI2C_MspInit(FMPI2C_HandleTypeDef* fmpi2cHandle){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if(fmpi2cHandle->Instance == FMPI2C1){
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FMPI2C1;
        PeriphClkInitStruct.Fmpi2c1ClockSelection = RCC_FMPI2C1CLKSOURCE_APB;
        
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK){
            Error_Handler();
        }

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**FMPI2C1 GPIO Configuration
        PB10     ------> FMPI2C1_SCL
        PB14     ------> FMPI2C1_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_FMPI2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_14;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_FMPI2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* FMPI2C1 clock enable */
        __HAL_RCC_FMPI2C1_CLK_ENABLE();
    }
}


