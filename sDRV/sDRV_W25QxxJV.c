#include "sDRV_W25QxxJV.h"

#include "sBSP_SPI.h"
#include "sBSP_QSPI.h"



/*命令表*/

#define CMD_WRITE_ENABLE              (0x06)
#define CMD_VOLATILE_SR_WRITE_ENABLE  (0x50)
#define CMD_WRITE_DISABLE             (0x04)

#define CMD_RELEASE_POWER_DOWN_ID     (0xAB)
#define CMD_MANUFACTURER_DEVICE_ID    (0x90)
#define CMD_JEDEC_ID                  (0x9F)
#define CMD_READ_UNIQUE_ID            (0x4B)

#define CMD_READ_DATA                 (0x03)
#define CMD_FAST_READ                 (0x0B)

#define CMD_PAGE_PROGRAM              (0x02)

#define CMD_SECTOR_ERASE_4KB          (0x20)
#define CMD_BLOCK_ERASE_32KB          (0x52)
#define CMD_BLOCK_ERASE_64KB          (0xD8)
#define CMD_CHIP_ERASE                (0xC7)

#define CMD_READ_STATUS_REGISTER_1    (0x05)
#define CMD_WRITE_STATUS_REGISTER_1   (0x01)
#define STATUS_REGISTER_1_MSK_BUSY    (0b00000001)

#define CMD_READ_STATUS_REGISTER_2    (0x35)
#define CMD_WRITE_STATUS_REGISTER_2   (0x31)
#define CMD_READ_STATUS_REGISTER_3    (0x15)
#define CMD_WRITE_STATUS_REGISTER_3   (0x11)

#define CMD_READ_SFDP_REGISTER        (0x5A)
#define CMD_ERASE_SECURITY_REGISTER   (0x44)
#define CMD_PROGRAM_SECURITY_REGISTER (0x42)
#define CMD_READ_SECURITY_REGISTER    (0x48)

#define CMD_GLOBAL_BLOCK_LOCK         (0x7E)
#define CMD_GLOBAL_BLOCK_UNLOCK       (0x98)
#define CMD_READ_BLOCK_LOCK           (0x3D)
#define CMD_INDIVIDUAL_BLOCK_LOCK     (0x36)
#define CMD_INDIVIDUAL_BLOCK_UNLOCK   (0x39)

#define CMD_ERASE_PROGRAM_SUSPEND     (0x75)
#define CMD_ERASE_PROGRAM_RESUME      (0x7A)
#define CMD_POWER_DOWN                (0xB9)

#define CMD_ENABLE_RESET              (0x66)
#define CMD_RESET_DEVICE              (0x99)


#define CMD_FAST_READ_DUAL_OUTPUT     (0x3B)

#define CMD_FAST_READ_DUAL_IO         (0xBB)
#define CMD_MFTR_DEVICE_ID_DUAL_IO    (0x92)

#define CMD_QUAD_INPUT_PAGE_PROGRAM   (0x32)
#define CMD_FAST_READ_QUAD_OUTPUT     (0x6B)

#define CMD_MFTR_DEVICE_ID_QUAD_IO    (0x94)
#define CMD_FAST_READ_QUAD_IO         (0xEB)
#define CMD_SET_BURST_WITH_WRAP       (0x77)



extern QSPI_HandleTypeDef hqspi;




#include "sUtils.h"


/// @brief 读器件ID,可以用来检查通信是否正常
/// @param 无
/// @return 器件ID
static uint32_t read_device_id(void){
    QSPI_CommandTypeDef sCommand = {0};
    uint8_t data[2] = {0};  // 存放Device ID的缓冲区

    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = CMD_MANUFACTURER_DEVICE_ID;
    //这个如果是QSPI_ADDRESS_1_LINE,会在MISO(IO0)上跟着指令后发送地址,整个通信时钟数就是:指令8 + 3*8(24bits地址) + 8*NbData=56CYCLES
    //! 这才是正常的!
    sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
    //这个如果是QSPI_ADDRESS_NONE,就是不发送地址,整个通信时钟数就是:指令8 + 0 + 8*NbData=24CYCLES
    // sCommand.AddressMode       = QSPI_ADDRESS_NONE;

    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address           = 0;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_1_LINE;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    //接收2个字节
    sCommand.NbData = 2;

    // 发送命令并接收数据
    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return 0;
    }
    if(HAL_QSPI_Receive(&hqspi, data, 100) != HAL_OK){
        return 0;
    }

    // Device ID数据存放在data数组中，返回设备ID
    uint32_t device_id = (data[0] << 8) | data[1];  // 将厂商ID与设备ID组合起来
    return device_id;  // 返回设备ID
}

