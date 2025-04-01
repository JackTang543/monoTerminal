#include "sBSP_TIM.h"

#define __TIM_CALC_PSC(__TIM_CLK_FREQ,__TARGET_FREQ,__TIM_ARR_VAL) ((__TIM_CLK_FREQ / (__TARGET_FREQ * (__TIM_ARR_VAL + 1))) - 1)


void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle);


#define TIM11_CLK_FREQ  (100000000u)
TIM_HandleTypeDef htim11;
//PWM准确度0.1% 频率区间2Hz~100kHz
static const uint32_t tim11_arr_val = 1000 - 1;
static const uint32_t tim11_psc_val = 0;


void sBSP_TIM_BuzzerInit(){
    TIM_OC_InitTypeDef oc = {0};

    htim11.Instance = TIM11;
    htim11.Init.Prescaler = tim11_psc_val;
    htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim11.Init.Period = tim11_arr_val;
    htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim11);
    HAL_TIM_PWM_Init(&htim11);
    
    oc.OCMode = TIM_OCMODE_PWM1;
    oc.Pulse = 0;
    oc.OCPolarity = TIM_OCPOLARITY_HIGH;
    oc.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim11, &oc, TIM_CHANNEL_1);

    HAL_TIM_MspPostInit(&htim11);
}


void sBSP_TIM_BuzzerSetFreq(uint32_t freq_hz){
    assert_param(freq_hz <= 100000 | freq_hz >= 2);
    __HAL_TIM_SET_PRESCALER(&htim11,__TIM_CALC_PSC(TIM11_CLK_FREQ,freq_hz,tim11_arr_val));
}

void sBSP_TIM_BuzzerSetEN(bool is_en){
    is_en ? HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1) : HAL_TIM_PWM_Stop(&htim11,TIM_CHANNEL_1);
}

void sBSP_TIM_BuzzerSet(float percent){
    if(percent > 100.0f){percent = 100.0f;}
    if(percent < 0.0f)  {percent = 0.0f;}
    __HAL_TIM_SET_COMPARE(&htim11,TIM_CHANNEL_1,(uint32_t)(percent * ((tim11_arr_val + 1) / 100.0f)));
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle){
    if(tim_baseHandle->Instance==TIM11){
        __HAL_RCC_TIM11_CLK_ENABLE();
    }
}


void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle){

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(timHandle->Instance==TIM11){
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM11 GPIO Configuration
        PB9     ------> TIM11_CH1
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF3_TIM11;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }

}


