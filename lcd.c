#include "../pre_emptive_os/api/general.h"
#include "../pre_emptive_os/api/osapi.h"
#include <lpc2xxx.h>

#define LCD_DATA      0x00ff0000  //P1.16-P1.23
#define LCD_E         0x02000000  //P1.25
#define LCD_RW        0x00400000  //P0.22
#define LCD_RS        0x01000000  //P1.24
#define LCD_BACKLIGHT 0x40000000  //P0.30

void secondRowLCD(void);
void clearLCD(void);
static void lcdBacklight(tU8 onOff);
void printStringLCD(tU8 *string);

static void
writeLCD(tU8 reg, tU8 data)
{
	volatile tU8 i;

	if (reg == 0){
		IOCLR1 = LCD_RS;
	}
	else{
		IOSET1 = LCD_RS;
	}

	IOCLR0 = LCD_RW;
	IOCLR1 = LCD_DATA;
	IOSET1 = ((tU32)data << 16) & LCD_DATA;

	IOSET1 = LCD_E;
	for(i=0; i<16; i++){
    asm volatile (" nop");
	}	//delay 15 ns x 16 = about 250 ns delay
	IOCLR1 = LCD_E;
	for(i=0; i<16; i++){
    asm volatile (" nop")
	}; //delay 15 ns x 16 = about 250 ns delay
}

static void
delay37us(void)
{
	volatile tU32 i;

//Temp test for 140 uS delay
	for(i=0; i<6*2500; i++){
		asm volatile (" nop"); //delay 15 ns x 2500 = about 37 us delay
	}
}

static void initLCD(void)
{
	IODIR1 |= (LCD_DATA | LCD_E | LCD_RS);
	IOCLR1  = (LCD_DATA | LCD_E | LCD_RS);

	IODIR0 |= LCD_RW;
	IOCLR0  = LCD_RW;

  IODIR0 |= LCD_BACKLIGHT;
  IOCLR0  = LCD_BACKLIGHT;

  writeLCD(0, 0x30);
  		delay37us();
  	    writeLCD(0, 0x30);
  	    delay37us();
  	    writeLCD(0, 0x30);
  	    delay37us();

  	    //function set
  	    writeLCD(0, 0x38);
  	    delay37us();

  	    //display off
  	    writeLCD(0, 0x08);
  	    delay37us();

}

void secondRowLCD(void){
	//move cursor to second row
	writeLCD(0, 0x80 | 0x40);
	delay37us();
}

void clearLCD(void)
{
	//display clear
    writeLCD(0, 0x01);
    osSleep(1);

	//cursor home
    writeLCD(0, 0x02);
	osSleep(1);
}

static void lcdBacklight(tU8 onOff)
{
	if (onOff == TRUE){
	  IOSET0 = LCD_BACKLIGHT;
	}
	else{
	  IOCLR0 = LCD_BACKLIGHT;
	}
}

void printStringLCD(tU8 *string)
{
	    //display control set
	    writeLCD(0, 0x06);
	    delay37us();

	    //display control set
	    writeLCD(0, 0x0c);
	    delay37us();



	tU32 i=0;
	while(string[i]!='\0')
	{
		if(i==16)
		{
			writeLCD(0, 0x80 | 0x40);
			delay37us();
		}
		writeLCD(1, string[i]);
		delay37us();
		i++;
	}
}

