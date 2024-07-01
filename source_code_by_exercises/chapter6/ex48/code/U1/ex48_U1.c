// ex48: A square wave with a positive pulse width of 250us is output
#include <reg51.h>

sbit u = P1 ^ 4;

void main(void)
{
	TMOD = 0x02; // Timer0 in method2
	EA = 1; // Enable global interrupt
	ET0 = 1;// Timer0 interrupt allowed

	// Every 250us triggers timer0 interupt
	TH0 = 256 - 230;
	TL0 = 256 - 230;

	TR0 = 1;

	while(1);
}

void timer0(void) interrupt 1 using 0
{
	u = ~u;
}