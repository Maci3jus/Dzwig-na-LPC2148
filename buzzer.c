/*
 * buzzer.c
 *
 *  Created on: May 16, 2024
 *      Author: embedded
 */

/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2005 Embedded Artists AB
 *
 *****************************************************************************/


#include <general.h>
#include <lpc2xxx.h>
#include "buzzer.h"
/*!
void disablebuzz odpowiada za wyłączenie buzera.
ustawiany jest odpowiedni PWMMR2.
oraz pin 0.7 ustawiany na 1.
*/

void disablebuzz(void)
{

	 PWMMR2 = 1000;
	 PWMLER = 0x5;
  IODIR0 |= 0x00000080;
  IOSET0= 0x00000080;


}
/*!
void buzzing odpowiada za włączenie sygnału dźwiękowego buzera.
ustawiany jest odpowiedni PWMMR2 aby osiągnąc zakładaną częstotliwość
*/

void buzzing(void){
	osSleep(10);
		  PINSEL0 |= 0x00008000;
		  PWMMR0 = 1000;
		  PWMMR2 = 900;
		  PWMMCR = 0x002;
		  PWMLER = 0x5;
		  PWMPCR = 0x5400;
		  PWMTCR = 0x0009;

}

