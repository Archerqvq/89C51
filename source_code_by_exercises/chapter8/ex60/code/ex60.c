// example 60:
// Stand-alone keyboard input experiment with software debounce
#include <reg51.h>

sbit S1 = P1^4;     // s1 as P1.4
sbit LED0 = P3^0;   // LED0 as P3.0

// delay around 30ms
void delay(void)
{
    unsigned char i, j;

    for(i = 0; i < 100; i++)
    {
        for(j = 0; j < 100; j++)
        {
        
        }
    }
}

void main(void)
{
    LED0 = 0; // P3.0 output low level

    while(1)
    {
        if(S1 == 0) // P1.4 at low level when s1 is pressed
        {
            delay(); // delay for a while for debouncing

            if(S1 == 0) // s1 pressed confirmed
            {
                LED0 = !LED0; // toggle P3.0            
            }
        }
    }
}