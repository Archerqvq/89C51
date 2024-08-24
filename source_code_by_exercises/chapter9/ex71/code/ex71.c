// Example 71
// This example writes 'A good person lives a peaceful life' into AT24C02 for storage and then plays it back

#include <reg51.h>
#include <intrins.h> // Where function _nop_() is defined 
#define OP_READ (0xA1)  // Device's address and reading operation(0xA1 = 1010 0001B)
#define OP_WRITE (0xA0)	// Device's address and writing operation(0xA0 = 1010 0000B)

sbit SDA = P3^4;
sbit SCL = P3^3;
sbit sound = P3^7; // Define P3.7 as the pin that ouputs audio
unsigned int C;    // Constant for timer

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
void delay()
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

void delay1ms()
{
	unsigned char i, j;

	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 33; j++)
		{
			;
		}
	}
}

void delaynms(unsigned char n)
{
	unsigned char i;

	for(i = 0; i < n; i++)
	{
		delay1ms();	
	}
}

void start(void)
{
	SDA = 1;
	SCL = 1;
	_nop_();
	_nop_();
	SDA = 0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCL = 0;
}

void stop(void)
{
	SDA = 0;
	_nop_();
	_nop_();
	SCL = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SDA = 1;
}

unsigned char ReadData()
{
	unsigned char i, x;

	for(i = 0; i < 8; i++)
	{
		SCL = 1;
		x <<= 1;
		x |= (unsigned char)SDA;
		SCL = 0;	
	}

	return (x);
}

bit WriteCurrent(unsigned char y)
{
	unsigned char i;
	bit ack_bit;

	for(i = 0; i < 8; i++)
	{
		SDA = (bit)(y&0x80);
		_nop_();
		SCL = 1;
		_nop_();
		_nop_();
		SCL = 0;
		y <<= 1;
	}

	// Waiting for ack bit
	SDA = 1;
	_nop_();
	_nop_();
	SCL = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();

	ack_bit = SDA;
	SCL = 0;

	return ack_bit;
}

void WriteSet(unsigned char add, unsigned char dat)
{
	start();
	if(WriteCurrent(OP_WRITE) == 0)
	{
		if(WriteCurrent(add) == 0)
		{			
			if(WriteCurrent(dat) == 0)
			{
				// Writing successfully
			}		
			else
			{
				return;
			}
		}	
		else
		{
			return;
		}
	}
	else
	{
		return;
	}
	stop();
	delaynms(4);
}

unsigned char ReadCurrent()
{
	unsigned char x;
	
	start();
	WriteCurrent(OP_READ);	
	x = ReadData();
	stop();
	
	return x;	
}

unsigned char ReadSet(unsigned char set_addr)
{
	start();
	WriteCurrent(OP_WRITE);
	WriteCurrent(set_addr);

	return (ReadCurrent());
}


void init_i2c(void)
{
	SCL = 1;
	SDA = 1;
}

void main(void)
{
	unsigned char i, j;
	unsigned char temp; // To store compressed audio
	unsigned char Ji;   // To store note
	// Store the maximum number of notes to allocate storage space for notes and beats in the AT24C02
	unsigned char N;   
	unsigned int fr;  // To store decompressed audio

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
							 0x00}; // 0x00 is used as the end mark of the note

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

	init_i2c();

	i = 0; // Writing note frequency from f of the index 0

	while(f[i] != 0x00)
	{
		temp = (unsigned char)(f[i]/8);
		WriteSet(0x00 + i, temp);
		i++;
	}

	N = i;
	i = 0;

	while(f[i] != 0x00)
	{
		WriteSet(0x00 + N + i, JP[i]);
		i++;
	}

	while(1)
	{
		for(i = 0; i < N; i++)
		{
			temp = ReadSet(0x00 + i);
			Ji = ReadSet(0x00 + N + i);
			fr = 8 * temp;

			C = 460830 / fr;
			TH0	= (8192 - C) / 32;
			TL0 = (8192 - C) % 32;
			TR0 = 1;

			for(j = 0; j < Ji; j++)
			{
				delay();			
			}

			TR0 = 0;
		}

		sound = 1;

		for(i = 0; i < 8; i++)
		{
			delay();
		}
	}
}

void timer0(void) interrupt 1 using 1
{
	sound = !sound;
	TH0	= (8192 - C) / 32;
	TL0 = (8192 - C) % 32;
}