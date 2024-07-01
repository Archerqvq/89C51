// ex6
#include <reg51.h>

void delay(void)
{
	unsigned char i, j;
	for(i = 0; i < 250; i++)
	{
		for(j = 0; j < 250; j++)
		{
			;
		}
	}
}

void main(void)
{
   while(1)
   {
   		P3 = 0xFE;
		delay();
		P3 = 0xFD;
		delay();
		P3 = 0xFB;
		delay();
		P3 = 0xF7;
		delay();
		P3 = 0xEF;
		delay();
		P3 = 0xDF;
		delay();
		P3 = 0xBF;
		delay();
		P3 = 0x7F;
		delay();
   }
}