/// @brief 复位器件
/// @param 无
/// @return 0表示操作成功
static int reset_device(void){
    QSPI_CommandTypeDef sCommand = {0};;

    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = CMD_ENABLE_RESET;
    sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize       = QSPI_ADDRESS_8_BITS;
    //用ADDR代替第二个复位指令
    sCommand.Address           = CMD_RESET_DEVICE;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    //没有数据
    sCommand.DataMode          = QSPI_DATA_NONE;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = 0;

    // 发送命令并接收数据
    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return -1;
    }
    return 0;
}

/// @brief 读寄存器
/// @param command 要读的寄存器地址
/// @return 寄存器内容
static uint8_t read_register(uint8_t command){
    QSPI_CommandTypeDef sCommand = {0};
    uint8_t data;

    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = command;
    sCommand.AddressMode       = QSPI_ADDRESS_NONE;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address           = 0;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_1_LINE;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    //接收1个字节
    sCommand.NbData = 1;

    // 发送命令并接收数据
    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return 0;
    }
    if(HAL_QSPI_Receive(&hqspi, &data, 100) != HAL_OK){
        return 0;
    }

    return data;  // 返回设备ID
}

/// @brief 写寄存器
/// @param command 寄存器地址
/// @param data 写入的数据
/// @return 0表示操作成功
static int write_register(uint8_t command,uint8_t data){
    QSPI_CommandTypeDef sCommand = {0};

    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = command;
    sCommand.AddressMode       = QSPI_ADDRESS_NONE;
    sCommand.AddressSize       = QSPI_ADDRESS_8_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_1_LINE;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = 1;

    //配置命令,没有发送
    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return -1;
    }
    if(HAL_QSPI_Transmit(&hqspi, &data, 100) != HAL_OK){
        return -2;
    }

    return 0;
}

/// @brief 发送命令字
/// @param command 要发送的命令字,其实就是寄存器地址
/// @return 0表示操作成功
static int send_command(uint8_t command){
    QSPI_CommandTypeDef sCommand = {0};

    sCommand.InstructionMode   = QSPI_INSTRUCTION_NONE;
    sCommand.Instruction       = 0;
    sCommand.AddressMode       = QSPI_ADDRESS_NONE;
    sCommand.AddressSize       = QSPI_ADDRESS_8_BITS;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_1_LINE;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = 1;

    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return -1;
    }
    if(HAL_QSPI_Transmit(&hqspi, &command, 100) != HAL_OK){
        return -2;
    }

    return 0;
}

/// @brief 阻塞式等待忙位清零
/// @param timeout_ms 超时时间
/// @return 0表示操作成功
static int wait_busy(uint32_t timeout_ms){
    QSPI_CommandTypeDef scmd = {
        .InstructionMode   = QSPI_INSTRUCTION_1_LINE,
        .Instruction       = CMD_READ_STATUS_REGISTER_1,
        .AddressMode       = QSPI_ADDRESS_NONE,
        .AddressSize       = QSPI_ADDRESS_24_BITS,
        .AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE,
        .DataMode          = QSPI_DATA_1_LINE,
        .DummyCycles       = 0,
        .DdrMode           = QSPI_DDR_MODE_DISABLE,
        .DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY,
        .SIOOMode          = QSPI_SIOO_INST_EVERY_CMD,
        .NbData = 1,
    };

    QSPI_AutoPollingTypeDef scfg = {
        .Match           = 0x00,
        .Mask            = STATUS_REGISTER_1_MSK_BUSY,
        .MatchMode       = QSPI_MATCH_MODE_AND,
        .StatusBytesSize = 1,
        .Interval        = 0x20,
        .AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE,
    };

    if(HAL_QSPI_AutoPolling(&hqspi,&scmd,&scfg,timeout_ms) != HAL_OK){
        return -1;
    }

    return 0;
}

