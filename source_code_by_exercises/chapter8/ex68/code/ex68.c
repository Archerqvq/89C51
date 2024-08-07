// example 68: lcd clock design
                     
#include <reg51.h>
#include <intrins.h>

sbit RS = P2^0; // register selection bit
sbit RW = P2^1; // read/write selection bit
sbit E = P2^2;  // enable signal bit
sbit BF = P0^7; // busy signal bit

// character array to display numbers
unsigned char code digit[] = {"0123456789"};
// character array to display notification 
unsigned char code string[] = {"time in beijing"}; 
// variable to count interrupt times
unsigned char count;
// variables to storage second, minute and hour
unsigned char s, m, h;

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

// display hours
void DisplayHour(void)
{
    unsigned char i, j;
    i = h / 10;
    j = h % 10;
    WriteAddress(0x44);
    WriteData(digit[i]);        
    WriteData(digit[j]);
}

// display minutes
void DisplayMinute(void)
{
    unsigned char i, j;
    i = m / 10;
    j = m % 10;
    WriteAddress(0x47);
    WriteData(digit[i]);        
    WriteData(digit[j]);
}

// display second
void DisplaySecond(void)
{
    unsigned char i, j;
    i = s / 10;
    j = s % 10;
    WriteAddress(0x4A);
    WriteData(digit[i]);        
    WriteData(digit[j]);    
}

void main(void)
{
    unsigned char i;
    LcdInitiate(); 

    TMOD = 0x01; // t0 in mode 1
    // 50ms for every interrupt
	TH0=(65536 - 46083) / 256;    
	TL0=(65536 - 46083) % 256;   
    EA = 1;  // global interrupt
    ET0 = 1; // t0 interupt allowed
    TR0 = 1; // enable t0

    count = 0;
    // first of all, display notification 
    s = 0;
    m = 0;
    h = 0;

    WriteAddress(0x00); // Write address, where is row 1 column 4
    i = 0;
    while(string[i] != '\0')
    {
        WriteData(string[i]);
        i++;      
    }
 	WriteAddress(0x46);     // Write address, where is row 2 column 7
	WriteData(':');         // write ':' on lcd
	WriteAddress(0x49);     // Write address, where is row 2 column 10
	WriteData(':');         // write ':' on lcd

    while(1)
    {
        DisplayHour();
        delay(5); // give some time for the reponse of hardware
        DisplayMinute();
        delay(5); // give some time for the reponse of hardware
        DisplaySecond();
        delay(5); // give some time for the reponse of hardware               
    }

}

// enter timer 0 interrupt every 50ms
void Time0(void ) interrupt 1 using 0 
{
    count++;
    
    if(count == 20)
    {
        count = 0;
        s++; 
    }
    
    if(s == 60) // if reaches 60s
    {
        s = 0;
        m++;        
    }  
    
    if(m == 60) // if reachses 60m
    {
        m = 0;
        h++;        
    } 
    
    if(h == 24) // if reaches 24h
    {        
        h = 0;        
    }

    // reload manually
    TH0=(65536 - 46083) / 256;    
	TL0=(65536 - 46083) % 256; 
}