#include	"config.h"

void Delay500ms(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	i = 29;
	j = 14;
	k = 54;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
