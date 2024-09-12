// Ex76: digital voltmeter based on ADC0832
#include <reg51.h> 
#include <intrins.h> // Definition of _nop_() included

sbit CS = P3^4;
sbit CLK = P1^0;
sbit DIO = P1^1;

unsigned char code digit[10] = {"0123456789"}; // Define digits string to display number
unsigned char code Str[]={"Volt="};			   // Comment on what is displaying

sbit RS = P2^0; // Registers selection bit
sbit RW = P2^1;	// Write/read selection bit	
sbit E = P2^2; // Enable bit
sbit BF = P0^7; // Busy flag bit

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

void delaynms(unsigned char n)
{
	unsigned char i;
	
	for(i = 0; i < n; i++)
	{
		delay1ms();
	}
}


// Judge whether LCD is in busy or not
// return = 1, busy
// return = 0, available
bit BusyTest(void)
{
	bit res;
	
	RS = 0;	 
	RW = 1; // According to the rules, status can be read while RS is at low and RW is at high
	E = 1;	// Write is availabel if E is 1
	_nop_(); 
	_nop_();
	_nop_();
	_nop_(); // Four idle operations of machine cycle for responsive time of hardware
	res = BF; // Get the busy flag
	E = 0; 	  // Reset E to low
	
	return res;
}

// Write mode setting insturctions or display addresses into the LCD
void WriteInstruction(unsigned char dictate)
{
	while(1 == BusyTest()); // Awaiting while LCD in busy
	RS = 0;	// Both RS and R/W are in low, instructions can be written
	RW = 0;
	E = 0;	// E is set to low
			// Because there should be a ascending edge before writing a instruction
	
	_nop_();
	_nop_(); // Two machien cycles for hardware's response
	P0 = dictate;	// Write data to P0, aka address or instruction
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E = 1; 	// E set to high
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E = 0;	// A descending edge of E that makes LCD to start excuting instructions
}

// Designate the actual address the string will display
void WriteAddress(unsigned char x)
{
	WriteInstruction(x | 0x80);	// The correct way to display is "80H + address code"
}

// Write data(ASCII) to the LCD
void WriteData(unsigned char y)
{
	while(BusyTest() == 1);
	RS = 1; // RS is high and R/W is low, then write data is available
	RW = 0;
	E = 0;	// E is set to low
			// Because there should be a ascending edge before writing a instruction 
			
	P0 = y;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E = 1;	// E set to high
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	E = 0;	// A descending edge of E that makes LCD to start excuting instructions 
}

// Initialzation of LCD display mode
void LcdInitiate(void)
{
	delaynms(15);
	WriteInstruction(0x38); // Display mode setting: 16x2 display, 5x7 dot matrix, 8 bits data interface
	delaynms(5);
	WriteInstruction(0x38);
	delaynms(5);
	WriteInstruction(0x38);
	delaynms(5);			// Three times to make sure initialzation successfully
	
	WriteInstruction(0x0C); // Display mode setting: Dispaly on, no cursor, cursor not flashes
	delaynms(5);
	WriteInstruction(0x06); // Display mode setting: cursor right shift but characters not mvoe
	delaynms(5);
	WriteInstruction(0x01); // Display mode setting: insturction to clear screen
	delaynms(5);
}


/*
** Displaying voltage :
**
*/

// Display volt symbol
void display_volt(void)
{
	unsigned char i;
	WriteAddress(0x03);	// Display at row 2, column 1
	
	i = 0;
	while(Str[i] != '\0')
	{
		WriteData(Str[i]);
		i++;
	}
}

/*****************************************************
函数功能：显示电压的小数点
***************************************************/   
void display_dot(void)
{         
	WriteAddress(0x09);	  //写显示地址，将在第1行第10列开始显示		   
	WriteData('.');       //将小数点的字符常量写入LCD		
}

/*****************************************************
函数功能：显示电压的单位(V)
***************************************************/   
void display_V(void)
{
    WriteAddress(0x0c); //写显示地址，将在第2行第13列开始显示	
	WriteData('V');     //将字符常量写入LCD						
}
/*****************************************************
函数功能：显示电压的整数部分
入口参数：x
***************************************************/ 
void display1(unsigned char x)
{
	WriteAddress(0x08);    //写显示地址,将在第2行第7列开始显示
	WriteData(digit[x]);    //将百位数字的字符常量写入LCD
 }
/*****************************************************
函数功能：显示电压的小数数部分
入口参数：x
***************************************************/ 
void display2(unsigned char x)
{
 	unsigned char i,j;
	i=x/10;            //取十位（小数点后第一位）
	j=x%10;            //取个位（小数点后第二位）
	WriteAddress(0x0a);      //写显示地址,将在第1行第11列开始显示
	WriteData(digit[i]);     //将小数部分的第一位数字字符常量写入LCD
	WriteData(digit[j]);     //将小数部分的第一位数字字符常量写入LCD
}

// A/D convert
unsigned char A_D(void)
{
	unsigned char i, dat;
	
	CS = 1;	// The beginning of one conversion cycle
	CLK = 0;	// Preparation for the first pulse	
	CS = 0;		// CS is available while CS is 0
	
	DIO = 1;	// DIO set to 1 to define the beginning signal
	CLK = 1;	// The first signal
	CLK = 0;	// DIO must be high before the desending edge of the first pulse
	DIO = 1;	// The second pulse
			    // Before the descending edges of the second and third pulses, DI must be set 1/0
				// to select corresponding channel and channel 0 was selected
	CLK = 1;
	CLK = 0;	// The descendig edge of the second pulse
	DIO = 0;	// DI set to 0 to select channel 0
	CLK = 1;
	CLK = 0;	// The descendig edge of the third pulse
	DIO = 1;	// DI is not available after the third pulse
	
	for(i = 0; i < 8; i++)
	{
		CLK = 1;
		CLK = 0; 
		dat <<= 1;
		dat |= DIO;
	}	
	
	CS = 1; 
	return dat;
}


void main(void)
{
	unsigned int AD_val;    //储存A/D转换后的值
	unsigned char Int,Dec;   //分别储存转换后的整数部分与小数部分
	LcdInitiate();         //将液晶初始化
	delaynms(5);           //延时5ms给硬件一点反应时间	
	display_volt();        //显示温度说明
	display_dot();         //显示温度的小数点
	display_V();           //显示温度的单位	

   while(1)
	{
		AD_val= A_D();    //进行A/D转换
		Int=(AD_val)/51;  //计算整数部分
		// Dec=(AD_val%51)*100/51;    //计算小数部分
		Dec=(AD_val%51)*19.6/10;	// 19.6mV for each one of AD_val
		display1(Int);     //显示整数部分
		display2(Dec);     //显示小数部分
		delaynms(250);     //延时250ms
	}	
}



