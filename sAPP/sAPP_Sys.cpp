#include "sAPP_Sys.hpp"

#include "main.hpp"



int sAPP_SYS_SystemInit(){
    int ret = 0;


    //初始化HAL
    if(HAL_Init() != HAL_OK){
        // dbg_warn("")
        Error_Handler();
    }

    if(sBSP_RCC_Init() != 0){
        
        Error_Handler();
    }


    cm_backtrace_init(APP_NAME,HARDWARE_VERSION,SOFEWARE_VERSION);


    return ret;
}


int sAPP_SYS_PeriphrialInit(){
    int ret = 0;




    return ret;
}











