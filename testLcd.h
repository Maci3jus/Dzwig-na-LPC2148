/*
 * testLcd.h
 *
 *  Created on: May 29, 2024
 *      Author: embedded
 */

#ifndef TESTLCD_H_
#define TESTLCD_H_

#include "testLedMatrix.h"
#include <config.h>
#include "buzzer.h"
#include "testMotor.h"


void ruch(tU16 liczba);
void startMenu(void);
void rysujMenu(int speed);
void rysujAwaria(void);
void rysujpredkosc(void);

#endif /* TESTLCD_H_ */
