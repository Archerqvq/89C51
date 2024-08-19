// example 70:
// Read and write operations for multiple AT24C02 devices connected to the I2C bus
#include <reg51.h>
#include <intrins.h> // Head file containing the _nop_() function definition

#define OP_READ1 (0xA1) // Device 1's address and read operation, where 0xA1 is equivalent to 1010 0001 in binary
#define OP_WRITE1 (0xA0) // Device 1's address and write operation, where 0xA1 is equivalent to 1010 0000 in binary
#define OP_READ2 (0xAF) // Device 2's address and read operation, where 0xAF is equivalent to 1010 1111 in binary
#define OP_WRITE2 (0xAE) // Device 2's address and write operation, where 0xAE is equivalent to 1010 1110 in binary

sbit SDA = P3^4;    // Define the Serial Data Line (SDA) bus pin as P3.4.
sbit SCL = P3^3;    // Define the Serial Clock Line (SCL) bus pin as P3.3.

// (3j+2)*i=(3x33+2)x10=1010(millisecond), so it can be considered as 1 millisecond
// no idea how this formula works 0.0
void delay1ms()
{
    unsigned char i, j;

    for(i = 0; i < 10; i++)
    {
        for(j = 0; j <33; j++)
        {
            // do nothing        
        }
    }
}

// delay x millisecond
void delaynms(unsigned char n)
{
    unsigned char i;
    
    for(i = 0; i < n; i++)
    {
        delay1ms();    
    }
}

// I2C start signal
void start(void)
{
    SCL = 1;
    SDA = 1;
    _nop_(); // Wait for one machine cycle
    _nop_(); // Another one machine cycle
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

unsigned char ReadData(void)
{
    unsigned char i, x;

    // Reading data though SDA
    for(i = 0; i < 8; i++)
    {
        SCL = 1;
        x <<= 1;
        x |= (unsigned char)SDA;        
        SCL = 0;    
    }

//    for(i = 0; i < 8; i++)
//    {
//        SCL = 1;
//        x |= ((unsigned char)SDA) << i;        
//        SCL = 0; 
//        _nop_();
//        _nop_();   
//    }
    return x;
}

bit WriteCurrent(unsigned char y)
{
    unsigned char i;
    bit ack_bit;

    for(i = 0; i < 8; i++)
    {
		SDA = (bit)(0x80&y);
		_nop_();
		SCL = 1;
		_nop_();
		_nop_();
		SCL = 0;
		y <<= 1;        
    }

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

void WriteSet1(unsigned char addr, unsigned char dat)
{
	start();
	if(WriteCurrent(OP_WRITE1))
	{
		if(WriteCurrent(addr))
		{
		   if(WriteCurrent(dat))
		   {
			   // Write data successfully
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


void WriteSet2(unsigned char addr, unsigned char dat)
{
	start();
	if(WriteCurrent(OP_WRITE2))
	{
		if(WriteCurrent(addr))
		{
		   if(WriteCurrent(dat))
		   {
			   // Write data successfully
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

unsigned char ReadCurrent1(void)
{
	unsigned char x;

	start();
	if(WriteCurrent(OP_READ1) == 0)
	{
		// Write data successfully 	
	}
	else
	{
		return 0;
	}
	x = ReadData();
	stop();

	return x;
}

unsigned char ReadCurrent2(void)
{
	unsigned char x;

	start();
	if(WriteCurrent(OP_READ2) == 0)
	{
		// Write data successfully 		
	}
	else
	{
		return 0;
	}
	x = ReadData();
	stop();
	
	return x;
}

unsigned char ReadSet1(unsigned char set_addr)
{
	start();
	if(WriteCurrent(OP_WRITE1) == 0)
	{
		if(WriteCurrent(set_addr) == 0)
		{
		  	return (ReadCurrent1());
		}
		else
		{
			return -1;
		}	 	
	}
	else
	{
		return -1;
	}	
}

unsigned char ReadSet2(unsigned char set_addr)
{
	start();
	if(WriteCurrent(OP_WRITE2) == 0)
	{
		if(WriteCurrent(set_addr) == 0)
		{
		  	return (ReadCurrent2());
		}
		else
		{
			return 0;
		}	 	
	}
	else
	{
		return 0;
	}	
}

void init_i2c(void)
{
	SCL = 1;
	SDA = 1;
}

int main(void)
{
	unsigned char x;

	init_i2c();

	WriteSet1(0x36, 0xAA);
	x = ReadSet1(0x36);

	WriteSet2(0x48, x);
	P1 = ReadSet2(0x48);
	
	return 0;
}