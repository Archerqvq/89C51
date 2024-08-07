// example 65:
// Matrix keyboard key values of the digital tube display experiment

#include <reg51.h>

// assignment for columns of matrix keyboard
sbit P14 = P1^4;
sbit P15 = P1^5;
sbit P16 = P1^6;
sbit P17 = P1^7;

// digits displayed in digit tube from 0 to 9 
unsigned char code tab[] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99,
    0x92, 0x82, 0xF8, 0x80, 0x90,
};
unsigned char keyval;

// digital tube dynamic scaning delay
void led_delay(void)
{
    unsigned char j;

    for(j = 0; j < 200; j++)
    {
    
    }
}

// key value display
void display(unsigned char k)
{
    P2 = 0xBF; // enable digit tube DS6
    P0 = tab[k/10]; // display tens
    led_delay();

    P2 = 0x7F; // enable digit tube DS7
    P0 = tab[k%10]; // display ones
    led_delay();
}

// delay 20ms
void delay20ms(void)
{
    unsigned char i, j;

    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < 60; j++)
        {
        }
    }
}

void main(void)
{
    EA = 1; // enable global interrupt
    ET0 = 1;// t0 interrupt allowed
    TMOD = 0x01; // t0 in mode 1
    TH0 = (65536 - 500) / 256; // high eight bits assignment of t0
    TL0 = (65536 - 500) % 256; // low eight bits assignment of t0
    TR0 = 1;// enable t0
    keyval = 0x00; 

    while(1)
    {
      display(keyval);  
    }
}

// the interrupt code is 1 of timer 0
// and it uses the first register
void time0_interserve(void) interrupt 1 using 1  
{
    TR0 = 0; // turn off timer 0
    P1 = 0xF0; // all rows set to 1, and all columns set to 0

    if((P1&0xF0) != 0xF0) // button is hit when one of column equals 0
    {
        delay20ms();  
    }

    if((P1&0xF0) != 0xF0) // confirmation of button press
    {
        P1 = 0xFE; // set the first row to 0  
        // check each column's value 
        if(P14 == 0)  // if p14 is euqal to 0, that means s1 pressed 
        {
            keyval = 1;
        }
        if(P15 == 0)   
        {
            keyval = 2;
        }
        if(P16 == 0)   
        {
            keyval = 3;
        }
        if(P17 == 0)   
        {
            keyval = 4;
        }

        P1 = 0xFD; // set the second row to 0 
        if(P14 == 0)  // if p14 is euqal to 0, that means s5 pressed 
        {
            keyval = 5;
        }
        if(P15 == 0)   
        {
            keyval = 6;
        }
        if(P16 == 0)   
        {
            keyval = 7;
        }
        if(P17 == 0)   
        {
            keyval = 8;
        }

        P1 = 0xFB; // set the third row to 0 
        if(P14 == 0)  // if p14 is euqal to 0, that means s9 pressed 
        {
            keyval = 9;
        }
        if(P15 == 0)   
        {
            keyval = 10;
        }
        if(P16 == 0)   
        {
            keyval = 11;
        }
        if(P17 == 0)   
        {
            keyval = 12;
        }

        P1 = 0xF7; // set the fourth row to 0 
        if(P14 == 0)  // if p14 is euqal to 0, that means s13 pressed 
        {
            keyval = 13;
        }
        if(P15 == 0)   
        {
            keyval = 14;
        }
        if(P16 == 0)   
        {
            keyval = 15;
        }
        if(P17 == 0)   
        {
            keyval = 16;
        }

    }
    
    TR0 = 1; // enable timer 0
    // reassignment of timer 0
    TH0 = (65536 - 500) / 256;
    TL0 = (65536 - 500) % 256;
}

