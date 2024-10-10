#include	"config.h"

void Delay_us(u16 j)	//@11.0592MHz
{	
	while (j--)
	{
		unsigned char data i;
		_nop_();
		_nop_();
		i = 1;
		while (--i);
	}
	
	
}
void Delay_ms(u16 k)	//@11.0592MHz
{
	while (k--)
	{
		unsigned char data i, j;

		i = 15;
		j = 90;
		do
		{
			while (--j);
		} while (--i);
	}
	
	
}

