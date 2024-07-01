// example 47
#include <reg51.h>

sbit sound = P3 ^ 7;
unsigned int C; // To store the constant number for timer

// Macro definition for audio lowering C tone
// Define the macro l_dao as the frequency of the low tone '1', which is 262 Hz
#define l_dao 262 
// Define the macro l_re as the frequency of the low tone '2', which is 286
#define l_re 286 
// Define the macro l_mi as the frequency of the low tone '3', which is 311
#define l_mi 311 
// Define the macro l_fa as the frequency of the low tone '4', which is 349 
#define l_fa 349
// Define the macro l_sao as the frequency of the low tone '5', which is 392
#define l_sao 392
// Define the macro l_la as the frequency of the low tone '6', which is 440
#define l_la 440
// Define the macro l_xi as the frequency of the low tone '7', which is 494
#define l_xi 494

// Macro definition for audio middle C tone	  
#define dao 523
#define re 587
#define mi 659
#define fa 698
#define sao 784
#define la 880
#define xi 987

// Macro definition for audio high C tone	 
#define h_dao 1046
#define h_re 1174
#define h_mi 1318
#define h_fa 1396
#define h_sao 1567
#define h_la 1760
#define h_xi 1975

// dealy for 200ms
void delay(void)
{
	unsigned char i, j;

	for(i = 0; i < 250; i++)
	{
		for(j = 0; j < 250; j++)
		{
			;
		}						  
	}
}

void main(void)
{
	unsigned char i, j;
	// Using the 'code' keyword in this context helps in storing the
	// array 'f' in the program memory.

	// Each line corresponds to a note
	unsigned int code f[] = {re, mi, re, dao, l_la, dao, l_la, 
							 l_sao, l_mi, l_sao, l_la, dao,
							 l_la, dao, sao, la, mi, sao,
							 re,
							 mi, re, mi, sao, mi,
							 l_sao, l_mi, l_sao, l_la, dao,
							 l_la, l_la, dao, l_la, l_sao, l_re, l_mi,
							 l_sao,
							 re, re, sao, la, sao,
							 fa, mi, sao, mi,
							 la, sao, mi, re, mi, l_la, dao,
							 re,
							 mi, re, mi, sao, mi,
							 l_sao, l_mi, l_sao, l_la, dao,
							 l_la, dao, re, l_la, dao, re, mi,
							 re,
							 l_la, dao, re, l_la, dao, re, mi,
							 re,
							 0xFF}; // 0xFF is used as the end mark of the note
	// Here are the beats for each note in the sheet music
	// "4" corresponds to 4 delay units, "2" corresponds to 2 delay units, and "1" corresponds to 1 delay unit

	unsigned char code JP[] = {4, 1, 1, 4, 1, 1, 2,
							   2, 2, 2, 2, 8,
							   4, 2, 3, 1, 2, 2,
							   10,
							   4, 2, 2, 4, 4,
							   2, 2, 2, 2, 4,
							   2, 2, 2, 2, 2, 2, 2,
							   10,
							   4, 4, 4, 2, 2,
							   4, 2, 4, 4,
							   4, 2, 2, 2, 2, 2, 2,
							   10,
							   4, 2, 2, 4, 4,
							   2, 2, 2, 2, 6,
							   4, 2, 2, 4, 1, 1, 4,
							   10,
							   4, 2, 2, 4, 1, 1, 4,
							   10};
	
	EA = 1; // Enable global interrupt
	ET0 = 1; // T0 interrupt allowed
	TMOD = 0x00; // Mode 1 with timer T0 (13-bit counter) 

	while(1)
	{
		i = 0; 
		while(f[i] != 0xFF) // As long as you don't read the end sign, continue playing
		{
			C = 460830 / f[i]; // Count the number of level reversals
			// It can be proved that this is the method of assigning the initial value of the 13-bit counter TH0 8 bits higher
			TH0 = (8192 - C) / 32;
			// It can be proved that this is the method of assigning the initial value of the 13-bit counter TH0 8 bits lower
			TL0 = (8192 - C) % 32;
			TR0 = 1; // Enable timer0

			for(j = 0; j < JP[i]; j++) // Control the number of beats
			{
				delay(); // Delay 1 beat unit			
			}

			TR0 = 0; // Disable timer0

			i++; // Play the next note		
		}		
	}
}

void timer0(void) interrupt 1 using 1
{
	sound = !sound;
	TH0 = (8192 - C) / 32;
	TL0 = (8192 - C) % 32;		
}