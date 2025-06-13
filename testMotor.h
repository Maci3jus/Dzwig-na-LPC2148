/*
 * testMotor.h
 *
 *  Created on: May 29, 2024
 *      Author: embedded
 */

#ifndef TESTMOTOR_H_
#define TESTMOTOR_H_

void disablemotor(void);
void enablemotorleft(tU16 x);
tU16 motorslower(tU16 x);
tU16 motorfaster(tU16 x);
void enablemotorright(tU16 x);
void enablemotor(void);
void initializemotor(void);

#endif /* TESTMOTOR_H_ */
