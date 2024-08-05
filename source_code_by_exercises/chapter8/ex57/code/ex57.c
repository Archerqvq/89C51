// example 57:
// Digital tube slow dynamic scan shows the number "1234"
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
		P2 = 0xFE; // P2.0 output low level, DS0 is enabled
		P0 = 0xF9; // number 1
		delay();

		P2 = 0xFD; // P2.1 output low lelve, DS1 is enabled
		P0 = 0xA4; // number 2
		delay();

		P2 = 0xFB; // P2.2 output low lelve, DS2 is enabled
		P0 = 0xB0; // number 3
		delay();

		P2 = 0xF7; // P2.3 output low lelve, DS3 is enabled
		P0 = 0x99; // number 4
		delay();

		P2 = 0xFF; // Disable all ds
	}  	
}