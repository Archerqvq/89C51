// example 56:
// The LED digital tube displays the number 0~9 in a loop
#include <reg51.h>

// Delay 200ms
void delay(void)
{
	unsigned char i, j;
	for(i = 0; i < 255; i++)
	{
		for(j = 0; j < 255; j++)
		{
		}
	}
}

void main(void)
{
	unsigned char i;
	// The keyword "code" could reduce the storage of the array
	unsigned char code tab[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
							 	  0x92, 0x82, 0xF8, 0x80, 0x90	
	};

	P2 = 0xFE; // P2.0 output low level to enable digital tube

	// infinite loop
	while(1)
	{
		for(i = 0; i < 10; i++)
		{
			P0 = tab[i];
			delay();	
			P0 = 0xFF;
			// delay();	
		}	
	}
}