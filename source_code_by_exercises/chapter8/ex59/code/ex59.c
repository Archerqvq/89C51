// example 59:
// Stand-alone keyboard input experiments without software debounce
#include <reg51.h>

sbit S1 = P1^4; // Define s1 as P1.4
sbit LED0 = P3^0; // Define LED0 as P3.0

void main(void)
{
	LED0 = 0; // P3.0 output low level as default

	while(1)
	{
		// If P1.4 output low level then button s1 has been pressed
		if(S1 == 0) 
		{
            LED0 = !LED0; // Toggle P3.0 the status of P3.0
		}
	}
}