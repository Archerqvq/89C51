// example 54:
// Receive date from pc to mcu
// NOTE: this example should be run on the real dev borad

#include <REG51.H>

unsigned char receive(void)
{
	unsigned char dat;
	while(RI == 0);
	RI = 0;

	dat = SBUF;

	return dat;
}

void main(void)
{
   unsigned char i;
   TMOD = 0x20; // Timer 1 in mode 2(8-bit auto-reload)
   TH1 = 0xFD;	// Set reload value for timer 1
   TL1 = 0xFD; 	// Reload value for timer 1

   SCON = 0x50; // Reception is allowed
   PCON = 0x00; // Baud rate 9600 bit/s

   TR1 = 1; 	// Timer 1 activated
   REN = 1; 	// Reception activated

   while(1)
   {
		P1 = receive();
   }

}