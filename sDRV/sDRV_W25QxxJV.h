#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f4xx_hal.h"


#include "stdbool.h"


#define SDRV_W25Q128JV_PAGE_SIZE           (256u)
#define SDRV_W25Q128JV_SECTOR_SIZE         (4096u)
#define SDRV_W25Q128JV_BLOCK_SIZE          (64 * 1024)

//这是最大的索引,number是MAX_INDEX+1
#define SDRV_W25Q128JV_SECTOR_MAX_INDEX    (16 * 256 - 1)
#define SDRV_W25Q128JV_BLOCK_MAX_INDEX     (255)


int sDRV_W25QxxJV_Init();

int sDRV_W25QxxJV_WritePage(uint32_t addr,uint8_t* data,uint16_t len);
int sDRV_W25QxxJV_ReadBytes(uint32_t addr,uint8_t* data,uint32_t len);
int sDRV_W25QxxJV_SectorErase4KB(uint32_t sector_index);
int sDRV_W25QxxJV_BlockErase32KB(uint32_t block_index);
int sDRV_W25QxxJV_BlockErase64KB(uint32_t block_index);

// int sDRV_W25QxxJV_WriteBytes(uint32_t addr, uint8_t* buf, uint32_t len);
int sDRV_W25QxxJV_WriteBytes(uint32_t begin_addr,uint8_t* pData,uint32_t number);

int sDRV_W25QxxJV_ChipErase(bool is_blocking);
bool sDRV_W25QxxJV_IsBusy();



#ifdef __cplusplus
}
#endif

