#include "sBSP_GPIO.h"

#include "sDRV_EC11.h"


#include "sAPP_Debug.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_PIN_8){
        sDRV_EC11_TrigProcess();
    }
    else if(GPIO_Pin == GPIO_PIN_9){
        sDRV_EC11_TrigProcess();
    }

}



