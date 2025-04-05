#pragma once
#ifdef __cplusplus
extern "C" {
#endif



#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>





#include "defines.h"
#ifdef RELEASE_CODE_MODE

#else
#define SUTILS_OUTPUT_ENABLE
#endif





//PI
#define M_PI            (3.14159f)
//EXP
#define M_EXP           (2.71828f)

//重力常数
#define M_GRAVITY       (9.81398f)

//度转弧度
#define DEG2RAD	    	(0.017453f)
//弧度转度
#define RAD2DEG		    (57.29578f)

//工程转速转弧度每秒
#define RPM2RADPS       (0.104720f)
//弧度每秒转工程转速
#define RADPS2RPM       (9.549297f)



//把一个浮点数从一个范围转换到另一个范围
float sut_fmap(float x, float in_min, float in_max, float out_min, float out_max);
//把一个整数从一个范围转换到另一个范围
uint32_t sut_imap(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);
//快速求平方根倒数
float sut_isqrt(float x);
//从气压转换到高度,返回单位m
float sut_press2alt(float press_pa);
//比较两个浮点数的大小,可以用于qsort的比较函数,返回1: a>b, 0: a==b, -1: a<b
int sut_fcmp(const void *a, const void *b);
//比较两个浮点数是否相等
int sut_fequal(float fa, float fb);
//获得航向角的差值
float sut_headingdiff(float heading1, float heading2);
//浮点数限幅
void sut_fconstrain(float *val, float min, float max);
//伽马矫正
float sut_gammacorr(float percent_brightness);



#ifdef SUTILS_OUTPUT_ENABLE
    #define log_assert   sUtils_LogAssert
    #define log_error    sUtils_LogError
    #define log_warn     sUtils_LogWarn
    #define log_info     sUtils_LogInfo
    #define log_printf   sUtils_LogPrintf
    #define log_printfln sUtils_LogPrintfln
#else
    #define log_assert(...)   ((void)0)
    #define log_error(...)    ((void)0)
    #define log_warn(...)     ((void)0)
    #define log_info(...)     ((void)0)
    #define log_printf(...)   ((void)0)
    #define log_printfln(...) ((void)0)
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



