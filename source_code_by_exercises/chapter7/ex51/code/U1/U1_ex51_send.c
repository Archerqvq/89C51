// example 51
// Send data

#include <reg51.h>

// Flow lamp control code
unsigned char code tab[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

// Send one byte data
void send(unsigned char dat)
{
	SBUF = dat; // Wait until write data into send buffer
	while(TI == 0); // Wait until IT flag not equivalent to 1, which means data has been sent by uart
	TI = 0;	 // Reset TI by software
}

// Delay for 150ms around
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

void main(void)
{
	unsigned char i;
	// Timer 1 set for baud rate generator
	TMOD = 0x20; // Timer 1 mode 2 (8-bit auto-reload)
	TH1 = 0xFD;	 // Set reload value for Timer 1
	TL1 = 0xFD;  // Reload value for Timer 1

	SCON = 0x40; // Serial port in method 1
	PCON = 0x00; // Baud rate is 9600bit/s

	TR1 = 1;     // Enable timer 1

	while(1)
	{
		for(i = 0; i < 8; i++)
		{
			send(tab[i]);
			delay(); // Send data every 150ms
		}
	}
	
}