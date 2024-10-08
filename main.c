#include <STC8G.H>
#include "OLED.h"
#include "CK.H"
#include "Delay.H"
#include "Dht.h"
void main(void)
{
	P_SW2 |= 0x80; 
	init_IO();
	init_IIC();
	OLED_Init();
	CK_Init();
	Dht_Init();
	Delay_ms(500);
	OLED_ShowString(0,0,"2024/10/7");
	OLED_ShowString(0,2,"LED_test");
	OLED_ShowString(0,4,"CK_message:");
	while(1)
	{	
		// Delay_ms(500);
		// CK_SendBack();

		P11 = 0;
		Delay_ms(500);
		P11 = 1;
		Delay_ms(500);

		
	}
	
}
