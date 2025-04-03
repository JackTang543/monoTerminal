#include "sAPP_Btns.hpp"




#include "main.hpp"


#define KEY_ENTER_GPIO_CLK_EN __GPIOB_CLK_ENABLE
#define KEY_ENTER_GPIO        GPIOB
#define KEY_ENTER_GPIO_PIN    GPIO_PIN_13

#define KEY_BACK_GPIO_CLK_EN  __GPIOB_CLK_ENABLE
#define KEY_BACK_GPIO         GPIOB
#define KEY_BACK_GPIO_PIN     GPIO_PIN_12

#define KEY_ECKEY_GPIO_CLK_EN __GPIOB_CLK_ENABLE
#define KEY_ECKEY_GPIO        GPIOB
#define KEY_ECKEY_GPIO_PIN    GPIO_PIN_15




static bool get_lv(uint8_t btn_id){
    
    if(btn_id == SGBD_KEY_ENTER_ID){
        return !!HAL_GPIO_ReadPin(KEY_ENTER_GPIO,KEY_ENTER_GPIO_PIN);
    }
    else if(btn_id == SGBD_KEY_BACK_ID){
        return !!HAL_GPIO_ReadPin(KEY_BACK_GPIO,KEY_BACK_GPIO_PIN);
    }
    else if(btn_id == SGBD_KEY_ECKEY_ID){
        return !!HAL_GPIO_ReadPin(KEY_ECKEY_GPIO,KEY_ECKEY_GPIO_PIN);
    }
    return false;
}



static void trig(uint8_t btn_id,ev_flag_t btn_ev){
    if(btn_ev == ev_dp){
        
    }
    else if(btn_ev == ev_pres || btn_ev == ev_lp_loop){
        if(btn_id == SGBD_KEY_ENTER_ID){

        }
        else if(btn_id == SGBD_KEY_BACK_ID){

        }
        else if(btn_id == SGBD_KEY_ECKEY_ID){

        }
    }

    //打印按键id的事件
    if(btn_ev == ev_pres){
        BinOutDrv.startPulse(BOD_BUZZER_ID,50);
        // dbg_println("btn_id:%u,press",btn_id);


    }
    else if(btn_ev == ev_rlsd){
    //     dbg_println("btn_id:%u,released",btn_id);

    }
    else if(btn_ev == ev_dp){
        // dbg_println("btn_id:%u,double press",btn_id);

    }
    else if(btn_ev == ev_dp_rlsd){
        // dbg_println("btn_id:%u,double released",btn_id);

    }
    else if(btn_ev == ev_lp){
        // dbg_println("btn_id:%u,long press",btn_id);

    }
    else if(btn_ev == ev_lp_rlsd){
        // dbg_println("btn_id:%u,long press released",btn_id);

        
    }
    else if(btn_ev == ev_lp_loop){
        // dbg_println("btn_id:%u,long press loop",btn_id);

    }



    // dbg_println("btn_id:%u,btn_ev:%u",btn_id,btn_ev);


    
}


void sAPP_Btns_Init(){
    KEY_ENTER_GPIO_CLK_EN();
    KEY_BACK_GPIO_CLK_EN();
    KEY_ECKEY_GPIO_CLK_EN();

    GPIO_InitTypeDef gpio = {0};
    gpio.Mode      = GPIO_MODE_INPUT;
    gpio.Pull      = GPIO_PULLUP;
    gpio.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = 0;

    gpio.Pin       = KEY_ENTER_GPIO_PIN;
    HAL_GPIO_Init(KEY_ENTER_GPIO, &gpio);
    gpio.Pin       = KEY_BACK_GPIO_PIN;
    HAL_GPIO_Init(KEY_BACK_GPIO, &gpio);
    gpio.Pin       = KEY_ECKEY_GPIO_PIN;
    HAL_GPIO_Init(KEY_ECKEY_GPIO, &gpio);


    btn_init_t btn_init;
    memset(&btn_init, 0, sizeof(btn_init));

    btn_init.en = 1;                //使能此按键
    btn_init.lv_rev = lv_non_reverse;   //空闲时的电平反转
    btn_init.dp_mode = dp_disable;   //禁用双击,可提高连续单击速度
    btn_init.lp_loop_pridt = 150;   //设置长按循环触发间隔
    btn_init.lp_trig_waitt = 600;  //设置长按触发时间
    btn_init.dp_prid_waitt = 200;   //设置最大等待双击时间
    sGBD_SetAllBtnEnable(1);        //设置所有按键使能
    sGBD_SetAllBtnMode(&btn_init);  //装载btn_init的配置参数
    sGBD_Init(get_lv,trig,HAL_GetTick);

    
    
}


