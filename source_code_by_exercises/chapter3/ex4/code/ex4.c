// example 4:
// use a mcu to control an LED to flash, and recognize the working frequency
// of the mcu
#include <reg51.h>

/*
	feature: delay
	note: no return value and param needed
*/
void delay(void)
{
	unsigned int i;

	for(i = 0; i < 20000; i++)
	{
		; // do nothing
	}
}

/*
	feature: main function
*/
void main(void)
{
	while(1)  // infinite loop
	{
		P1 = 0xFE; // P1 = 1111 1110B, P1.0 output low level
		delay();   // delay for a while
		P1 = 0xFF; // P1 = 1111 1111B, P1.0 output high level
		delay();
	}
}