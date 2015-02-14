/**
 * @file include/vulcan/mechop.h
 * @author Robert Shrote
 * @brief Header file for advanced mecanum control <br>
 * See vulcan/mechop.c for details of all functions
 *
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/main.h for additional notice.
 ********************************************************************************/

#ifndef MECHOP_H
#define MECHOP_H
/**
 * @brief Constants
 */
#define STRAFE_CONST	254
#define THRESHOLD		25

double getJoyTheta(int, int);
int thetaSector(double);
int aJoy(int, int);
double cHypo(int, int);
int aHypo(double, double);
void JoystickControl();

#endif /*MECHOP_H*/