/// @brief 读数据
/// @param addr 起始读地址
/// @param pData 读取的缓冲区
/// @param len 要读多少的数据
/// @return 0表示操作成功
static int read_bytes(uint32_t addr,uint8_t* pData,uint32_t len){
    QSPI_CommandTypeDef sCommand = {0};
    //Fast Read Quad I/O,除了指令占8个时钟周期,地址和数据都是以4bit per cycles传输的

    //要等待busy位
    if(wait_busy(1000) != 0){
        return -1;
    }

    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = CMD_FAST_READ_QUAD_IO;
    sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address = addr;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_4_LINES;
    sCommand.DummyCycles       = 2 + 4; //空周期 M7~0 和四个dummy
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = len;

    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return -2;
    }
    if(HAL_QSPI_Receive(&hqspi, pData, 1000) != HAL_OK){
        return -3;
    }

    return 0;
}

static int set_write_enable(){
    int ret = 0;
    ret = send_command(CMD_VOLATILE_SR_WRITE_ENABLE);
    ret = send_command(CMD_WRITE_ENABLE);

    return ret;
}


/// @brief 写数据
/// @param addr 起始写地址
/// @param pData 缓冲区
/// @param len 要写多少,注意一次最大256Bytes的数据!
/// @return 0表示操作正常
static int write_bytes(uint32_t addr,uint8_t* pData,uint32_t len){
    QSPI_CommandTypeDef sCommand = {0};
    //Fast Read Quad I/O,除了指令占8个时钟周期,地址和数据都是以4bit per cycles传输的

    if(len > SDRV_W25Q128JV_PAGE_SIZE){
        return -1;
    }

    set_write_enable();


    //要等待busy位
    if(wait_busy(1000) != 0){
        return -2;
    }    

    // log_printfln("下面的是写操作,addr=%u,pData=%u,len=%u,准备开始写入",addr,pData,len);


    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = CMD_QUAD_INPUT_PAGE_PROGRAM;
    sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address = addr;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_4_LINES;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = len;

    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return -1;
    }
    if(HAL_QSPI_Transmit(&hqspi, pData, 1000) != HAL_OK){
        return -2;
    }

    return 0;
}





