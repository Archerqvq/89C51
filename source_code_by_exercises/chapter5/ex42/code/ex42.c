// example 42:
#include <reg51.h>

void main(void)
{	
	TMOD = 0x01; // timer0 in mode1
	TH0 = (65536 - 46083) / 256; // high 8 bits of timer0 assigned
	TL0 = (65536 - 46083) % 256; // low 8 bits of timer0 assigned
	TR0 = 1; // enable timer0

	P2 = 0xFF; // turn off all led

	while(1)
	{
		while(1)
		{
			// check timer flag to confirm whether timer0 is overflowed
			// if not overflowed then loop for doing nothing
			while(TF0 == 0);

			// TF0 is overflowed
			TF0 = 0; // when timer is up, then reset it from 1 to 0
			P2 = ~P2; // reverse all leds bit by bit

			// reassign timer value
			TH0 = (65536 - 46083) / 256;
			TL0 = (65536 - 46083) % 256;
		}	
	}
}