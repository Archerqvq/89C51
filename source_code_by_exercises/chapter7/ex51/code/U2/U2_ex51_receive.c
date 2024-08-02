// example 51:
// Receive data

#include <reg51.h>

unsigned char receive(void)
{
	unsigned char dat;

	// Wait until receive is done, then RI set to 1, which means that 1 byte data has been received
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
	TMOD = 0x20; // Timer 1 mode 2 (8-bit auto-reload)
	TH1 = 0xFD;	 // Set reload value for Timer 1
	TL1 = 0xFD;  // Reload value for Timer 1

	SCON = 0x50; // Serial port in method 1, reception is allowed
	PCON = 0x00; // Baud rate it 9600bit/s
	REN = 1;     // Allowed to receive data

	TR1 = 1;     // Timer 1 activated

	while(1)
	{
		P1 = receive(); // Show received data by Port 1
	}

}