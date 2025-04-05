#pragma once
#ifdef __cplusplus
extern "C" {
#endif



/*如果定义,切换为发布模式,所有调试字符都不编译,无断言等提示*/
// #define RELEASE_CODE_MODE


#define APP_NAME                    "monoTerminal"
#define HARDWARE_VERSION            "v2"
#define SOFEWARE_VERSION            "v1.0"



//attribute:手动设置对齐
#define ALIGN_ATTR(__BYTES) __attribute__((aligned(__BYTES)))
//中断服务函数标识
#define ISR_ATTR            __attribute__((used))

#define IRAM_ATTR           __attribute__((section(".ramfunc")))

//attribute:Normal memory
#define IRAM1_ATTR     __attribute__((section("IRAM1")))
//attribute:Core coupled memory
#define IRAM2_ATTR     __attribute__((section("IRAM2")))
//attribute:flash memory
#define IROM_ATTR      __attribute__((section("IROM")))


#define KEY_ENTER_GPIO_CLK_EN __GPIOB_CLK_ENABLE
#define KEY_ENTER_GPIO        GPIOB
#define KEY_ENTER_GPIO_PIN    GPIO_PIN_13

#define KEY_BACK_GPIO_CLK_EN  __GPIOB_CLK_ENABLE
#define KEY_BACK_GPIO         GPIOB
#define KEY_BACK_GPIO_PIN     GPIO_PIN_12

#define KEY_ECKEY_GPIO_CLK_EN __GPIOB_CLK_ENABLE
#define KEY_ECKEY_GPIO        GPIOB
#define KEY_ECKEY_GPIO_PIN    GPIO_PIN_15






#ifdef __cplusplus
}
#endif



