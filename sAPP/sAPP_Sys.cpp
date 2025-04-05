#include "sAPP_Sys.hpp"

#include "main.hpp"



int sAPP_SYS_KernelInit(){
    int ret = 0;

    //初始化HAL
    if(HAL_Init() != HAL_OK){
        Error_Handler();
    }

    if(sBSP_RCC_Init() != 0){
        Error_Handler();
    }

    // sBSP_UART_Debug_Init(115200);
    sBSP_UART_Debug_Init(512000);

    cm_backtrace_init(APP_NAME,HARDWARE_VERSION,SOFEWARE_VERSION);


    sAPP_SYS_OutputDevInit();

    return ret;
}


int sAPP_SYS_SystemInit(){
    int ret = 0;

    sBSP_TIM_BuzzerInit();
    sBSP_TIM_BuzzerSetFreq(2700);
    sBSP_TIM_BuzzerSet(50.0f);
    sBSP_TIM_BuzzerSetEN(1);

    HAL_Delay(10);


    sBSP_DMA_Init();


    sBSP_SPI_LCDInit();



    sBSP_QSPI_Init();

    sDRV_EC11_Init();
    sDRV_ST7305_Init();
    if(sDRV_W25QxxJV_Init() != 0){
        log_printfln("W25Qxx初始化失败");
    }


    sAPP_SYS_OutputDevInit();
    sAPP_Btns_Init();



    return ret;
}



static void buzzer_output_callback(uint16_t id,bool lv){
    if(id == BOD_BUZZER_ID){
        // sBSP_TIM_BuzzerSetEN(lv);
        sBSP_TIM_BuzzerSet(lv ? 50.0f : 0.0f);
    }
}


void sAPP_SYS_OutputDevInit(){
    BinOutDrv.init();
    BinOutDrv.addDev(BOD_LED_ID   ,GPIOB,GPIO_PIN_7);
    BinOutDrv.addDev(BOD_BUZZER_ID,buzzer_output_callback);

    
    BinOutDrv.confDevMode(BOD_LED_ID,sBOD::DEV_MODE::ASYMMETRIC_TOGGLE,sBOD::LEVEL::LOW);
    BinOutDrv.confDevMode(BOD_BUZZER_ID,sBOD::DEV_MODE::PULSE_HIGH,sBOD::LEVEL::LOW);

    BinOutDrv.confTime(BOD_LED_ID,1000,100);
    BinOutDrv.confTime(BOD_BUZZER_ID,100,50);

}











