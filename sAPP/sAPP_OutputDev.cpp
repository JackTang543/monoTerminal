#include "sAPP_OutputDev.hpp"

#include "sBinOutDev.hpp"

#include "main.hpp"












void sAPP_OutputDev_Init(){
    BinOutDrv.init();
    BinOutDrv.addDev(BOD_LED_ID   ,GPIOB,GPIO_PIN_7);
    
    BinOutDrv.confDevMode(BOD_LED_ID,sBOD::DEV_MODE::ASYMMETRIC_TOGGLE,sBOD::LEVEL::LOW);
    BinOutDrv.confTime(BOD_LED_ID,1000,100);
}










