#include <STC8G.H>
#include "OLED.h"

void main(void)
{
	P_SW2 |= 0x80; 
	init_IO();
	init_IIC();
	OLED_Init();
	OLED_ShowString(0,0,"2024/9/20");
	OLED_ShowString(0,2,"Ishtar");
	while(1)
	{
		
	}
}
