#include "STC8G.H"
#include "System_init.h"
#include "task.h"


void main(void)
{
    P_SW2 |= 0x80;
    SYS_INIT();
    while (1) {
        Task_Start_Callback();
    }
}
