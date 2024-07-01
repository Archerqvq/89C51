// example 45
#include <reg51.h>

sbit D1 = P2 ^ 0; // 
sbit D2 = P2 ^ 1;

// Global variables to store the number of times that T1 and T2 interrupt have occured.
unsigned char cnt1;	
unsigned char cnt2; 

void main(void)
{
	EA = 1; // Enable global interrupt

	ET1 = 1;// Enable T1 interrupt
	TMOD = 0x10;// T1 in method1
	TH1 = (65536 - 46083) / 256; // Assigned high 8 bits for T1
	TL1 = (65536 - 46083) % 256; // Assigned low 8 bits for T1
	TR1 = 1; // Enable T1
	cnt1 = 0; // Counting the number of times of T1 from 0
	cnt2 = 0; // Counting the number of times of T2 from 0

	while(1); // Infinite loop
}

// interrupt 3 means the interrupt number of T1
void timer1(void) interrupt 3 using 0
{
	cnt1++;
	cnt2++;

	// When cnt1 comes to 2, it means hitting 100ms
	if(cnt1 == 2)
	{
		D1 = ~D1; // Revese D1
		cnt1 = 0; // Reset cnt1 to 0
	}

	// When cnt2 comes to 8, it means hitting 400ms
	if(cnt2 == 8)
	{
		D2 = ~D2; // Revese D2
		cnt2 = 0; // Reset cnt2 to 0
	}

	TH1 = (65536 - 46083) / 256; // Reassigned high 8 bits for T1
	TL1 = (65536 - 46083) % 256; // Reassigned low 8 bits for T1
}