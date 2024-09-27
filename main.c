#include <STC8G.H>
#include "OLED.h"
#include "CK.H"
#include "Delay.H"
void main(void)
{
	P_SW2 |= 0x80; 
	init_IO();
	init_IIC();
	OLED_Init();
	CK_Init();
	Delay500ms();
	OLED_ShowString(0,0,"2024/9/27");
	OLED_ShowString(0,2,"Ishtar");
	OLED_ShowString(0,4,"CK_message:");
	while(1)
	{
		CK_Read();
		

		
	}
	
}
