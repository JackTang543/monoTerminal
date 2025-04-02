#include "sUtils.h"


#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static char printf_fmt_buf[512];

#ifdef SUTILS_OUTPUT_ENABLE
    #define log_assert  sUtils_LogAssert
    #define log_error   sUtils_LogError
    #define log_warn    sUtils_LogWarn
    #define log_info    sUtils_LogInfo
    #define log_print   sUtils_LogPrintf
    #define log_println sUtils_LogPrintln
#else
    #define log_assert  ((void)0)
    #define log_error   ((void)0)
    #define log_warn    ((void)0)
    #define log_info    ((void)0)
    #define log_print   ((void)0)
    #define log_println ((void)0)
#endif



#include "sBSP_UART.h"


//log输出接口
static void log_output(char* str){
    #ifdef SUTILS_OUTPUT_ENABLE
        sBSP_UART_Debug_Printf("%s",str);
    #endif
}



void sDBG_Printf(const char *fmt,...){
    va_list args;
    va_start(args, fmt);
    vsprintf(printf_fmt_buf, fmt, args);
    va_end(args);

    log_output(printf_fmt_buf);
}






















