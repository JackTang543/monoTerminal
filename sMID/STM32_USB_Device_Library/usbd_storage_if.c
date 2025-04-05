/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @version        : v1.0_Cube
  * @brief          : Memory management layer.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_STORAGE
  * @brief Usb mass storage device module
  * @{
  */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Defines
  * @brief Private defines.
  * @{
  */

#define STORAGE_LUN_NBR                  1


#include "sDRV_W25QxxJV.h"


// // 64KB SRAM 中做一个“模拟磁盘”
// #define SRAM_DISK_SIZE      (64 * 1024UL)
// #define SRAM_BLOCK_SIZE     512U
// #define SRAM_BLOCK_COUNT    (SRAM_DISK_SIZE / SRAM_BLOCK_SIZE)

#define STORAGE_BLK_NBR                  (SDRV_W25Q128JV_SECTOR_MAX_INDEX + 1)
// #define STORAGE_BLK_NBR                  (64UL)

#define STORAGE_BLK_SIZ                  (4096UL)
// #define STORAGE_BLK_SIZ                  (512UL)



#include "defines.h"



/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN INQUIRY_DATA_FS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] = {/* 36 */

  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                      /* Version      : 4 Bytes */
};
/* USER CODE END INQUIRY_DATA_FS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the storage unit (medium) over USB FS IP
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Init_FS(uint8_t lun)
{
  /* USER CODE BEGIN 2 */
  UNUSED(lun);

  return (USBD_OK);
  /* USER CODE END 2 */
}

/**
  * @brief  Returns the medium capacity.
  * @param  lun: Logical unit number.
  * @param  block_num: Number of total block number.
  * @param  block_size: Block size.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */
  UNUSED(lun);

  *block_num  = STORAGE_BLK_NBR;
  *block_size = STORAGE_BLK_SIZ;
  return (USBD_OK);
  /* USER CODE END 3 */
}
#include "sUtils.h"

/**
  * @brief   Checks whether the medium is ready.
  * @param  lun:  Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
  /* USER CODE BEGIN 4 */
  UNUSED(lun);

  // log_printfln("is_ready?");


  return (USBD_OK);
  // return sDRV_W25QxxJV_IsBusy() ? USBD_FAIL : USBD_OK;

  /* USER CODE END 4 */
}

/**
  * @brief  Checks whether the medium is write protected.
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
  /* USER CODE BEGIN 5 */
  UNUSED(lun);

  return 0;
  /* USER CODE END 5 */
}


/**
  * @brief  Reads data from the medium
  * @param  lun:       Logical unit number (一般忽略).
  * @param  buf:       传递给主机的数据缓冲区.
  * @param  blk_addr:  逻辑块地址 (LBA).
  * @param  blk_len:   要读取的块数量.
  * @retval USBD_OK or USBD_FAIL
  */
// int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len){
//     UNUSED(lun);

//      /* 判断是否越界 */
//     if((blk_addr + blk_len) > STORAGE_BLK_NBR){
//         return USBD_FAIL;
//     }


//     log_printfln("读取,blk_addr=%u,blk_len=%u",blk_addr,blk_len);

//     for(uint32_t i = 0;i < blk_len;i++){
// 		  sDRV_W25QxxJV_ReadBytes(blk_addr * STORAGE_BLK_SIZ + i * STORAGE_BLK_SIZ,buf + i * STORAGE_BLK_SIZ,STORAGE_BLK_SIZ );
//     }
//     // if(sDRV_W25QxxJV_ReadBytes(blk_addr * STORAGE_BLK_SIZ,buf,(uint32_t)(blk_len) * STORAGE_BLK_SIZ) != 0){
//     //   log_printfln("读取出错");
//     //   return USBD_FAIL;
//     // }
//     return USBD_OK;
// }
int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    // 1. 计算起始物理地址：
    //    每个逻辑块大小 = 4KB
    uint32_t addr = blk_addr * 4096UL;

    // 2. 计算需要读取的总字节数
    uint32_t total_size = (uint32_t)blk_len * 4096UL;

    log_printfln("读取,blk_addr=%u,blk_len=%u",blk_addr,blk_len);

    // 3. 调用你的底层读取接口
    int ret = sDRV_W25QxxJV_ReadBytes(addr, buf, total_size);
    if (ret != 0)
    {
        // 读取失败
        return USBD_FAIL;
    }

    // 读取成功
    return USBD_OK;
}
// int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
// {
//     // 每个逻辑块大小为 512 字节
//     uint32_t addr = blk_addr * 4096UL;  // 计算物理地址，每个逻辑块 = 4KB

