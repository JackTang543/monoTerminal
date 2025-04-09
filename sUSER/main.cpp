#include "main.hpp"




typedef enum{

}mono_is_charging;




mono_t mono;



/**
 * v1硬件验证项目:
 * 编码器 完成
 * 两个按键 完成
 * TP4056充电状态反馈引脚 
 * 串口 完成
 * swd下载 完成
 * 蜂鸣器 完成
 * todo sd卡sdio 4线0
 * w25q128 quadSPI 完成
 * 2.9寸单色LCD
 * ds3231
 * aht20
 * icm42688
 * bmp280
 * todo adc测量电池电压
 *  
 */



int main(){
    sAPP_SYS_KernelInit();
    sAPP_SYS_SystemInit();


    log_info("系统主频:%uMHz",HAL_RCC_GetSysClockFreq() / 1'000'000);
    log_info("monoTerminal 初始化完成");


    sAPP_GUI_Init();
    sAPP_GUI_WeightsInit();

    // GPIO_InitTypeDef gpio = {0};
    // __GPIOB_CLK_ENABLE();
    // gpio.Mode = GPIO_MODE_OUTPUT_PP;
    // gpio.Pull = GPIO_NOPULL;
    // gpio.Speed = GPIO_SPEED_LOW;
    // gpio.Pin = GPIO_PIN_7;
    // HAL_GPIO_Init(GPIOB,&gpio);
    // HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);



    sAPP_Tasks_CreateAll();

    usb_device_init();


    log_info("Current free heap size: %u bytes", (unsigned int)xPortGetFreeHeapSize());
    log_info("FreeRTOS启动任务调度");
    vTaskStartScheduler();

    int i = 0;

    while(1){
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);

        sDRV_AHT20_StartMeasure();


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
        float temp,humi;
        sDRV_AHT20_GetMeasure(&temp,&humi);
        log_printfln("temp=%.2f,humi=%.2f",temp,humi);
        i++;
        
    }
}






    // log_printfln("读取地址:0x1000,len=4096");
    // sDRV_W25QxxJV_ReadBytes(0x1000,buf,4096);
    // //一行32个数据,多行打印
    // for(int i = 0; i < 4096; i++){
    //     if(i % 32 == 0){
    //         log_printfln("");
    //     }
    //     log_printf("0x%02X ",buf[i]);
    // }
    
    // log_printfln("写入地址:0x1000,len=4096,数据0x5D");
    // for(int i = 0;i < 4096;i++){
    //     buf[i] = 0x5D;
    // }

    // // sDRV_W25QxxJV_WritePage(0x1000,buf,4096);
    // sDRV_W25QxxJV_SectorErase4KB(1);
    // sDRV_W25QxxJV_WriteBytes(0x1000,buf,4096);

    // for(int i = 0;i < 4096;i++){
    //     buf[i] = 0;
    // }

    // log_printfln("读地址:0x1000,len=4096");
    // {
    //     sDRV_W25QxxJV_ReadBytes(0x1000,buf,4096);
    //     //一行32个数据,多行打印
    //     for(int i = 0; i < 4096; i++){
    //         if(i % 32 == 0){
    //             log_printfln("");
    //         }
    //         log_printf("0x%02X ",buf[i]);
    //     }
    
    // }

    // for(int i = 0;i < 4096;i++){
    //     buf[i] = 0;
    // }

    // sDRV_W25QxxJV_SectorErase4KB(1);

    // log_printfln("擦除扇区1,读地址:0x1000,len=4096");
    // sDRV_W25QxxJV_ReadBytes(0x1000,buf,4096);
    // //一行32个数据,多行打印
    // for(int i = 0; i < 4096; i++){
    //     if(i % 32 == 0){
    //         log_printfln("");
    //     }
    //     log_printf("0x%02X ",buf[i]);
    // }





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

