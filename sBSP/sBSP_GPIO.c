#include "sBSP_GPIO.h"

#include "sDRV_EC11.h"

#include "defines.h"
#include "sAPP_Debug.h"



int sBSP_GPIO_Init(){
    /*DS3231 Power control*/
    {
        DS_PWR_GPIO_CLK_EN();
        GPIO_InitTypeDef gpio = {
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed= GPIO_SPEED_LOW,
            .Pin  = DS_PWR_GPIO_PIN,
        };
        HAL_GPIO_Init(DS_PWR_GPIO,&gpio);
    }
    /*TP4056X CHRG state*/
    {
        TP_CHRG_GPIO_CLK_EN();
        GPIO_InitTypeDef gpio = {
            .Mode = GPIO_MODE_INPUT,
            .Pull = GPIO_PULLUP,
            .Speed= GPIO_SPEED_LOW,
            .Pin  = TP_CHRG_GPIO_PIN,
        };
        HAL_GPIO_Init(TP_CHRG_GPIO,&gpio);
    }
    /*TP4056X STDBY state*/
    {
        TP_STBY_GPIO_CLK_EN();
        GPIO_InitTypeDef gpio = {
            .Mode = GPIO_MODE_INPUT,
            .Pull = GPIO_PULLUP,
            .Speed= GPIO_SPEED_LOW,
            .Pin  = TP_STBY_GPIO_PIN,
        };
        HAL_GPIO_Init(TP_STBY_GPIO,&gpio);
    }
    /*DS3231 Interrupt*/
    {
        
    }


    return 0;
}


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



bool sBSP_GPIO_GetTP4056XCHRG(){
    return !!HAL_GPIO_ReadPin(TP_CHRG_GPIO,TP_CHRG_GPIO_PIN); 
}

bool sBSP_GPIO_GetTP4056XSTDBY(){   
    return !!HAL_GPIO_ReadPin(TP_STBY_GPIO,TP_STBY_GPIO_PIN); 
}

void sBSP_GPIO_SetDS3231Power(bool is_on){
    HAL_GPIO_WritePin(DS_PWR_GPIO,DS_PWR_GPIO_PIN,!is_on);
}



