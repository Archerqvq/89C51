// example 52: 
// send data in method 3

#include <reg51.h>

sbit p = PSW ^ 0; // Define p as bit 0 of the program status register (parity bit)
unsigned char code tab[] = {
	0xFE, 0xFD,
	0xFB, 0xF7,
	0xEF, 0xDF,
	0xBF, 0x7F
};

// Send one byte data
void send(unsigned char dat)
{
	ACC = dat;
	// Write parity to TB8
	// Note: p will be changed by counting the amount of 1 in the ACC 
	TB8 = p;	
	SBUF = dat; // Write data to sending buffer

	while(TI == 0)
	{
		;
	}

	TI = 0;
}

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

	TMOD = 0x20; // Timer 1 mode 2 (8-bit auto-reload)
	TH1 = 0xFD;	 // Set reload value for Timer 1
	TL1 = 0xFD;  // Reload value for Timer 1

	SCON = 0xC0; // Serial port in mode 3
	PCON = 0x00; // Baud rate 9600bit/s

	TR1 = 1;     // Timer 1 activated

	while(1)
	{
		for(i = 0; i < 8; i++)
		{
			send(tab[i]); // Send data every 150ms
			delay();
		}
	}
}

