#ifndef __TASK_H__
#define __TASK_H__

#include "Config.h"
#include "OLED.h"
#include "CK.H"
#include "Delay.H"
#include "Dht.h"
#include "Timer.h"
typedef struct
{
    u8 run;//任务执行与否
    u16 TIMCOUNT;//定时器倒数计数
    u16 REVALUE;//定时器重装值
    void (*TaskHook) (void);
} Task_Control;


void Task_Flag_Callback(void);
void Task_Start_Callback(void);


#endif