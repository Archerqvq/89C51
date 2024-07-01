// example 43
#include <reg51.h>

sbit sound = P3^7; 

void main(void)
{
	TMOD = 0x10; // set timer1 in mothod1

// frequency for 500HZ
// cycle for 2ms
//	TH1 = (65536 - 921) / 256;
//	TL1 = (65536 - 921)	% 256;

// frequency for 1kHZ
// cycle for 1ms
	TH1 = (65536 - 461) / 256;
	TL1 = (65536 - 461) % 256;	
	TR1 = 1; // enable timer1

	while(1)
	{
		while(TF1 == 0); // do nothing

		TF1 = 0; // reset timer1 interrupt
		sound = ~sound;

		TH1 = (65536 - 461) / 256;
		TL1 = (65536 - 461) % 256;

// frequency for 1kHZ
//		TH1 = (65536 - 921) / 256;
//		TL1 = (65536 - 921)	% 256;
	}
}