//     // 计算需要读取的总字节数
//     uint32_t total_size = blk_len * 512UL;  // 读取的总字节数

//     // 调用 W25Q128JV 驱动函数读取指定地址的数据
//     int ret = sDRV_W25QxxJV_ReadBytes(addr, buf, total_size);
//     if (ret != 0)
//     {
//         // 读取失败
//         return USBD_FAIL;
//     }

//     // 打印读取的数据
//     for (uint32_t i = 0; i < blk_len; i++) {
//         log_printf("读取, blk_addr=%d, blk_len=%d:\n", blk_addr + i, blk_len);
//         for (uint32_t j = 0; j < 512; j += 32) {
//             // 每行打印32个字节
//             log_printf("%3d-%3d: ", j, j + 31);
//             for (uint32_t k = 0; k < 32; k++) {
//                 // 打印每行的32个字节
//                 log_printf("0x%02X ", buf[i * 512 + j + k]);
//             }
//             log_printf("\n");
//         }
//     }

//     // 读取成功
//     return USBD_OK;
// }
/**
  * @brief  Writes data into the medium
  * @param  lun:       Logical unit number (一般忽略).
  * @param  buf:       主机写入的数据源缓冲区.
  * @param  blk_addr:  逻辑块地址 (LBA).
  * @param  blk_len:   要写入的块数量.
  * @retval USBD_OK or USBD_FAIL
  */
// int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len){
//     UNUSED(lun);

//     /* 判断是否越界 */
//     if((blk_addr + blk_len) > STORAGE_BLK_NBR){
//         return USBD_FAIL;
//     }

    
//     log_printfln("写入,blk_addr=%u,blk_len=%u",blk_addr,blk_len);

//     // 1. 计算写入的起始物理地址
//     uint32_t addr = blk_addr * STORAGE_BLK_SIZ;
//     // 2. 计算本次写入的总字节数
//     uint32_t total_size = (uint32_t)blk_len * STORAGE_BLK_SIZ;

//     // 由于1个块= 4KB, 最小擦除单位也是4KB, 则每写一个4KB的逻辑块:
//     //   a) 先擦除对应扇区
//     //   b) 再分多次(页写)把这4KB数据写入Flash

//     // 写时可能一次性要写多个块(blk_len > 1)，
//     // 可以遍历每个块，分别擦除+写入
//     uint32_t offset = 0;  // buf中的偏移
//     for (uint16_t i = 0; i < blk_len; i++)
//     {
//         // 当前块的起始物理地址
//         uint32_t cur_addr = addr + (i * 4096UL);

//         // a) 扇区擦除
//         //    先计算扇区索引 (每个4KB是1个sector)
//         uint32_t sector_index = (cur_addr / 4096UL);
//         if (sDRV_W25QxxJV_SectorErase4KB(sector_index) != 0)
//         {
//             return USBD_FAIL;
//         }
//         // 等待擦除结束
//         while (sDRV_W25QxxJV_IsBusy())
//         {
//             // 这里可适当加超时或喂狗
//         }

//         // b) 将这4KB的数据分多页写入(256字节/页)
//         //    先得到当前块对应的buf指针
//         uint8_t *p_data = &buf[offset];
//         offset += 4096; // 每次4KB

//         uint32_t bytes_remaining = 4096;
//         uint32_t page_offset = 0;
//         while (bytes_remaining > 0)
//         {
//             // 本次要写的字节数(最多256)
//             uint16_t write_size = (bytes_remaining > 256) ? 256 : bytes_remaining;

//             // 写入一页
//             int ret = sDRV_W25QxxJV_WritePage(cur_addr + page_offset, p_data + page_offset, write_size);
//             if (ret != 0)
//             {
//                 return USBD_FAIL;
//             }

//             // 等待写完成
//             while (sDRV_W25QxxJV_IsBusy())
//             {
//                 // 这里可适当加超时或喂狗
//             }

//             page_offset     += write_size;
//             bytes_remaining -= write_size;
//         }
//     }

//     return USBD_OK;
// }

