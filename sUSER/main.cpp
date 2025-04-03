#include "main.hpp"



#include "sDRV_ST7305.h"
#include "sG2D.hpp"



/**
 * v1硬件验证项目:
 * 编码器 完成
 * 两个按键 完成
 * todo TP4056充电状态反馈引脚 
 * 串口 完成
 * swd下载 完成
 * 蜂鸣器 完成
 * todo sd卡sdio 4线0
 * todo w25q128 quadSPI
 * 2.9寸单色LCD
 * todo ds3231
 * aht20
 * icm42688
 * bmp280
 * todo adc测量电池电压
 *  
 */


int main(){
    sAPP_SYS_KernelInit();
    sAPP_SYS_SystemInit();

    
    
    usb_device_init();


    sAPP_Btns_Init();

    sDRV_EC11_Init();

    sBSP_QSPI_Init();
    if(sDRV_W25QxxJV_Init() != 0){
        // dbg_println("W25Qxx初始化失败");
    }

    sAPP_Tasks_CreateAll();
    
    HAL_Delay(100);

    log_info("系统主频:%uMHz",HAL_RCC_GetSysClockFreq() / 1'000'000);

    log_info("monoTerminal 初始化完成");


    sBSP_SPI_LCDInit();
    sDRV_ST7305_Init();
    oled.init(SDRV_ST7305_W,SDRV_ST7305_H);
    sDRV_ST7305_SetInvShowMode(1);
    sDRV_ST7305_SetAll(0x0);

    sAPP_GUI_Init();
    sAPP_GUI_WeightsInit();


    


    GPIO_InitTypeDef gpio = {0};
    __GPIOB_CLK_ENABLE();
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_LOW;
    gpio.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB,&gpio);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);


    sAPP_Tasks_CreateAll();
    log_info("Current free heap size: %u bytes", (unsigned int)xPortGetFreeHeapSize());
    log_info("FreeRTOS启动任务调度");
    vTaskStartScheduler();

    int i = 0;

    while(1){
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);


        oled.setAll(0);
        oled.printf(10,10,"Hello,monoLCD i = %d",i);
        oled.printf(10,30,"The ST7305 is a single-chip controller/driver for small and medium");
        oled.printf(10,50,"ST7305 is controlled using a conventional mobile driver SoC interface w");
        oled.printf(10,70,"ST7305 driver SoC to have wide compatibility with various LCD and TFT types.");
        // oled.drawRectangle(100,100,300,160,1);
        oled.printf(50,120,"The built-in timing controller (TCON)");
        oled.revArea(100,100,200,150);
        // oled.revArea(1,1,10,10);
        oled.handler();





        HAL_Delay(100);
        i++;
        
    }
}










/*用于重载c++ new/delete分配器,让其指向FreeRTOS的内存管理策略*/
void* operator new(std::size_t size) {
    return pvPortMalloc(size);  // FreeRTOS memory allocation
}

void operator delete(void* ptr) noexcept {
    vPortFree(ptr);  // FreeRTOS memory free
}

void* operator new[](std::size_t size) {
    return pvPortMalloc(size);  // For array allocation
}

void operator delete[](void* ptr) noexcept {
    vPortFree(ptr);  // For array deallocation
}

