#pragma once


#include "stm32f4xx_hal.h"



#include "defines.h"



/*stand library*/
#include "stdio.h"
#include "string"
#include "math.h"

/*application layer*/
#include "sAPP_Debug.h"
#include "sAPP_Tasks.hpp"
#include "sAPP_Btns.hpp"
#include "sAPP_Sys.hpp"
#include "sAPP_GUI.hpp"

/*library layer*/
#include "sBinOutDev.hpp"
#include "sGenBtnDrv2.h"
#include "sDWTLib.hpp"
#include "sG2D.hpp"
#include "lvgl.h"

/*middleware layer*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "usb_device.h"
#include "cm_backtrace.h"
#include "sUtils.h"
#include "usb_device.h"


/*driver layer*/
#include "sDRV_EC11.h"
#include "sDRV_W25QxxJV.h"
#include "sDRV_ST7305.h"
#include "sDRV_AHT20.h"

/*BSP layer*/
#include "sBSP_RCC.h"
#include "sBSP_UART.h"
#include "sBSP_SPI.h"
#include "sBSP_TIM.h"
#include "sBSP_QSPI.h"
#include "sBSP_GPIO.h"
#include "sBSP_DMA.h"
#include "sBSP_FI2C.h"










