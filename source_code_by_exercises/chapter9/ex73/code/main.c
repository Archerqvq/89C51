// Example 73
// Using SPI(Serial Peripheral Interface) to write "0xAA" to X5045 and read it back	through P1

#include <REG51.H>
#include <intrins.h> // For using _nop_()

sbit SCK = P3^4; // Define p3.4 as SCK
sbit SI = P3^5;  // Define p3.5 as SI
sbit SO = P3^6;  // Define p3.6 as SO
sbit CS = P3^7;  // Define p3.7 as CS

// Command for X5045 with SPI
#define WREN (0x06) // The write enable latch allows
#define WRDI (0x04) // The write enable latch disallows
#define WRSR (0x01) // Write status register
#define READ (0x03) // Read
#define WRITE (0x02) // Write

void delay1ms(void)
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

// Delay for x milliseconds   
void delaynms(unsigned char n)
{
	unsigned char i;

	for(i = 0; i < n; i++)
	{
		delay1ms();
	}
}

// Read data from the current address of X5045
unsigned char ReadCurrent(void)
{
	unsigned char i;
	unsigned char x = 0x00; // Store data that reads from x5045

	SCK = 1;				// Set sck to high

	for(i = 0; i < 8; i++)
	{
		SCK = 1; // Pull sck high
		SCK = 0; // Transmit data at the falling edge of SCK 
		x <<= 1; // Shift each binary digit in x left by one position, because the most significant byte data is read out first
		x |= (unsigned char)SO;
	}

	return (x); // Return one byte that reads from SO
}

// Write a byte to the address of x5045
void WriteCurrent(unsigned char dat)
{
	unsigned char i;
	SCK = 0;			// Pull SCK to low

	for(i = 0; i < 8; i++)
	{
		SI = (bit)(dat&0x80); // Assign the most significant bit of data to SI
							  // Because the bits during data transmission are from high to low
		SCK = 0;
		SCK = 1;			  // Write bits while SCK in ascending edge
		// Shift left to make sure the bit that is needed to transfer next time is the most significant bit
		dat <<= 1;			  
	}
}

// Write register status, then set watchdog overflow time and data protection
void WriteSR(unsigned char rs)
{
	CS = 0; // Pull CS low to select x5045
	WriteCurrent(WREN); // The write enable latch allows 
	CS = 1;	// Pull CS high
	CS = 0; // Pull CS low to make sure the register status command below is availible
	WriteCurrent(WRSR);	 // Write register status 
	WriteCurrent(rs); // Write a new status
	CS = 1;	// Pull CS high
}

// Write data to the designated address of x5045
void WriteSet(unsigned char dat, unsigned char addr)
{
	SCK = 0;	// Set sck to a known status
	CS = 0;		// Pull CS low to select x5045
	WriteCurrent(WREN);	// The write enable latch allows
	CS = 1;		// Pull CS high
	CS = 0;		// Pull CS low otherwise the write command below will be omitted
	WriteCurrent(WRITE);	// Write command
	WriteCurrent(addr);	// Designate the address
	WriteCurrent(dat);	// Designate the data
	CS = 1;
	SCK = 0; 		// Set sck to a known status
}

// Read data from the designated address of x5045
unsigned char ReadSet(unsigned char addr)
{
	unsigned char dat;
	SCK = 0;	// Set sck to a known status
	CS = 0;		// Pull CS low to select x5045
	WriteCurrent(READ);	// Start to read
	WriteCurrent(addr);	// Designate the address that you require to read
	dat = ReadCurrent();	// Read data
	CS = 1;	// Pull CS high
	SCK = 0;	// Set sck to a know status
	return dat; // Return the data that has been read 	
}

// Watchdog reset
void WatchDog(void)
{
	CS = 1; // Pull CS high
	CS = 0;	// Pull CS low
			// A descending edge to reset watchdog's timer
	CS = 1; // Pull Cs high
}

void main(void)
{
	// Write status register(Set watchdog overflow time to 600ms, and not write protection)
	WriteSR(0x12); 
	delaynms(10); // The write cycle of x5045 is 10ms

	while(1)
	{
		WriteSet(0xAA, 0x10); // Write data "0xAA" to the designated address of "0x10"
		delaynms(10);	// The write cycle of x5045 is 10ms
		P1 = ReadSet(0x10); // Display the data that reads from x5045 on P1
		WatchDog(); 	// Reset watchdog
	}
}