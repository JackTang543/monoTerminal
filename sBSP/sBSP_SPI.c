#include "sBSP_SPI.h"


#define SPI1_CS_CLK_EN     __GPIOA_CLK_ENABLE
#define SPI1_CS_PORT       GPIOA
#define SPI1_CS_PIN        GPIO_PIN_4

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;


void sBSP_SPI_LCDInit(){
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    HAL_SPI_Init(&hspi1);

    SPI1_CS_CLK_EN();
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
    gpio.Pin = SPI1_CS_PIN;
    HAL_GPIO_Init(SPI1_CS_PORT,&gpio);
    HAL_GPIO_WritePin(SPI1_CS_PORT,SPI1_CS_PIN,GPIO_PIN_SET);
}

void sBSP_SPI_LCDSendByte(uint8_t byte){
    HAL_GPIO_WritePin(SPI1_CS_PORT,SPI1_CS_PIN,(GPIO_PinState)0);
    HAL_SPI_Transmit(&hspi1,&byte,1,100);
    HAL_GPIO_WritePin(SPI1_CS_PORT,SPI1_CS_PIN,(GPIO_PinState)1);
}

void sBSP_SPI_LCDSendBytes(uint8_t *pData,uint16_t Size){
    HAL_GPIO_WritePin(SPI1_CS_PORT,SPI1_CS_PIN,(GPIO_PinState)0);

    // HAL_SPI_Transmit(&hspi1,pData,Size,1000);
    
    HAL_SPI_Transmit_DMA(&hspi1,pData,Size);
    // while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);

    // HAL_GPIO_WritePin(SPI1_CS_PORT,SPI1_CS_PIN,(GPIO_PinState)1);
}





void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(spiHandle->Instance==SPI1){
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**SPI1 GPIO Configuration
        PA4     ------> SPI1_NSS
        PA5     ------> SPI1_SCK
        PA7     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        hdma_spi1_tx.Instance = DMA2_Stream2;
        hdma_spi1_tx.Init.Channel = DMA_CHANNEL_2;
        hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_spi1_tx.Init.Mode = DMA_NORMAL;
        hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;
        hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        HAL_DMA_Init(&hdma_spi1_tx);
    
        __HAL_LINKDMA(spiHandle,hdmatx,hdma_spi1_tx);

    }
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
    if(hspi->Instance == SPI1){
        HAL_GPIO_WritePin(SPI1_CS_PORT,SPI1_CS_PIN,(GPIO_PinState)1);
    }
}


