#include "task.h"



static Task_Control Task_Ctl[] =
{
    // 执行与否 计数 重装 函数'
    {0, 1000, 1000, Task_Dht11},
    {0, 100, 100, Task_OLED},
};

u8 Tasks_Max = sizeof(Task_Ctl) / sizeof(Task_Ctl[0]);

/// @brief 任务标志位回调函数
/// @param 空
void Task_Flag_Callback(void)
{
    u8 i;
    for (i = 0; i < Tasks_Max; i++) // 遍历任务数组
    {
        if (Task_Ctl[i].TIMCOUNT) // 定时器计数
        {
            Task_Ctl[i].TIMCOUNT--;        // 定时器计数自减
            if (Task_Ctl[i].TIMCOUNT == 0) // 计数为零，允许函数执行
            {
                Task_Ctl[i].TIMCOUNT = Task_Ctl[i].REVALUE; // 重装定时器计数值
                Task_Ctl[i].run      = 1;                   // 允许函数执行
            }
        }
    }
}
/// @brief 任务执行回调函数
/// @param  void
void Task_Start_Callback(void)
{
    u8 i;
    for (i = 0; i < Tasks_Max; i++)
    {
        if (Task_Ctl[i].run == 1)
        {   
            Task_Ctl[i].run = 0;
            Task_Ctl[i].TaskHook();
            
        }
    }
}
