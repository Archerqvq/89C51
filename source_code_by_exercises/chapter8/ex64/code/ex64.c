// example 64:
// Stand-alone keypad controls stepper motor experiments
#include <reg51.h>

sbit S1 = P1^4; // s1 as P1.4
sbit S2 = P1^5; // s2 as P1.5
sbit S3 = P1^6; // s3 as P1.6

unsigned char keyval; // to store key value
unsigned char ID;     // to store function symbol

// software debouncing
void delay(void)
{
    unsigned char i, j;

    for(i = 0; i < 150; i++)
    {
        for(j = 0; j < 100; j++)
        {
        
        }
    }
}

// The stepper motor rotates with a delay, and the longer the delay time, the slower the rotational speed
void motor_delay(void)
{
    unsigned int i;
    for(i = 0; i < 2000; i++)
    {
    
    }
}

// The stepper motor rotates forward
void forward(void)
{
    P0 = 0xFC; // P0 low four-bit output ring pulse -> 1100
    motor_delay();
    P0 = 0xF6;  // P0 low four-bit output ring pulse -> 0110
    motor_delay();
    P0 = 0xF3; // 0011
    motor_delay();
    P0 = 0xF9;   // 1001
    motor_delay();
}


// The stepper motro rotates backward
void backward(void)
{
    P0 = 0xFC; // P0 low four-bit output ring pulse -> 1100
    motor_delay();
    P0 = 0xF9; // P0 low four-bit output ring pulse -> 1001
    motor_delay();
    P0 = 0xF3; // P0 low four-bit output ring pulse -> 0011
    motor_delay();
    P0 = 0xF6; // P0 low four-bit output ring pulse -> 0110
    motor_delay();    
} 

// The stepper stop rotating
void stop(void)
{
    P0 = 0xFF; // Stop output pulse
}

void main(void)
{
    TMOD = 0x01; // t0 in mode 1
    EA = 1;      // enable global interrupt
    ET0 = 1;     // t0 interrupt allowed
    TR0 = 1;     // t0 enabled

    TH0 = (65536 - 200) / 256; // t0 assignment, every 200 counts generate interrupt
    TL0 = (65536 - 200) % 256; // t0 assignment
    keyval = 0; 
    ID = 0;  // function sysmbol assgian to 0

    while(1)
    {
        switch(keyval)
        {
             case 1: forward(); break;
             case 2: backward(); break;
             case 3: stop();     break;

        }
    }
}

// t0 interrupt service request
void timer0_serve(void) interrupt 1 using 1
{
     TR0 = 0; // stop t0

     if((P1&0xF0) != 0xF0) // key pressed detected
     {
        delay(); // debouncing

        if((P1&0xF0) != 0xF0) // key pressed confirmation
        {
            if(S1 == 0) // if key actually pressed
            {
                keyval = 1;
            }
            if(S2 == 0) // if key actually pressed
            {
                keyval = 2;
            }
            if(S3 == 0) // if key actually pressed
            {
                keyval = 3;
            }
        }
     }

    TH0 = (65536 - 200) / 256; // t0 reassignment, every 200 counts generate interrupt
    TL0 = (65536 - 200) % 256; // t0 reassignment

    TR0 = 1; // enable t0
}