#include "sDRV_W25QxxJV.h"

#include "sBSP_SPI.h"
#include "sBSP_QSPI.h"

/* 复位操作 */
#define CMD_RESET_ENABLE        (0x66)
#define CMD_RESET_MEMORY        (0x99)

/* 识别操作 */
#define CMD_READ_JEDEC_ID       (0x9F)
#define CMD_READ_ID             (0x90)

/* 读操作 */
#define CMD_READ                (0x03)
#define CMD_FAST_READ           (0x0B)

/* 写操作 */
#define CMD_WRITE_EN            (0x06)
#define CMD_WRITE_DIS           (0x04)

/* 状态寄存器 */
#define CMD_READ_SR1            (0x05)
#define CMD_READ_SR2            (0x35)
#define CMD_READ_SR3            (0x15)
#define CMD_WRITE_SR1           (0x01)
#define CMD_WRITE_SR2           (0x31)
#define CMD_WRITE_SR3           (0x11)

#define CMD_SECTOR_ERASE        (0x20)
#define CMD_32BLOCK_ERASE       (0x52)
#define CMD_64BLOCK_ERASE       (0xD8)
#define CMD_CHIP_ERASE          (0xC7)

/* 编程操作 */
#define CMD_QUAD_INPUT_PAGE_PROG (0x32)
#define CMD_PAGE_PROG           (0x02)


/* 状态寄存器标志 */
#define W25Q256JV_FSR_BUSY                    ((uint8_t)0x01)    /*!< busy */
#define W25Q256JV_FSR_WREN                    ((uint8_t)0x02)    /*!< write enable */
#define W25Q256JV_FSR_QE                      ((uint8_t)0x02)    /*!< quad enable */

#define W25Q128JV_PAGE_SIZE 4096

extern QSPI_HandleTypeDef hqspi;


/**
* @brief  读取存储器的SR并等待EOP
* @param  QSPIHandle: QSPI句柄
* @param  Timeout 超时
* @retval 无
*/
static uint8_t QSPI_AutoPollingMemReady(uint32_t Timeout)
{
    QSPI_CommandTypeDef     s_command;
    QSPI_AutoPollingTypeDef s_config;
    /* 配置自动轮询模式等待存储器准备就绪 */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = CMD_READ_SR1;
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_1_LINE;
    s_command.DummyCycles       = 0;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    s_config.Match           = 0x00;
    s_config.Mask            = W25Q256JV_FSR_BUSY;
    s_config.MatchMode       = QSPI_MATCH_MODE_AND;
    s_config.StatusBytesSize = 1;
    s_config.Interval        = 0x10;
    s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, Timeout) != HAL_OK) {
        return -1;
    }
    return 0;
}

/**
* @brief  复位QSPI存储器。
* @param  QSPIHandle: QSPI句柄
* @retval 无
*/
static int portResetMemory(){
    QSPI_CommandTypeDef s_command;
    /* 初始化复位使能命令 */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = CMD_RESET_ENABLE;
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_NONE;
    s_command.DummyCycles       = 0;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    /* 发送命令 */
    if(HAL_QSPI_Command(&hqspi, &s_command, 1000) != HAL_OK){
        return -1;
    }

    /* 发送复位存储器命令 */
    s_command.Instruction = CMD_RESET_ENABLE;
    if(HAL_QSPI_Command(&hqspi, &s_command, 1000) != HAL_OK){
        return -1;
    }

    /* 配置自动轮询模式等待存储器就绪 */
    if(QSPI_AutoPollingMemReady(1000) != HAL_OK){
        return -1;
    }

    return 0;
}

static int portQSPIWriteEnable(){
    QSPI_CommandTypeDef     s_command;
    QSPI_AutoPollingTypeDef s_config;
    /* 启用写操作 */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = CMD_WRITE_EN;
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_NONE;
    s_command.DummyCycles       = 0;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    if(HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK){
        return -1;
    }

    /* 配置自动轮询模式等待写启用 */
    s_config.Match           = W25Q256JV_FSR_WREN;
    s_config.Mask            = W25Q256JV_FSR_WREN;
    s_config.MatchMode       = QSPI_MATCH_MODE_AND;
    s_config.StatusBytesSize = 1;
    s_config.Interval        = 0x10;
    s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

    s_command.Instruction    = CMD_READ_SR1;
    s_command.DataMode       = QSPI_DATA_1_LINE;
    s_command.NbData         = 1;

    if(HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK){
        return -1;
    }
    return 0;
}