int sDRV_W25QxxJV_Init(){
    reset_device();
    HAL_Delay(1);
    log_printfln("w25q128 reset ok");

    uint32_t dev_id = read_device_id();
    log_printfln("dev_id=0x%04X",dev_id);

    if(dev_id == 0xEF17){
        log_printfln("dev ok");
    }

    uint8_t sr1 = read_register(CMD_READ_STATUS_REGISTER_1);
    uint8_t sr2 = read_register(CMD_READ_STATUS_REGISTER_2);
    uint8_t sr3 = read_register(CMD_READ_STATUS_REGISTER_3);

    log_printfln("SR1=0x%02X,SR2=0x%02X,SR3=0x%02X",sr1,sr2,sr3);


    //其实JVSIQ默认就工作在QuadSPI下(SR2=0x02),但是这里还是重新设置一下保险一点
    write_register(CMD_WRITE_STATUS_REGISTER_2,sr2 | 0x02);
    log_printfln("set w25q128 QSPI work mode");

    //turn on write enable
    set_write_enable();
    log_printfln("set write enable");

    HAL_Delay(1);

    {
        uint8_t sr1 = read_register(CMD_READ_STATUS_REGISTER_1);
        uint8_t sr2 = read_register(CMD_READ_STATUS_REGISTER_2);
        uint8_t sr3 = read_register(CMD_READ_STATUS_REGISTER_3);
        log_printfln("SR1=0x%02X,SR2=0x%02X,SR3=0x%02X",sr1,sr2,sr3);
    }


    // log_printfln("读地址0x000000");

    // //读一段数据看看
    // uint8_t buf[128] = {0};
    // read_bytes(0x000000,buf,128);
    // //一行32个数据,多行打印
    // for(int i = 0; i < 128; i++){
    //     if(i % 16 == 0){
    //         log_printfln("");
    //     }
    //     log_printf("0x%02X ",buf[i]);
    // }
    

    // log_printfln("写入地址0x000000,0~31");

    // uint8_t send_buf[32];
    // for(int i = 0;i < 32;i++){
    //     send_buf[i] = i;
    // }
    // write_bytes(0x000000,send_buf,32);

    // log_printfln("读地址0x000000");

    // //读一段数据看看
    // {
    //     uint8_t buf[128] = {0};
    //     read_bytes(0x000000,buf,128);
    //     //一行32个数据,多行打印
    //     for(int i = 0; i < 128; i++){
    //         if(i % 16 == 0){
    //             log_printfln("");
    //         }
    //         log_printf("0x%02X ",buf[i]);
    //     }
    
    // }

    // sDRV_W25QxxJV_SectorErase4KB(0);

    // log_printfln("读地址0x000000");

    // //读一段数据看看
    // {
    //     uint8_t buf[128] = {0};
    //     read_bytes(0x000000,buf,128);
    //     //一行32个数据,多行打印
    //     for(int i = 0; i < 128; i++){
    //         if(i % 16 == 0){
    //             log_printfln("");
    //         }
    //         log_printf("0x%02X ",buf[i]);
    //     }
    
    // }



    
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


/// @brief 页写入,耗时0.4ms~3ms
/// @param addr 写的起始地址
/// @param data 数据缓冲区
/// @param len 写入的长度,注意一次最大256Bytes
/// @return 0表示操作成功
int sDRV_W25QxxJV_WritePage(uint32_t addr,uint8_t* data,uint16_t len){
    return write_bytes(addr,data,len);
}

/// @brief 读数据
/// @param addr 读的起始地址
/// @param data 数据缓冲区
/// @param len 读取的长度,无限制
/// @return 0表示操作成功
int sDRV_W25QxxJV_ReadBytes(uint32_t addr,uint8_t* data,uint32_t len){
    return read_bytes(addr,data,len);
}

/// @brief 扇区擦除,耗时45ms~400ms
/// @param sector_index 扇区索引
/// @return 0表示操作成功
int sDRV_W25QxxJV_SectorErase4KB(uint32_t sector_index){
    if(sector_index > SDRV_W25Q128JV_SECTOR_MAX_INDEX){
        return -1;
    }

    uint32_t sector_begin_addr = (sector_index) * SDRV_W25Q128JV_SECTOR_SIZE;

    if(set_write_enable() != 0){
        return -2;
    }

    //要等待busy位
    if(wait_busy(1000) != 0){
        return -3;
    }

    QSPI_CommandTypeDef sCommand = {0};
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = CMD_SECTOR_ERASE_4KB;
    sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address = sector_begin_addr;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_NONE;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = 0;

    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return -4;
    }

    //要等待busy位
    if(wait_busy(5000) != 0){
        return -5;
    }

    return 0;
}

/// @brief 32KB块擦除,也就是认为一个块为32KB,索引也是以32KB为基准,耗时120~1600ms
/// @param sector_index block索引
/// @return 0表示操作成功
int sDRV_W25QxxJV_BlockErase32KB(uint32_t block_index){
    if(block_index > SDRV_W25Q128JV_BLOCK_MAX_INDEX * 2){
        return -1;
    }

    uint32_t block_begin_addr = block_index * (SDRV_W25Q128JV_BLOCK_SIZE / 2);

    if(set_write_enable() != 0){
        return -2;
    }

    //要等待busy位
    if(wait_busy(1000) != 0){
        return -3;
    }

    QSPI_CommandTypeDef sCommand = {0};
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = CMD_BLOCK_ERASE_32KB;
    sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address = block_begin_addr;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_NONE;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = 0;

    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return -4;
    }

    return 0;
}


