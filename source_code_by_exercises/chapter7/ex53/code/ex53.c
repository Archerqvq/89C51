// example 53: 
// Sending 0xAB through uart txd from mcu to pc
// The purpose of the MAX232 is to convert the output signal of the mcu in to a signal
// that the computer can recognize(because the mcu recognize +5v as "1" and the computer 
// recognize -5v as "1" )

// ATTENTON: This example should run on real cirtuit board

#include <REG51.H>

// Arrary to be sent to control flowing lambs
unsigned char code tab[] = {
	0xFE, 0xFD, 
	0xFB, 0xF7,
	0xEF, 0xDF,
	0xBF, 0x7F
};

// Send one byte data
void send(unsigned char dat)
{
	SBUF = dat; 	// Put data to be sent into send buffer
	while(TI == 0); // Wait until sending is complete
	TI = 0;			// Reset TI by software
}

// Delay around 150ms
void delay(void)
{
	unsigned char m, n;
	for(m = 0; m < 200; m++)
	{
		for(n = 0; n < 250; n++)
		{
		
		}
	}
}


void main(void)
{
   unsigned char i;
   TMOD = 0x20; // Timer 1 in mode 2(8-bit auto-reload)
   TH1 = 0xFD;	// Set reload value for timer 1
   TL1 = 0xFD; 	// Reload value for timer 1

   SCON = 0xC0; // Serial port in mode 3
   PCON = 0x00; // Baud rate 9600 bit/s

   TR1 = 1; 	// Timer 1 activated

   while(1)
   {
		for(i = 0; i < 8; ++)
		{
			send(tab[i]);
			delay();
		}   	
   }

}

