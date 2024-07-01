// ex48
// Timer0 works in method2 to mesure the positive pulse width
#include <reg51.h>
sbit ui = P3 ^ 2; // INT0 pin

void main(void)
{
	TMOD = 0x0A; // Configure Timer0 to operate in Mode2 with GATE set to 1
	EA = 1; // Enable global interrupt
	ET0 = 0; // Disable Timer0 interrupt
	TR0 = 1; // Enable Timer0
	TH0 = 0; //	Initilize high 8-bits for Timer0
	TL0 = 0; //	Reload value for Timer0 

	while(1)
	{
		while(ui == 0); // Timer0 wouldn't work when INT0 is in low level

		TL0 = 0; // INT0 is in high level, Timer0 ready to count and set it to a initilized value

		while(ui == 1); // Timer0 is counting while INT0 is in hight level

		// Showing 230 in the experiment
		P1 = TL0; // Send counting consequency to P1
	}
}