// example 67:
// using LCD to diaplay character "A"
#include <reg51.h>
#include <intrins.h> // including the declaration of _nop_()

sbit RS = P2^0; // register selection bit
sbit RW = P2^1; // read/write selection bit
sbit E = P2^2;  // enable signal bit
sbit BF = P0^7; // busy signal bit

// (3j+2)*i = (3x33+2)x10=1010(us) // no idea how this formula comes -.-
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

// delay for x millisecond
void delay(unsigned char n)
{
    unsigned char i;
    for(i = 0; i < n; i++)
    {
        delay1ms();    
    }
}

// read whether ldc module is occupied
// return value : busy = 1 , available = 0
unsigned char BusyTest(void)
{
    bit result;
    // set RS to 0 and RW to 1 to read lcd's status
    RS = 0;  
    RW = 1;
    // enable read/write
    E = 1; 

    _nop_(); // do nothing for a machine cycle
    _nop_();
    _nop_();
    _nop_(); // four serial machine cycles for response of lcd

    result = BF; // read busy signal and set to result

    // ATTENTION: this should not be commnented
    // an assumption, that a falling edge of E make operation availible
    E = 0;   

    return result;
}

// write mode setting or display address to ldc
void WriteInstructioin(unsigned char dictate)
{
    while(BusyTest() == 1); // await if ldc is occupied
    // RS and RW are 0, write instruction operation is available
    RS = 0;
    RW = 0;
    // there is a rising edge when writing instruction
    // so first let E to be 0
    E = 0;
    _nop_();
    _nop_();
    P0 = dictate; // dictate could be address or insturction
    _nop_();
    _nop_();
    _nop_();
    _nop_();    // response for lcd
    // a rising edge for E, which means write operation is available
    E = 1;
    _nop_();
    _nop_();
    _nop_();
    _nop_();    // response for lcd
    // a falling edge for E, which makes lcd module excutes instructions automatically
    E = 0;
}

// specify the real address of character display
void WriteAddress(unsigned char x)
{
    WriteInstructioin(x | 0x80);
    // the way to display right position on lcd is "80H+x"(x is address code)
}

// write data(standard ASCII) to lcd
void WriteData(unsigned char y)
{
    while(BusyTest() == 1);
    RS = 1; // RS is 1 and RW is 0, which can write data
    RW = 0;
    // there is a rising edge when writing instruction
    // so first let E to be 0
    E = 0;  
    P0 = y; // send data through P0
    _nop_();
    _nop_();
    _nop_();
    _nop_(); // response for lcd
    // a rising edge for E, which means write operation is available
    E = 1;
    _nop_();
    _nop_();
    _nop_();
    _nop_();    // response for lcd
    // a falling edge for E, which makes lcd module excutes instructions automatically
    E = 0; 
}

// initialization for lcd's display mode
void LcdInitiate(void)
{
    delay(15); // 15ms for the first time to write insturction
    // display mode setting: 16x2 display, 5x7 dot matrix, 8-bit data interface
    WriteInstructioin(0x38);
    delay(5);
    // mode setting
    WriteInstructioin(0x38);
    delay(5);
    WriteInstructioin(0x38);
    delay(5);
    // continuously write three times to make sure it works

    // display mode setting: display on, cursor on, cursor blink
    // WriteInstructioin(0x0F);
    // display mode setting: display on, cursor off, cursor not blink
    WriteInstructioin(0x0C);
    delay(5);
    // display mode setting: cursor right shift, character not move
    WriteInstructioin(0x06);
    delay(5);
    // clear screen instruction to clear previous content
    WriteInstructioin(0x01);
    delay(5);
}

void main(void)
{
    LcdInitiate();
    while(1)
    {
//        WriteAddress(0x07); // specify row 1, column 8 on lcd to display
//        WriteData('A');     // Write 'A' to lcd, automatically finish by lcd 
//        WriteAddress(0x08);
//        WriteData('B');
//        WriteAddress(0x09);       
//        WriteData('C');

        WriteAddress(0x02); // specify row 1, column 8 on lcd to display
        WriteData('H');     // Write 'A' to lcd, automatically finish by lcd 
        WriteAddress(0x03);
        WriteData('E');
        WriteAddress(0x04);       
        WriteData('L'); 
        WriteAddress(0x05);       
        WriteData('L');
        WriteAddress(0x06);       
        WriteData('O');

        WriteAddress(0x07);       
        WriteData(' ');

        WriteAddress(0x08);       
        WriteData('W');
        WriteAddress(0x09);       
        WriteData('O');
        WriteAddress(0x0A);       
        WriteData('R');
        WriteAddress(0x0B);       
        WriteData('L');
        WriteAddress(0x0C);       
        WriteData('D');                                           
        WriteAddress(0x0D);
        WriteData('!');
    }

}