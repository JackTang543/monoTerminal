#include "sAPP_GUI.hpp"


#include "main.hpp"

#include "ui.h"


static void lvgl_log_cb(lv_log_level_t level, const char* buf){
    log_printfln("LV:%d :%s",level, buf);
}


#include "sG2D.hpp"


IRAM_ATTR static void lvgl_flush_cb(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
    // uint16_t *buf16 = (uint16_t *)px_map;
    uint8_t *buf8 = (uint8_t *)px_map;


    
    int32_t x, y;
    uint16_t color;
    
    // oled.clear();
    // oled.setAll(0);


    // 遍历区域内的每个点
    for (y = area->y1; y <= area->y2; y++) {
        for (x = area->x1; x <= area->x2; x++) {
            // 获取颜色值
            color = buf8[(y - area->y1) * (area->x2 - area->x1 + 1) + (x - area->x1)];

            if(x > SDRV_ST7305_W || y > SDRV_ST7305_H){
                log_info("x=%d,y=%d,1",x,y);
            }

            if(color > 60){
                

                oled.setDot(x, y, 1);
            }else{
                oled.setDot(x, y, 0);
            }

            px_map++;
        }
    }


    // oled.revArea(10,10,100,100);

    oled.handler();

    // 通知 LVGL 刷新完成
    lv_display_flush_ready(disp);
}


lv_obj_t* label;
lv_obj_t* label1;

lv_indev_t* indev;
static lv_display_t* screen;



static void encoder1_read_cb(lv_indev_t * _indev, lv_indev_data_t * data){
    if(_indev == indev){

        data->enc_diff = sDRV_EC11_GetEncoder();
        if(data->enc_diff != 0){
            BinOutDrv.startPulse(BOD_BUZZER_ID,20);
        }
        static lv_indev_state_t prev_state = LV_INDEV_STATE_REL;
        data->state = (lv_indev_state_t)!sBSP_GPIO_GetEncoder1KEY();
        if(prev_state == LV_INDEV_STATE_REL && data->state == LV_INDEV_STATE_PR) {
            BinOutDrv.startPulse(BOD_BUZZER_ID,30);
        }
        prev_state = data->state;
    }
}

void sAPP_GUI_Init(){
    /*初始化LVGL*/
    lv_init();
    //设置滴答定时器回调
    lv_tick_set_cb(HAL_GetTick);
    //注册日志回调
    lv_log_register_print_cb(lvgl_log_cb);

    //创建屏幕
    screen = lv_display_create(SDRV_ST7305_W,SDRV_ST7305_H);
    //屏幕旋转
    lv_display_set_rotation(screen,LV_DISPLAY_ROTATION_0);
    //注册屏幕刷新回调
    lv_display_set_flush_cb(screen,lvgl_flush_cb);
    
    /*屏幕绘制缓冲区*/
    static uint8_t ALIGN_ATTR(4) lvgl_buf1[0x10000];
    //单64K缓冲区,180MHz 31FPS 36%CPU delay20ms
    lv_display_set_buffers(screen,(void*)lvgl_buf1,NULL,sizeof(lvgl_buf1),LV_DISPLAY_RENDER_MODE_FULL);

    //单0x1FFFF缓冲区 FMC最快的时序,180MHz 32FPS 35%CPU delay20ms
    // lv_display_set_buffers(screen,(void*)SBSP_FMC_SRAM_128KB_3,NULL,0x10000,LV_DISPLAY_RENDER_MODE_PARTIAL);

    //双64K缓冲区,180MHz 31FPS 36%CPU delay20ms
    // lv_display_set_buffers(screen,(void*)SBSP_FMC_SRAM_128KB_3,(void*)(SBSP_FMC_SRAM_128KB_3 + 0x10000),0x10000,LV_DISPLAY_RENDER_MODE_PARTIAL);

    // lv_display_set_buffers(screen,(void*)SBSP_FMC_SRAM_128KB_0,0,SDRV_ILI9488_H * SDRV_ILI9488_W * 2,LV_DISPLAY_RENDER_MODE_PARTIAL);


    
    



    // group = lv_group_create();
    // lv_group_set_default(group);

    indev = lv_indev_create();
    lv_indev_set_type(indev,LV_INDEV_TYPE_ENCODER);
    lv_indev_set_read_cb(indev,encoder1_read_cb);

    // indev_touch = lv_indev_create();
    // lv_indev_set_type(indev_touch,LV_INDEV_TYPE_POINTER);
    // lv_indev_set_read_cb(indev_touch,touch_read_cb);


    // lv_obj_t * label2 = lv_label_create(lv_screen_active());
    // lv_obj_set_width(label2, 150);
    // lv_label_set_text(label2, "By Sightseer.");
    // lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_text_color(label2,lv_color_black(),0);
    

}


void sAPP_GUI_WeightsInit(){
    ui_init();
    lv_indev_set_group(indev,groups.group);
}






