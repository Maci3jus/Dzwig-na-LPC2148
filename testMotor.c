/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2005 Embedded Artists AB
 *
 *****************************************************************************/


#include <general.h>
#include <lpc2xxx.h>
#include "testMotor.h"


#include <config.h>

#include <stdio.h>

/*****************************************************************************
 *
 * Description:
 *    A process entry function. 
 *
 ****************************************************************************/
void

/*!
void initializemotor odpowiada po kolei za ustawienie pinów 9,8,10 na zero.
*/


initializemotor(void){


  IODIR0 |= 0x00000700;
  IOCLR0  = 0x00000700;


}

/*!
void enablemotor odpowiada po kolei za ustawienie pinów 9,8 na zero i pinu 10 na jeden, ponieważ jest to pin enable do sterowania silnikiem.
*/

void enablemotor(void)
{


  IODIR0 |= 0x00000400;
  IOSET0  = 0x00000400;
  IODIR0 |= 0x00000300;
  IOCLR0  = 0x00000300;

}

/*!
void enablemotorright jest funkcją włączająca ruch silnika w lewo.
Ustawia pinu 0.8 w tryb PWM
x - to przekazywana prędkość
Ustawia PWMMR4 na x
dla x 500 mamy 50% prędkości
*/

void enablemotorright(tU16 x)
{
  osSleep(10);
  PINSEL0 |= 0x00020000;
  PWMPR = 60-1;
  PWMMR0 = 1000;
  PWMMR4 = x;
  PWMMCR = 0x00000002;
  PWMLER = 0x11;
  PWMPCR = 0x00001000;
  PWMTCR = 0x09;


}

/*!
void motorfaster/slower zmieniają nam zmienną x, która odpowiada za prędkość.
*/



tU16 motorfaster(tU16 x)
{

if(x<1000){
	 x +=200;
}
return x;
}

tU16 motorslower(tU16 x)
{

if(x>=0){
	 x -=200;
}
return x;
}

/*!
void enablemotorleft jest funkcją włączająca ruch silnika w lewo.
Ustawia pinu 0.9 w tryb PWM
x - to przekazywana prędkość
Ustawia PWM6 na x
dla x 500 mamy 50% prędkości
*/


void enablemotorleft(tU16 x)
{

	osSleep(10);
	  PINSEL0 |= 0x00080000;
	  PWMPR = 60-1;
	  PWMMR0 = 1000;
	  PWMMR6 = x;
	  PWMMCR = 0x002;
	  PWMLER = 0x41;
	  PWMPCR = 0x4000;
	  PWMTCR = 0x0009;

}

/*!
void disablemotor jest funkcją wyłączająca ruch silnika.
Ustawia pin 0.8-0.7 na 0
Ustawia PWM4/6 na 0
*/

void disablemotor(void)
{

	osSleep(10);
  IODIR0 |= 0x00000300;
  IOCLR0  = 0x00000300;
  PWMMR4= 0;
  PWMLER = 0x11;
  PWMMR6= 0;
  PWMLER = 0x41;
}


