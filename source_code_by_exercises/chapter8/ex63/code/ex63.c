// example 63:
// Expansion of the key functions: one key for four functions
#include <reg51.h>

unsigned char ID;
sbit S1 = P1^4;

// it requires about 200ms to delay for just one key scaning
void delay(void)
{
    unsigned char i, j;
    for(i = 0; i < 200; i++)
    {
        for(j = 0; j < 100; j++)
        {
        }
    }
}

void main(void)
{
    TMOD = 0x02; // timer 0 in mode 2
    EA = 1;      // global interrupt enable
    ET0 = 1;     // t0 interrupt enable
    TR0 = 1;     // enable t0
    TH0 = 256 - 200; // initialized value, 200us for 1 interrupt request
    TL0 = 256 - 200; // auto-reload value

    ID = 0;
    while(1)
    {
        switch(ID)
        {
            case 0: P3 = 0xFE; break;
            case 1: P3 = 0xFD; break;
            case 2: P3 = 0xFB; break;
            case 3: P3 = 0xF7; break;        
        }
    }
}

// interrupt service for timer 0 to scaning keyboard
void intersev(void) interrupt 1 using 1
{
    TR0 = 0; // disable timer 0
    P1 = 0xFF; // reset key status
    
    if(S1 == 0)
    {
        delay();
        if(S1 == 0)
        {
            ID += 1;
        }
    }  

    if(ID == 4)
    {
        ID = 0;
    }

    TR0 = 1; // enable timer 0
}