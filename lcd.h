#ifndef _LCD_H_
#define _LCD_H_

#include "../pre_emptive_os/api/general.h"

static void delay37us(void);
static void initLCD(void);
static void delay37us(void);
void secondRowLCD(void);
void clearLCD(void);
static void writeLCD(tU8 reg, tU8 data);
static void lcdBacklight(tU8 onOff);
void printStringLCD(tU8 *string);
#endif
