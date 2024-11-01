#include "Timer.h"
#include "OLED.h"
#include "task.h"
extern float Get_humi, Get_tem;
void Timer0_Isr(void) interrupt 1
{   
    static u16 count = 0;
    Task_Flag_Callback();
    count++;
    if (count == 5000) {
        OLED_ShowNum(32, 0, Get_tem);
        OLED_ShowNum(40, 2, Get_humi);
        count = 1;
    }
}