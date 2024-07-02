// example 51:
// Receive data

#include <reg51.h>

unsigned char receive(void)
{
	unsigned char dat;

	// Wait until receive is done, then RI set to 1
	while(RI == 0)
	{
		;
	}

	RI = 0; // Set RI to 0 for receiving next frame
	dat = SBUF; // Assign data from SBUF to variable dat

	return dat; // Return data from receiving
}

void main(void)
{
	TMOD = 0x20; // Timer 1 in method 2
	TH1 = 0xFD;	 // Initialize high 8-bits of timer 1
	TL1 = 0xFD;  // Initialize low 8-bits of timer 1

	SCON = 0x50; // Serial port in method 1, reception is allowed
	PCON = 0x00; // Baud rate it 9600bit/s
	REN = 1;     // Allowed to receive data

	TR1 = 1;     // Timer 1 activated

	while(1)
	{
		P1 = receive(); // Show received data by Port 1
	}

}