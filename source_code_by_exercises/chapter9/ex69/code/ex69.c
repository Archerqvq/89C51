// example 69: first write button counting in to AT24C02, 
// then read in into 1602 LCD

#include <reg51.h>
#include <intrins.h> // where _nop_() included(_nop_ means "no operation" for one machine cycle)

// NOTE: the high four-bit of OP_READ/OP_WRITE represent the address of i2c device 
// device address and write operation(0xA1, aka 1010 0001B, and the last bit - 1 means write)
#define OP_READ (0xA1) 
// device address and read operation(0xA0, aka 1010 0000B, and the last bit - 0 means read)
#define OP_WRITE (0xA0)

// 1602 LCD pins definition
sbit RS = P2^0; // register selection bit 
sbit RW = P2^1; // write/read selection bit
sbit E = P2^2;  // enable signal bit
sbit BF = P0^7; // busy signal bit
sbit S = P1^4; // S
sbit SDA = P3^4; // serial data line defined as p3.4
sbit SCL = P3^3; // serial clock line defined as p3.3

unsigned char code digit[] = {"0123456789"}; // array to display number

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

// operations to LCD
// feature: assert whether LCD is in busy
// return value: 1 - busy, 0 - idle
unsigned char BusyTest(void)
{
    bit result;
    // read status is allowed when RS is low and RW is high
    RS = 0;
    RW = 1;
    E = 1; // E = 1, write/read allowed
    _nop_(); // do nothing, just to consume time
    _nop_();
    _nop_();
    _nop_(); // idle operation for serial four machine cycles to give hardware some time to respond
    result = BF; // get busy status by reading BF
    E = 0; // let E be 0 to make LCD available
    return result;
}

// feature: mode settings or display addresses that need to be sent to LCD
void WriteInstruction(unsigned char dictate)
{
    while(BusyTest() == 1); // await if LCD is in used
    // writing instructions is allowed when RS and RW are both 0
    RS = 0;
    RW = 0;
    // NOTE: here we let E to be 0 first(according to chart 8-6 in the book, E shoud be high when writing is needed).
    // just let E have a change from 0 to 1, so E should be set to 0 first
    E = 0;

    _nop_();    
    _nop_();    // Two no operation instructions to give hardware some time to respond
    P0 = dictate; // Write data into P0, which could be instruction or address
    _nop_();
    _nop_();
    _nop_();
    _nop_(); // Four machine cycles are consumed here 
    E = 1;   // Let E be 1 to enable writing capability.
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    E = 0;   // LCD start executing instructions when E has a falling edge
}

// Specify the actual address for the character to be displayed on LCD
void WriteAddress(unsigned char x)
{
    WriteInstruction(x | 0x80); // Using "80H+x(x means address code)" to refers the actual address 
}

// Writing data(characters in ASCII table) into LCD 
void WriteData(unsigned char y)
{
    while(BusyTest() == 1); // await if LCD is in used
    // writing data is allowed when RS and RW are both 0
    RS = 1;
    RW = 0;
    // NOTE: here we let E to be 0 first(according to chart 8-6 in the book, E shoud be high when writing is needed).
    // just let E have a change from 0 to 1, so E should be set to 0 first
    E = 0;

    P0 = y;
    _nop_();
    _nop_();
    _nop_();
    _nop_(); // Four clock cycles are consumed here 
    E = 1;
    _nop_();
    _nop_();
    _nop_();
    _nop_(); // Four clock cycles are consumed here
    E = 0; // LCD start executing instructions when E has a falling edge       
}

// initialization for lcd's display mode
void LcdInitiate(void)
{
    delaynms(15); // 15ms for the first time to write insturction
    // display mode setting: 16x2 display, 5x7 dot matrix, 8-bit data interface
    WriteInstruction(0x38);
    delaynms(5);
    // mode setting
    WriteInstruction(0x38);
    delaynms(5);
    WriteInstruction(0x38);
    delaynms(5);
    // continuously write three times to make sure it works

    // display mode setting: display on, cursor off, cursor not blink
    WriteInstruction(0x0C);
    delaynms(5);
    // display mode setting: cursor right shift, character not move
    WriteInstruction(0x06);
    delaynms(5);
    // clear screen instruction to clear previous content
    WriteInstruction(0x01);
    delaynms(5);
}

// Display number on the screen
void Display(unsigned char x)
{
    unsigned char i, j;
    i = x / 10; // Get it's tens
    j = x % 10; // Get it's ones
    WriteAddress(0x44); // The address indicates where the number needs to be displayed on the screen
    WriteData(digit[i]);
    WriteData(digit[j]);
}

// The following operations involve writing to and reading from AT24C02(I2C interface)

