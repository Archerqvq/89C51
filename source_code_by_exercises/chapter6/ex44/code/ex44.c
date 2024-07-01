// example 44:
#include <reg51.h>

sbit D1 = P2^0; // set bit D1 to P2.0 pin

void main(void)
{
	EA = 1; // turn on global interrupt
	ET0 = 1; // timer0 interrupt allowed

	TMOD = 0x01; // timer0 works in method1	
	TH0 = (65536 - 46083) / 256; // assign high 8 bits for timer0
	TL0 = (65536 - 46083) % 256; // assign low 8 bits for timer0

	TR0 = 1; // enable timer0
	while(1); // infinite loop, waiting for interrupt
}

// interrupt service routine for timer0
// 1 for interrupt number of timer0
// 0 for number 0 working registers
void timer0(void) interrupt 1 using 0 
{
	D1 = ~D1; // revert for evevry bit
	TH0 = (65536 - 46083) / 256; // reassign high 8 bits for timer0
	TL0 = (65536 - 46083) % 256; // reassign low 8 bits for timer0	 
}