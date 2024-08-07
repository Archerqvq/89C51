// example 67:
// simple electronic organ

#include <reg51.h>

// columns definition of matrix keyboard
sbit P14 = P1^4;
sbit P15 = P1^5;
sbit P16 = P1^6;
sbit P17 = P1^7;

unsigned char keyval;
sbit sound = P3^7; // pin that outputs pulse in particular frequency
// global variable, that stores a constant value of timer
// and it's related to each frequency
unsigned int C; 
unsigned int f;

// Macro definition of a C-key bass
#define l_dao 262 // C-key bass l_dao is set to 262Hz
#define l_re 286
#define l_mi 311
#define l_fa 349
#define l_sao 392
#define l_la 440
#define l_xi 494

// Macro definition of a C-key midrange
#define dao 525 // C-key midrange dao is set to 525Hz
#define re 587  
#define mi 659
#define fa 698
#define sao 784
#define la 880
#define xi 987

// Macro definition of a C-key high
#define h_dao 1046 // C-key high dao is set to 1046Hz
#define h_re 1174  
#define h_mi 1318
#define h_fa 1396
#define h_sao 1567
#define h_la 1760
#define h_xi 1975

// soft debouncing
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

// delay 200ms
void delay(void)
{
    unsigned char i, j;

    for(i = 0; i < 250; i++)
    {
        for(j = 0; j < 250; j++)
        {
        }
    }
}

// output audio
void output_sound(void)
{
    C = (46083 / f) * 10; 
    // C = (46083 / f);  
    TH0 = (8192 - C) / 32; // 13-bit timer 0 high 8 bits assignment
    TL0 = (8192 - C) % 32; // 13-bit timer 0 low 5 bits assignment
    TR0 = 1; // enable timer 0
    delay(); // delay 200ms to play sound
    TR0 = 0; // disable timer 0
    sound = 1; // disable buzzer
    keyval = 0xFF; // stop playing after buzzer output finished
}


void main(void)
{
    EA = 1; // global interrupt enable
    ET0 = 1; // timer 0 interrupt allowed
    ET1 = 1; // timer 1 interrupt allowed
    TR1 = 1;  // timer 1 enable to scan keyboard
    TMOD = 0x10; // timer 1 in mode 1, timer 0 in mode 0
    TH1 = (65536 - 500) / 256; // high 8 bits assignment of timer 1
    TL1 = (65536 - 500) % 256; // low 8 bits assignment of timer 1
    
    while(1)
    {
        switch(keyval)
        {
            case 1: f = dao; output_sound(); break;
            case 2: f = l_xi; output_sound(); break;
            case 3: f = l_la; output_sound(); break;
            case 4: f = l_sao; output_sound(); break;

            case 5: f = sao; output_sound(); break;
            case 6: f = fa; output_sound(); break;
            case 7: f = mi; output_sound(); break;
            case 8: f = re; output_sound(); break;

            case 9: f = h_re; output_sound(); break;
            case 10: f = h_dao; output_sound(); break;
            case 11: f = xi; output_sound(); break;
            case 12: f = la; output_sound(); break;

            case 13: f = h_la; output_sound(); break;
            case 14: f = h_sao; output_sound(); break;
            case 15: f = h_fa; output_sound(); break;
            case 16: f = h_mi; output_sound(); break;
        }
    }
}

// timer 0 interrupt service, let P3.7 output audio pulse
void timer0_serve(void) interrupt 1 using 1
{
    TH0 = (8192 - C) / 32;
    TL0 = (8192 - C) % 32;

    sound = !sound; // toggle P3.7 to output square wave
}

// timer 1 interrupt service, scaning keyboard to assert key value
void timer1_serve(void) interrupt 3 using 2
{
    TR1 = 0; // disable timer 0
    P1 = 0xF0; // let all rows to be 0, and all columns to be 1

    if((P1&0xF0) != 0xF0) // key press detected
    {
        delay20ms();    
    }

    if((P1&0xF0) != 0xF0) // key press confirmation
    {
        P1 = 0xFE; // let the first row set to 0  
        if(P14 == 0)  
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

        P1 = 0xFD; // let the second row set to 0  
        if(P14 == 0)  
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

        P1 = 0xFB; // let the third row set to 0  
        if(P14 == 0)  
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

        P1 = 0xF7; // let the fourth row set to 0  
        if(P14 == 0)  
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

    TR1 = 1; // enable timer 0
    TH1 = (65536 - 500) / 256; // high 8 bits assignment of timer 1
    TL1 = (65536 - 500) % 256; // low 8 bits assignment of timer 1
}