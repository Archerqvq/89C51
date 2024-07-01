// ex49:
// Use the timer t0 to control the output of rectangular waves with different heights and widths
#include <reg51.h>

sbit u = P3 ^ 0;
unsigned char cnt;

void delay30ms(void)
{
	unsigned char m, n;
	for(m = 0; m < 100; m++)
	{
		for(n = 0; n < 100; n++)
		{
		}
	}	
}

void main(void)
{
	unsigned char i;

	EA = 1; // Enable global interrupt
	EX0 = 1; // Enable external interrupt
	IT0 = 1; // Configure INT0 for falling edge trigger 
	
	for(i = 0; i < 100; i++) // Output 
	{
		u = 1;
		delay30ms();
		u = 0;
		delay30ms();									
	} 

	while(1);
}

void int0(void) interrupt 0 using 0
{
	cnt++;
	P1 = cnt;
}