// I2C start signal 
void start(void)
{
    SCL = 1; // Initialize SDA to 1
    SDA = 1; // SCL must be 1 when the data transmission starts
    _nop_();            
    _nop_(); // Wait for two machine cycles
    SDA = 0; // A falling edge of SDA is considered as start signal
    _nop_();            
    _nop_();
    _nop_();            
    _nop_(); // Wait for four machine cycles
    SCL = 0; // Data transmitted through SDA is changeable when SCL is low 
}

// I2C stop singnal
void stop()
{
    SDA = 0; // Initialize SDA to 1
    _nop_();
    _nop_(); // Wait for two machine cycles
    SCL = 1;
    _nop_();
    _nop_();
    _nop_();
    _nop_(); // Wait for four machine cycles
    SDA = 1; // A rising edge of SDA when SCL is high is considered as stop signal
}

// Read data from AT24Cxx
unsigned char ReadData(void)
{
    unsigned char i, x;

    // Read data through SDA
    for(i = 0; i < 8; i++)
    {
        // When SCL is high, the data in SDA cannot be changed, so it can be read.
        SCL = 1;
        x <<= 1;
        x |= (unsigned char)SDA;
        SCL = 0;              
    }
    return x;    
}

// Write data to the current address of the AT24Cxx
bit WriteCurrent(unsigned char y)
{
    unsigned char i;
    bit ack_bit;    // Store ack bit
    
    // Write a byte
    // When transmitting, the most significant bit is sent first, followed by the least significant bit
    for(i = 0; i < 8; i++) 
    {
        // Through a bitwise 'AND' operation, send the most significant bit of data to SDA
        SDA = (bit)(0x80&y);                
        _nop_(); // Wait for a machine cycle
        SCL = 1; // Write data to AT24Cxx on the rising edge of SCL
        _nop_();
        _nop_();
        SCL = 0; // Set SCL back to low level to generate the required eight pulses on the SCL line for data transmission.
        y <<= 1; // Shift each bit in y one position to the left
    }    

    // Wait for ack signal after sending one byte data
    // The host releases SDA and waits for the slave's response, which will pull SDA low
    // Check the book for more details
    SDA = 1;
    _nop_(); // Wait for one machine cycle
    _nop_();
    SCL = 1;
    _nop_();
    _nop_();
    _nop_();
    _nop_(); // Wait for one machine cycle

    ack_bit = SDA; // The receiving device (AT24Cxx) sends a low level to SDA, indicating that a byte has been received

    SCL = 0;    // Data on SDA is only allowed to change when SCL is at a low level, allowing subsequent data transmission.

    return ack_bit;
}

// Write data to the specified address in the AT24Cxx
void WriteSet(unsigned char add, unsigned char dat)
{
    start();  // Initiate data transmission
    
//    WriteCurrent(OP_WRITE); // Select the AT24Cxx chip to be operated on and indicate that data will be written to it
//    WriteCurrent(add); // Write to the specified address
//    WriteCurrent(dat); // Write data to the current address (specified above)

    if(WriteCurrent(OP_WRITE) == 0)
    {
        if(WriteCurrent(add) == 0)
        {
            if(WriteCurrent(dat) == 0)            
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

    stop();            // Stop data tramsmission
    delaynms(4);  // The write cycle for one byte is 1 ms; it is recommended to have a delay of at least 1 ms
}

// Read data from the current address in the AT24Cxx
unsigned char ReadCurrent()
{
    unsigned char x;
    start();  // // Initiate data transmission

    if(WriteCurrent(OP_READ) == 0)
    {
        // Write data successfully      
    }
    else
    {
        return 0;
    }

    x=ReadData();
    stop();

    return x;
}

// Read data from the specified address in the AT24Cxx
unsigned char ReadSet(unsigned char set_add)
{
    // Refer to the I2C read operation timing sequence for more information
    start();
    WriteCurrent(OP_WRITE);
    WriteCurrent(set_add);
    return (ReadCurrent());
}

void main(void)
{
    // The variable 'sum' is used to store the count value when the button is pressed
    unsigned char sum = 0; 
    // The value that read from the AT24C02 which should be the same as sum
    unsigned char x = 0; 
 
    LcdInitiate();

    while(1)
    {
        if(S == 0) // If button pressed
        {
            delaynms(80); // decouncing by software, the delay lasts 80 milliseconds
            if(S == 0)
            {
                // button perss confirmed
            } 
            
            while(S != 1)
            {
                // Block the program when the button is pressed
            }

            if(sum == 10)
            {
               sum = 0;
            }
            else
            {
                sum++;
            }                                   
        }    
        
        WriteSet(0x01, sum); 
        x = ReadSet(0x01);
        Display(x);                    
    }
}