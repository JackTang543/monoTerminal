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
#include "sAPP_OutputDev.hpp"
#include "sAPP_Sys.hpp"

/*library layer*/
#include "sBinOutDev.hpp"
#include "sGenBtnDrv2.h"

/*middleware layer*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "usb_device.h"
#include "cm_backtrace.h"

/*driver layer*/
#include "sDRV_EC11.h"
#include "sDRV_W25QxxJV.h"

/*BSP layer*/
#include "sBSP_RCC.h"
#include "sBSP_UART.h"
#include "sBSP_SPI.h"
#include "sBSP_TIM.h"
#include "sBSP_QSPI.h"