int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
    // 对于4KB对齐的逻辑块，每个LBA=4KB
    // blk_addr: 要写入的起始逻辑块
    // blk_len:  要写入的逻辑块数量
    // buf:      主机发送过来的数据(大小=blk_len * 4KB)

    // 写操作流程：
    //   for i in [0 .. blk_len-1]:
    //     1) 扇区擦除(i对应的4KB)
    //     2) 分多页(256B/次)写入4KB的数据

    // 数据在buf中：第 i 个块对应buf起始= i * 4096
    
    log_printfln("写入,blk_addr=%u,blk_len=%u",blk_addr,blk_len);

    uint32_t offset = 0;  // buf里的偏移
    for(uint16_t i = 0; i < blk_len; i++)
    {
        // (1) 计算当前块的物理地址
        //     = (blk_addr + i)*4096
        uint32_t cur_blk_addr = (blk_addr + i) * 4096UL;

        // (2) 先擦除当前4KB扇区
        {
            uint32_t sector_index = (cur_blk_addr / 4096UL);
            int erase_ret = sDRV_W25QxxJV_SectorErase4KB(sector_index);
            if(erase_ret != 0)
            {
                return USBD_FAIL;
            }

            // 等待擦除完成
            while(sDRV_W25QxxJV_IsBusy())
            {
                // 这里可适度加超时逻辑，或喂狗
            }
        }

        // (3) 写入4KB数据 (每次最多256B)
        uint32_t bytes_written = 0;
        while(bytes_written < 4096)
        {
            // 剩余待写字节
            uint32_t remain = 4096 - bytes_written;
            uint16_t write_size = (remain > 256) ? 256 : (uint16_t)remain;

            // 调用Page Program
            int ret = sDRV_W25QxxJV_WritePage(
                cur_blk_addr + bytes_written,         // 写地址
                &buf[offset + bytes_written],         // 数据源
                write_size                            // 写入大小
            );
            if(ret != 0)
            {
                return USBD_FAIL;
            }

            // 等待页写完成
            while(sDRV_W25QxxJV_IsBusy())
            {
                // 喂狗或超时处理
            }

            bytes_written += write_size;
        }

        // 处理完当前块后，移动buf偏移量
        offset += 4096;
    }

    return USBD_OK;
}


// int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
// {
//     uint32_t offset = 0;  // 用于在缓冲区中移动数据

//     log_printf("写入, blk_addr=%d, blk_len=%d:\n", blk_addr, blk_len);


//     for (uint16_t i = 0; i < blk_len; i++)
//     {
//         // 当前逻辑块的起始物理地址
//         uint32_t cur_blk_addr = (blk_addr + 1 + i) * 4096UL;  // 每个块的起始地址为4KB对齐

//         // (1) 先擦除当前 4KB 扇区
//         uint32_t sector_index = (cur_blk_addr / 4096UL);  // 每个扇区 4KB
//         int erase_ret = sDRV_W25QxxJV_SectorErase4KB(sector_index);
//         if (erase_ret != 0)
//         {
//             return USBD_FAIL;
//         }

//         // 等待擦除完成
//         while (sDRV_W25QxxJV_IsBusy())
//         {
//             // 等待擦除完成
//         }

//         // (2) 写入 512 字节数据到前 512 字节（每个扇区的前 512 字节）
//         uint32_t bytes_written = 0;
//         while (bytes_written < 512)
//         {
//             uint16_t write_size = (512 - bytes_written) > 256 ? 256 : (512 - bytes_written);  // 每次写入最大 256 字节

//             int ret = sDRV_W25QxxJV_WritePage(cur_blk_addr + bytes_written, &buf[offset + bytes_written], write_size);
//             if (ret != 0)
//             {
//                 return USBD_FAIL;
//             }

//             // 等待写入完成
//             while (sDRV_W25QxxJV_IsBusy())
//             {
//                 // 等待写入完成
//             }

//             bytes_written += write_size;
//         }

//         offset += 512;  // 每次移动 512 字节

//         // 打印写入的数据
//         for (uint32_t j = 0; j < 512; j += 32) {
//             // 每行打印32个字节
//             log_printf("%3d-%3d: ", j, j + 31);
//             for (uint32_t k = 0; k < 32; k++) {
//                 // 打印每行的32个字节
//                 log_printf("0x%02X ", buf[i * 512 + j + k]);
//             }
//             log_printf("\n");
//         }
//     }

//     // 写入完成
//     return USBD_OK;
// }

/**
  * @brief  Returns the Max Supported LUNs.
  * @param  None
  * @retval Lun(s) number.
  */
int8_t STORAGE_GetMaxLun_FS(void)
{
  /* USER CODE BEGIN 8 */
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 8 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

