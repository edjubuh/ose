/**
* @file mechop.c
* @brief Source file for mecanum functions
*
* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.
* Portions of this file may contain elements from the PROS API.
* See include/API.h for additional notice.
***********************************************************************/

#include <math.h>
#include "main.h"

#include "dios/mechop.h"
#include "dios/Chassis.h"


/**
 * @brief Functions for converting radians to degrees
 * NOTE: M_PI is defined in math.h
 *
 * @param radians
 *        radian value of the angle to be converted
 *
 * @return Degree value (as a double or as an int: depends on the function)
 */
double toDegrees(double radians)
{
	return (radians * (180/M_PI));
}
int toIntDegrees(double radians)
{
	return (int)(radians * (180/M_PI));
}


/**
 * @brief Returns theta, determined from function parameters.
 *
 * @param x
 *        X value for a joystick.
 * @param y
 *        Y value for a joystick.
 *
 * @return Theta as a double; unrounded
 */
double getJoyTheta(int x, int y)
{
	if (x == 0)
	{
		if (y >= 0)
			return 90.0;
		else
			return -90.0;
	}
	else if (x < 0)
	{
		if ( atan( ((double)y)/x) < 0.0)
			return ( toDegrees( atan( ((double)y)/x)) + 180.0 );
		else
			return ( toDegrees( atan( ((double)y)/x)) - 180.0 );
	}
	else
		return ( toDegrees( atan( ((double)y)/x) ) );
}



/**
 * @brief Rounds theta values to one value based on what sector theta is in.
 *        Is in degrees in order to simplify comparisons.
 *
 * @param theta
 *        Angle to be rounded (in degrees!) and its sector determined.
 *        NOTE: The double 'theta' will be rounded to two decimal places and converted to
 *              an int in order to prevent error in comparisons.
 *              EXAMPLE: double 24.526312  ------> int 2453
 *
 * @return Sector number 0, 45, 90, 135, 180, -45, -90, -135; & -1 for error
 *  \|/
 * --0--
 *- /|\
 */
int thetaSector( double theta )
{
	//get rid of that quirky decimal place!
	int ltheta = (int)( roundf(theta * 100) );
	if (ltheta >= -2250 && ltheta <= 2250)
		return 0;
	else if (ltheta >= -6750  && ltheta < -2250 )
		return -45;
	else if (ltheta >= -11250 && ltheta < -6750 )
		return -90;
	else if (ltheta >= -15750 && ltheta < -11250 )
		return -135;
	else if ( (ltheta >= -180 && ltheta < -15750 ) || (ltheta > 15750 && ltheta <= 180) )
		return 180;
	else if (ltheta > 2250 && ltheta <= 6750 )
		return 45;
	else if (ltheta > 6750 && ltheta <= 11250 )
		return 90;
	else if (ltheta > 11250 && ltheta <= 15750 )
		return 135;
	else
		return -1;
}

/**
 * @brief Calculates average joy value
 *
 * @param val1
 *        first joy value
 * @param val2
 *        second joy value
 *
 * @return Average of the two values as an int (always positive though)
 */
int aJoy(int val1, int val2)
{
	return (abs(val1) + abs(val2)) / 2;
}

/**
 * @brief Calculates the hypotenuse of a triangle with legs x and y.
 *
 * @param x
 *        first leg of the triangle (or an joystick x value)
 * @param y
 *        second leg of the triangle (or an joystick y value)
 *
 * @return The hypotenuse of the triangle (as a double & unrounded).
 */
double cHypo(int x, int y)
{
	return sqrt( pow(x, 2) * pow(y, 2) );
}

/**
 * @brief Calculates the average of two hypotenuses
 *
 * @param h1
 *        first hypotenuse
 * @param h2
 *        second hypotenuse
 *
 * @return The average hypotenuse value as an int (for use in functions accepting ints).
 */
int aHypo(double h1, double h2)
{
	return (int)(roundf((h1 + h2) / 2.0));
}

/**
 * @brief Controls robot wheels.
 */
void JoystickControl()
{
	int l3 = joystickGetAnalog(1, 3), //up left y
	    l4 = joystickGetAnalog(1, 4), //side left x
	    r1 = joystickGetAnalog(1, 1), //side right x
	    r2 = joystickGetAnalog(1, 2); //up right y
	int theta1 = thetaSector( getJoyTheta(l4, l3) ),
	    theta2 = thetaSector( getJoyTheta(r1, r2) );

	//Up / Down
	if (theta1 == 90 && theta2 == 90)
		ChassisSet( aJoy(r1, l4), aJoy(r1, l4), false);
	else if (theta1 == -90 && theta2 == -90)
		ChassisSet( -aJoy(r1, l4), -aJoy(r1, l4), false);
	//Left / Right
	else if (theta1 == 0 && theta2 == 0)
		ChassisSetMecanum(-M_PI/2, aJoy(l4, r1), 0, false);
	else if (theta1 == 180 && theta2 == 180)
		ChassisSetMecanum(M_PI/2, aJoy(l4, r1), 0, false);
	//Tank Drive ^+v or v+^
	else if (theta1 == 90 && theta2 == -90)
		ChassisSet( aJoy(r1, l4), -aJoy(r1, l4), false);
	else if (theta1 == -90 && theta2 == 90)
		ChassisSet( -aJoy(r1, l4), aJoy(r1, l4), false);
	//northeast
	else if (theta1 == 45 && theta2 == 45)
		ChassisSetMecanum(-M_PI/4, aHypo( cHypo(l4, l3), cHypo(r1, r2) ), 0, false);
	//northwest
	else if (theta1 == 135 && theta2 == 135)
		ChassisSetMecanum(M_PI/4, aHypo( cHypo(l4, l3), cHypo(r1, r2) ), 0, false);
	//southeast
	else if (theta1 == -45 && theta2 == -45)
		ChassisSetMecanum(-3 * M_PI/4, aHypo( cHypo(l4, l3), cHypo(r1, r2) ), 0, false);
	//southwest
	else if (theta1 == -135 && theta2 == -135)
		ChassisSetMecanum(3 * M_PI/4, aHypo( cHypo(l4, l3), cHypo(r1, r2) ), 0, false);
	//strafe right
	else if ((theta1 == 90 || theta1 == -90) && theta2 == 0)
		ChassisSet(l3, (int)(l3 * (1.0 - abs( ((double)r1) / STRAFE_CONST ) ) ), false);
	//strafe left
	else if (theta1 == 180 && (theta2 == 90 || theta2 == -90))
		ChassisSet( (int)(r2 * (1.0 - abs( ((double)l4) / STRAFE_CONST ) ) ), r2, false);
	//TODO: Strafe up / down
	else
		ChassisSet(0, 0, false); //stop, so motors don't break
}