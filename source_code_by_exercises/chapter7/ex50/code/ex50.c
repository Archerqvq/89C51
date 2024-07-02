// example 50
// Expand the parallel output to control the flow lamp in mode 0
															
#include <reg51.h>
#include <intrins.h> // Include head file that has definition of function _nop_()

// Flow lamp control code
unsigned char code tab[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
sbit P17 = P1 ^ 7;

// Delay about 150ms
void delay(void)
{
	unsigned char m, n;

	for(m = 0; m < 200; m++)
	{
		for(n = 0; n < 250; n++)
		{
			;
		}
	}
}

// Send one byte data
void send(unsigned char dat)
{
	P17 = 0; // Pin P1.7 output 0 to clear 74LS164
	_nop_(); // Dealy for one machine cycle
	_nop_(); // One more machine cycle to make sure clear is done
	P17 = 1; // The end of clearing 74LS164
	SBUF = dat; // Write data into buffer to activate sending

	while(TI == 0); // Wait until sending finished
	TI = 0; // After sending successfully, reset TI to 0
}

void main(void)
{
	unsigned char i;

	SCON = 0x00; // Let serial port works in method 0

	while(1)
	{
		for(i = 0 ; i < 8; i++)
		{
			send(tab[i]); // Send data
			delay();
		}
	}
}