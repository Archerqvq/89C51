#include <reg51.h>

void main(void)
{
	while(1)
	{
		P1 = 0xFF; // turn off all led of P1
		P0 = P1;
		P2 = P1;
		P3 = P1;
	}
}