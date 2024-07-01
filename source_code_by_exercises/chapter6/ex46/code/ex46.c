// example 46
#include <reg51.h>

sbit sound = P3 ^ 7;

void main(void)
{
	EA = 1;	// Enable global interrupt

	ET1 = 1; // T1 interrupt allowed
	TMOD = 0x10; // T1 in method1
	// Frequency for 1kHZ
	// Cycle for 1ms
	TH1 = (65536 - 461) / 256;
	TL1 = (65536 - 461) % 256;	
	TR1 = 1; // Enable T1

	while(1); // Infinite loop
}

void timer1(void) interrupt 3 using 0
{
	sound = ~sound;
	TH1 = (65536 - 461) / 256;
	TL1 = (65536 - 461) % 256;		
}