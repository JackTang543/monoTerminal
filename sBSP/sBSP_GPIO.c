#include "sBSP_GPIO.h"

#include "sDRV_EC11.h"

#include "defines.h"
#include "sAPP_Debug.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_PIN_8){
        sDRV_EC11_TrigProcess();
    }
    else if(GPIO_Pin == GPIO_PIN_9){
        sDRV_EC11_TrigProcess();
    }

}


bool sBSP_GPIO_GetEncoder1KEY(){
    return !!HAL_GPIO_ReadPin(KEY_ECKEY_GPIO,KEY_ECKEY_GPIO_PIN);
}

bool sBSP_GPIO_GetEnterKEY(){
    return !!HAL_GPIO_ReadPin(KEY_ENTER_GPIO,KEY_ENTER_GPIO_PIN);
}

bool sBSP_GPIO_GetBackKEY(){
    return !!HAL_GPIO_ReadPin(KEY_BACK_GPIO,KEY_BACK_GPIO_PIN);
}



