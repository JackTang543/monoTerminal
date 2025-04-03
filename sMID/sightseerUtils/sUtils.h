#pragma once
#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"







#define SUTILS_OUTPUT_ENABLE








#ifdef SUTILS_OUTPUT_ENABLE
    #define log_assert   sUtils_LogAssert
    #define log_error    sUtils_LogError
    #define log_warn     sUtils_LogWarn
    #define log_info     sUtils_LogInfo
    #define log_printf   sUtils_LogPrintf
    #define log_printfln sUtils_LogPrintfln
#else
    #define log_assert   ((void)0)
    #define log_error    ((void)0)
    #define log_warn     ((void)0)
    #define log_info     ((void)0)
    #define log_printf   ((void)0)
    #define log_printfln  ((void)0)
#endif






void sUtils_LogAssert(const char *fmt,...);
void sUtils_LogError(const char *fmt,...);
void sUtils_LogWarn(const char *fmt,...);
void sUtils_LogInfo(const char *fmt,...);
void sUtils_LogPrintf(const char *fmt,...);
void sUtils_LogPrintfln(const char *fmt,...);






#ifdef __cplusplus
}
#endif



