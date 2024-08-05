// example 61:
// CPU-controlled keyboard scan experiments
#include <reg51.h>

sbit S1 = P1^4; // s1 as p1.4
sbit S2 = P1^5; // s2 as p1.5
sbit S3 = P1^6; // s3 as p1.6
sbit S4 = P1^7; // s4 as p1.7
unsigned char keyval; // to store key value

// delay for led display
void led_delay(void)
{
    unsigned char i, j;

    for(i = 0; i < 250; i++)
    {
        for(j = 0; j < 250; j++)
        {
        
        }
    }
}

// delay for key debouncing
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

// The forward flow light lights up the LED
void forward(void)
{
    P3 = 0xFE; // lights up the first light
    led_delay();
    P3 = 0xFD; // lights up the second light
    led_delay();
    P3 = 0xFB; // lights up the third light
    led_delay();
    P3 = 0xF7; // lights up the fourth light
    led_delay();
    P3 = 0xEF; // lights up the fifth light
    led_delay();
    P3 = 0xDF; // lights up the sixth light
    led_delay();
    P3 = 0xBF; // lights up the seventh light
    led_delay();
    P3 = 0x7F; // lights up the eight light
    led_delay();
    // P3 = 0xFE; // lights up the first light
    // led_delay();  
}

// The backward flow light lights up the LED
void backward(void)
{
    P3 = 0x7F; // lights up the eight light
    led_delay();
    P3 = 0xBF; // lights up the seventh light
    led_delay();
    P3 = 0xDF; // lights up the sixth light
    led_delay();
    P3 = 0xEF; // lights up the fifth light
    led_delay();
    P3 = 0xF7; // lights up the fourth light
    led_delay();
    P3 = 0xFB; // lights up the third light
    led_delay();
    P3 = 0xFD; // lights up the second light
    led_delay();
    P3 = 0xFE; // lights up the first light
    led_delay(); 
}

// turn off all leds
void stop(void)
{
    P3 = 0xFF;
}

// turn on-off leds
void flash(void)
{
    P3 = 0xFF; // turn off all leds
    led_delay(); 
    P3 = 0x00; // lights up all leds
    led_delay();
}

// keyboard scaning module
void key_scan(void)
{
    // first time to detect whether the key is pressed
    if((P1&0xF0) != 0xF0)  
    {
        delay20ms();  // debouncing

        if(S1 == 0)
        {
            keyval = 1;        
        }
        if(S2 == 0)
        {
            keyval = 2;        
        }
        if(S3 == 0)
        {
            keyval = 3;        
        }
        if(S4 == 0)
        {
            keyval = 4;        
        }
    }
}

void main(void)
{
    keyval = 0; // default value of key, doing nothing

    // infinite loop, keyboard scaning by the control of cpu
    while(1)
    {
        key_scan();
        
        switch(keyval)
        {
            case 1: forward(); break;
            case 2: backward(); break;
            case 3: stop(); break;
            case 4: flash(); break;
        }    
    }
}