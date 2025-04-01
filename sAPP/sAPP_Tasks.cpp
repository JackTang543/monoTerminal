#include "sAPP_Tasks.hpp"



static void print_tasks_mang();



static void task_dev(void* param){
    dbg_printf("inited\n");

    for(;;){
        sGBD_Handler();
        BinOutDrv.update();

        int16_t val = sDRV_EC11_GetEncoder();
        if(val != 0){
            dbg_printf("Encoder:%d\n",val);
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}


void tasks_mang(void* param){

    for(;;){
        print_tasks_mang();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void sAPP_Tasks_CreateAll(){
    xTaskCreate(task_dev      , "devices"      , 1024 / sizeof(int), NULL, 2, NULL);
    // xTaskCreate(tasks_mang     , "TaskMang"     , 2048 / sizeof(int), NULL, 1, NULL);


}




static void print_tasks_mang(){
    const char task_state[]={'r','R','B','S','D'};
    volatile UBaseType_t uxArraySize, x;
    unsigned portSHORT ulTotalRunTime,ulStatsAsPercentage;
    /* 获取任务总数目 */
    uxArraySize = uxTaskGetNumberOfTasks();
    sBSP_UART_Debug_Printf("当前任务数量:%d\n",uxArraySize);
    TaskStatus_t pxTaskStatusArray[16];
    /*获取每个任务的状态信息 */
    uxArraySize = uxTaskGetSystemState(pxTaskStatusArray,uxArraySize,&ulTotalRunTime);
    sBSP_UART_Debug_Printf("任务名        状态       ID       优先级       堆栈剩余      CPU使用率\n");
    /* 避免除零错误 */
    if( ulTotalRunTime > 0 ){
        /* 将获得的每一个任务状态信息部分的转化为程序员容易识别的字符串格式 */
        for(x = 0; x < uxArraySize; x++){
            char tmp[128];
            /* 计算任务运行时间与总运行时间的百分比。*/
            ulStatsAsPercentage =(uint16_t)(pxTaskStatusArray[x].ulRunTimeCounter)*100 / ulTotalRunTime;
 
            if( ulStatsAsPercentage > 0UL ){
               sprintf(tmp,"%-15s%-10c%-10lu%-12lu%-12d%d%%",pxTaskStatusArray[ x].pcTaskName,task_state[pxTaskStatusArray[ x ].eCurrentState],
                                                             pxTaskStatusArray[ x ].xTaskNumber,pxTaskStatusArray[ x].uxCurrentPriority,
                                                             pxTaskStatusArray[ x ].usStackHighWaterMark,ulStatsAsPercentage);
            }
            else{
                /* 任务运行时间不足总运行时间的1%*/
                sprintf(tmp,"%-15s%-10c%-10lu%-12lu%-12dt<1%%",pxTaskStatusArray[x ].pcTaskName,task_state[pxTaskStatusArray[ x ].eCurrentState],
                                                               pxTaskStatusArray[ x ].xTaskNumber,pxTaskStatusArray[ x].uxCurrentPriority,
                                                               pxTaskStatusArray[ x ].usStackHighWaterMark);               
            }
           sBSP_UART_Debug_Printf("%s\n",tmp);
        }
    }
    sBSP_UART_Debug_Printf("任务状态:   r-运行  R-就绪  B-阻塞  S-挂起  D-删除\n");
}







