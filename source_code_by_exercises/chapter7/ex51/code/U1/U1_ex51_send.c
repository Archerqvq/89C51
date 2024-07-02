// example 51
// Send data

#include <reg51.h>

// Flow lamp control code
unsigned char code tab[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

// Send one byte data
void send(unsigned char dat)
{
	SBUF = dat; // Wait until write data into send buffer
	while(TI == 0); // Wait until IT flag not equivalent to 1
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
	
	TMOD = 0x20; // Timer 1 in method 2
	TH1 = 0xFD;	 // Initialize high 8-bits of timer 1
	TL1 = 0xFD;  // Initialize low 8-bits of timer 1

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