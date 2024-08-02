// example 52:
// receive data in mode 3
#include <REG51.h>

// ATTENTION: the 0 bit of PSW is for parity  
// unsigned char *p = (unsigned char *)0xD0;   // sfr PSW  = 0xD0;
sbit p = 0xD0;  

// Note: Receive one byte data
unsigned char recive(void)
{
	unsigned char dat;

	while(RI == 0); // Wait until one byte data recption is completed
	RI = 0; 		// Reset RI to 0 for receiving the next frame of data

	ACC = SBUF;		// Calculate parity whether is valid or not

	// Receive data when parity is matched
	// RB8 is the 9th bit when uart in mode 2 
	if(p == RB8)	 
	{
		// dat = ACC;
		dat = SBUF;
	}

	return dat;
}


void main(void)
{
	// Timer 1 set for baud generator
	TMOD = 0x20; // Timer 1 mode 2 (8-bit auto-reload)
	TH1 = 0xFD;  // Set reload value for timer 1
	TL1 = 0xFD;	 // Reload value for timer 1

	SCON = 0xC0; // Serial in mode 1, and reception is allowed
	PCON = 0x00; // Baud rate set to 9600 bit/s

	TR1 = 1; 	 // Enable timer 1
	REN = 1; 	 // Reception allowed

	while(1)
	{
		P1 = recive();
	}
}