#include "sAPP_Debug.h"


static int portQSPIInit(){
    QSPI_CommandTypeDef s_command;
    uint8_t value = W25Q256JV_FSR_QE;

    /* QSPI存储器复位 */
    if (portResetMemory() != 0) {
        dbg_println("QSPI_ResetMemory error");
        return -1;
    }

    // /* 设置QSPI存储器为4字节地址模式 */
    // if (BSP_QSPI_4BYTE_ADDR_MOD() != 0) {
    //     dbg_println("BSP_QSPI_4BYTE_ADDR_MOD error");
    //     return -1;
    // }

    /* 使能写操作 */
    if (portQSPIWriteEnable() != 0) {
        dbg_println("QSPI_WriteEnable error");
        return -1;
    }
     /*设置四路使能的状态寄存器，使能四通道IO2和IO3引脚 */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = CMD_WRITE_SR1;
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_1_LINE;
    s_command.DummyCycles       = 0;
    s_command.NbData            = 1;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    /* 配置命令 */
    if (HAL_QSPI_Command(&hqspi, &s_command,
        HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return -1;
    }
    /* 传输数据 */
    if (HAL_QSPI_Transmit(&hqspi, &value,
        HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return -1;
    }
    /* 自动轮询模式等待存储器就绪 */
    if (QSPI_AutoPollingMemReady(1000)!= 0) {
        return -1;
    }
    // /* 配置地址模式为 4 字节 */
    // if (QSPI_Addr_Mode_Init() != 0) {
    //     return -1;
    // }
    return 0;
}


static uint32_t QSPI_FLASH_ReadID(void){
    QSPI_CommandTypeDef s_command;
    uint32_t Temp = 0;
    uint8_t pData[3];
    /* 读取JEDEC ID */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = CMD_READ_JEDEC_ID;
    s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
    s_command.DataMode          = QSPI_DATA_1_LINE;
    s_command.AddressMode       = QSPI_ADDRESS_NONE;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DummyCycles       = 0;
    s_command.NbData            = 3;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

if(HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        dbg_printf("something wrong ....\r\n");
        Error_Handler();
    }
    if(HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)!= HAL_OK) {
        dbg_printf("something wrong ....\r\n");
        Error_Handler();
    }

    Temp = ( pData[2] | pData[1]<<8 )| ( pData[0]<<16 );

    return Temp;
}

 /**
 * @brief  从QSPI存储器中读取大量数据.
 * @param  pData: 指向要读取的数据的指针
 * @param  ReadAddr: 读取起始地址
 * @param  Size: 要读取的数据大小
 * @retval QSPI存储器状态
 */
uint8_t BSP_QSPI_FastRead(uint8_t* pData, uint32_t ReadAddr, uint32_t
    Size)
{
QSPI_CommandTypeDef s_command;

//  if(Size == 0)
//  {
//    BURN_DEBUG("BSP_QSPI_FastRead Size = 0");
//    return QSPI_OK;
//  }
/* 初始化读命令 */
s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
s_command.Instruction       = CMD_FAST_READ;
s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
s_command.Address           = ReadAddr;
s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
s_command.DataMode          = QSPI_DATA_4_LINES;
s_command.DummyCycles       = 6;
s_command.NbData            = Size;
s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

/* 配置命令 */
if (HAL_QSPI_Command(&hqspi, &s_command,
HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
return -1;
}

/* 接收数据 */
if (HAL_QSPI_Receive(&hqspi, pData,
HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
return -1;
}
return 0;
}


 /**
 * @brief  将大量数据写入QSPI存储器
 * @param  pData: 指向要写入数据的指针
 * @param  WriteAddr: 写起始地址
 * @param  Size: 要写入的数据大小
 * @retval QSPI存储器状态
 */
uint8_t BSP_QSPI_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t
    Size)
{
QSPI_CommandTypeDef s_command;
uint32_t end_addr, current_size, current_addr;
/* 计算写入地址和页面末尾之间的大小 */
current_addr = 0;

while (current_addr <= WriteAddr) {
current_addr += W25Q128JV_PAGE_SIZE;
}
current_size = current_addr - WriteAddr;

/* 检查数据的大小是否小于页面中的剩余位置 */
if (current_size > Size) {
current_size = Size;
}

/* 初始化地址变量 */
current_addr = WriteAddr;
end_addr = WriteAddr + Size;

/* 初始化程序命令 */
s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
s_command.Instruction       = CMD_QUAD_INPUT_PAGE_PROG;
s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
s_command.DataMode          = QSPI_DATA_4_LINES;
s_command.DummyCycles       = 0;
s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

/* 逐页执行写入 */
do {
s_command.Address = current_addr;
s_command.NbData  = current_size;

/* 启用写操作 */
if (portQSPIWriteEnable() != 0) {
return -1;
}

/* 配置命令 */
if (HAL_QSPI_Command(&hqspi, &s_command,
HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
return -1;
}

/* 传输数据 */
if (HAL_QSPI_Transmit(&hqspi, pData,
HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
return -1;
}

/* 配置自动轮询模式等待程序结束 */
if (QSPI_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
!= 0) {
return -1;
}

/* 更新下一页编程的地址和大小变量 */
current_addr += current_size;
pData += current_size;
current_size = ((current_addr + W25Q128JV_PAGE_SIZE) >
    end_addr) ? (end_addr - current_addr) :
    W25Q128JV_PAGE_SIZE;
} while (current_addr < end_addr);
return 0;
}

uint8_t BSP_QSPI_Erase_Block(uint32_t BlockAddress)
{
    QSPI_CommandTypeDef s_command;
    /* 初始化擦除命令 */
    s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    s_command.Instruction       = CMD_SECTOR_ERASE;
    s_command.AddressMode       = QSPI_ADDRESS_1_LINE;
    s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
    s_command.Address           = BlockAddress;
    s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    s_command.DataMode          = QSPI_DATA_NONE;
    s_command.DummyCycles       = 0;
    s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
    s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    /* 启用写操作 */
    if (portQSPIWriteEnable() != 0) {
        return -1;
    }

    /* 发送命令 */
if(HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return -1;
    }

    /* 配置自动轮询模式等待擦除结束 */
    if (QSPI_AutoPollingMemReady(1000) != 0) {
        return -1;
    }
    return 0;
}

int sDRV_W25QxxJV_Init(){
    //初始化QSPI
    if(portQSPIInit() != 0){
        dbg_println("portQSPIInit error");
    }

    if(portResetMemory() != 0){
        dbg_println("portResetMemory error");
    }

    //写使能
    if(portQSPIWriteEnable() != 0){
        dbg_println("portQSPIWriteEnable error");
    }

    

    // if(QSPI_AutoPollingMemReady(100) != 0){
    //     return -1;
    // }

    // dbg_println("w25 id=0x%0X",QSPI_FLASH_ReadID()); 


    //先擦除当前扇区
    if(BSP_QSPI_Erase_Block(0) != 0){
        dbg_println("BSP_QSPI_Erase_Block error");
    }
    
    // //写入128字节的0x81数据到0x0地址,然后读取回来,打印出来
    // static uint8_t data[128];
    // for(int i = 0; i < 128; i++){
    //     data[i] = 0x01;
    // }
    // BSP_QSPI_Write(data,0x0,128);
    // static uint8_t read_data[512];
    // BSP_QSPI_FastRead(read_data,0x0,512);
    // //一行32个数据,多行打印
    // for(int i = 0; i < 512; i++){
    //     if(i % 16 == 0){
    //         dbg_println("");
    //     }
    //     dbg_printf("0x%02X ",read_data[i]);
    // }

    return 0;
}





