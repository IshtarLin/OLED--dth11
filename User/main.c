#include "STC8G.H"
#include "System_init.h"
#include "task.h"
extern float Get_humi, Get_tem;

void main(void)
{
    P_SW2 |= 0x80;
    SYS_INIT();
    while (1) {
        Task_Start_Callback();
    }
}
void Timer0_Isr(void) interrupt 1
{
    Task_Flag_Callback();
    static int n = 10;
    n++;
    if (n == 5000) {
        OLED_ShowNum(32, 0, Get_tem);
        OLED_ShowNum(40, 2, Get_humi);
        n = 1;
    }
}
