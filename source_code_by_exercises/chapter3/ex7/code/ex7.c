// ex7
#include <reg51.h>

sfr x = 0xB0;  // address of Port 3

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
	x = 0xFE;
	delay();
	x = 0xFD;	
	delay();
	x = 0xFB;
	delay();
	x = 0xF7;
	delay();
	x = 0xEF;
	delay();
	x = 0xDF;
	delay();
	x = 0xBF;
	delay();
	x = 0x7F;
	delay();
}