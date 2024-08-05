// example 55:
// The numbers are displayed through a common anode digital tube
#include <REG51.H>

void main(void)
{
	P2 = 0xFE; // P2.0 output low level, Q1 is available
	P0 = 0x92; // P0 output number 5
}