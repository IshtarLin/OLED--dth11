#include <STC8G.H>
#include "OLED.h"
#include "CK.H"
#include "Delay.H"
#include "Dht.h"
#include "Timer.h"
float Get_humi=69.12,Get_tem=22.86;
void main(void)
{
	P_SW2 |= 0x80; 
	init_IO();
	init_IIC();
	OLED_Init();
	CK_Init();
	Dht_Init();
	Timer0_Init();
	Delay_ms(1000);
	OLED_ShowString(0,0,"Tem:");
	OLED_ShowString(0,2,"Humi:");
	OLED_ShowString(0,4,"CK_message:");
	while(1)
	{	
		CK_SendBack();
		OLED_ShowNum(32,0,Get_tem);
		OLED_ShowNum(40,2,Get_humi);
		
	}
	
}
void Timer0_Isr(void) interrupt 1
{
    static u16 count = 0;  
    count++;  
    if (count >= 5000) {//5s中断
        Dht_recivedat(&Get_humi,&Get_tem);; 
        count = 0;
    }  
}