/// @brief 块擦除,64KB,耗时150~2000ms
/// @param sector_index block索引
/// @return 0表示操作成功
int sDRV_W25QxxJV_BlockErase64KB(uint32_t block_index){
    if(block_index > SDRV_W25Q128JV_BLOCK_MAX_INDEX){
        return -1;
    }

    uint32_t block_begin_addr = block_index * (SDRV_W25Q128JV_BLOCK_SIZE);

    if(set_write_enable() != 0){
        return -2;
    }

    //要等待busy位
    if(wait_busy(1000) != 0){
        return -3;
    }

    QSPI_CommandTypeDef sCommand = {0};
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = CMD_BLOCK_ERASE_64KB;
    sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
    sCommand.Address = block_begin_addr;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = QSPI_DATA_NONE;
    sCommand.DummyCycles       = 0;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
    sCommand.NbData = 0;

    if(HAL_QSPI_Command(&hqspi, &sCommand, 1000) != HAL_OK){
        return -4;
    }

    return 0;
}


int sDRV_W25QxxJV_WriteBytes(uint32_t addr, uint8_t* buf, uint32_t len)
{
    uint32_t bytes_written = 0;  // 已写入的字节数
    uint32_t offset = 0;         // 用于缓冲区的偏移量

    // // 1. 确保写入前目标区域已经被擦除
    // uint32_t sector_index = addr / 4096;  // 计算对应的扇区
    // int erase_ret = sDRV_W25QxxJV_SectorErase4KB(sector_index);
    // if (erase_ret != 0)
    // {
    //     return -1;  // 擦除失败
    // }

    // 等待擦除完成
    while (sDRV_W25QxxJV_IsBusy())
    {
        // 等待擦除完成
    }

    // 2. 处理写入数据，分段进行写入
    while (bytes_written < len)
    {
        // 计算每次写入的长度，最多写入 256 字节（即一个页的最大写入长度）
        uint32_t write_size = (len - bytes_written) > 256 ? 256 : (len - bytes_written);

        // 3. 写入数据
        int ret = sDRV_W25QxxJV_WritePage(addr + bytes_written, &buf[offset + bytes_written], write_size);
        if (ret != 0)
        {
            return -2;  // 写入失败
        }

        // 等待写入完成
        while (sDRV_W25QxxJV_IsBusy())
        {
            // 等待写入完成
        }

        // 更新已写入的字节数
        bytes_written += write_size;
    }

    // 写入成功
    return 0;
}

// void sDRV_W25QxxJV_WriteBytes(uint32_t begin_addr,uint8_t* pData,uint16_t number){
    
//     //sHMI_Debug_Printf("__WRITE BYTES ADDR: %#X ,NUMBER: %#X\n",begin_addr,number);
//     uint16_t page_remain_bytes = 256 - begin_addr % 256;  //当前页还有多少个空余字节
//     if(number <= page_remain_bytes){
//         page_remain_bytes = number; //一页剩下的空间能写完
//     }
//     while(1){
//         sDRV_W25QxxJV_WritePage(begin_addr,pData,page_remain_bytes);
//         // sDrv_W25Qxx_WritePage(pData,begin_addr,page_remain_bytes);
//         if(number == page_remain_bytes){
//             break;      //写入结束
//         }else{
//             pData += page_remain_bytes;
//             begin_addr += page_remain_bytes;
            
//             number -= page_remain_bytes;
//             if(number > 256){
//                 page_remain_bytes = 256;
//             }else{
//                 page_remain_bytes = number;
//             }
//         }
        
//     }
// }



/// @brief 全片擦除,需要很长的时间(真的很长! 40~200s不等,注意单位是秒哦)
/// @param is_blocking 是否阻塞
/// @return 0表示操作成功
int sDRV_W25QxxJV_ChipErase(bool is_blocking){
    return send_command(CMD_CHIP_ERASE);
}

/// @brief 检查器件是否处于忙状态
/// @return 是否忙
bool sDRV_W25QxxJV_IsBusy(){
    uint8_t sr1 = read_register(CMD_READ_STATUS_REGISTER_1);
    return sr1 & STATUS_REGISTER_1_MSK_BUSY;
}


