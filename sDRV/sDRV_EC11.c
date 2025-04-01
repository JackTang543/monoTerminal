#include "sDRV_EC11.h"




static int16_t ec11_1_diff;




void Aio_inter(void)
{
  
}

void sDRV_EC11_Init(){
    SDRV_EC11_1A_CLK_EN();
    SDRV_EC11_1B_CLK_EN();

    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_LOW;
    gpio.Pin = SDRV_EC11_1A_PIN;
    HAL_GPIO_Init(SDRV_EC11_1A_PORT,&gpio);

    gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_LOW;
    gpio.Pin = SDRV_EC11_1B_PIN ;
    HAL_GPIO_Init(SDRV_EC11_1B_PORT,&gpio);

    NVIC_SetPriority(EXTI9_5_IRQn,4);
    NVIC_EnableIRQ(EXTI9_5_IRQn);


}


void sDRV_EC11_TrigProcess(){
  
    static bool CW_1 = false;
    static bool CW_2 = false;
    // 只要处理一个脚的外部中断--上升沿和下降沿
    int alv = HAL_GPIO_ReadPin(SDRV_EC11_1A_PORT,SDRV_EC11_1A_PIN);
    int blv = HAL_GPIO_ReadPin(SDRV_EC11_1B_PORT,SDRV_EC11_1B_PIN);

    static int flag = 0;
    if (flag == 0 && alv == 0){
        CW_1 = blv;
        flag = 1;
    }
    if (flag && alv){
        CW_2 = !blv;  // 取反是因为 alv, blv必然异步，一高一低。
        if (CW_1 && CW_2){
            ec11_1_diff--;
        }
        if (CW_1 == false && CW_2 == false){
            ec11_1_diff++;
        }
        flag = 0;
    }
}


int16_t sDRV_EC11_GetEncoder(){
    int16_t tmp = ec11_1_diff;
    ec11_1_diff = 0;
    return tmp;
}




