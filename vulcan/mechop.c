/**
 * @file vulcan/mechop.c
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
 * @brief Returns theta, determined from function parameters.
 *
 * NOTE: Constants defined in math.h
 *       M_PI     pi constant
 *       M_PI_2   pi/2 constant
 *       M_PI_4   pi/4 constant
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
            return M_PI_2;
        else
            return -M_PI_2;
    }
    else if (x < 0)
    {
        if ( atan( ((double)y)/x) < 0.0)
            return (( ( atan( ((double)y)/x)) + M_PI ));
        else
            return (( ( atan( ((double)y)/x)) - M_PI ));
    }
    else
        return ( atan( ((double)y)/x) );
}



/**
 * @brief Rounds theta values and calculates sector theta is in.
 *
 * @param theta
 *        Angle to be rounded (in radians) and its sector determined.
 *        NOTE: The double 'theta' will be rounded to three decimal places and converted to
 *              an int in order to prevent error in comparisons.
 *              EXAMPLE: double 1.526312 r  ------> int 1526
 *
 * @return Sector number +/-0, +/-1, +/-2, +/-3, +/-4, +/-5, +/-6, +/-7
 *  \|/
 * --0--
 *- /|\
 */
int thetaSector( double theta )
{
    //get rid of that quirky decimal place!
    //integer representation of theta (in radians) and modulus by 
    return ((int)( roundf(theta * 1000) )) / 393;
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
 *        first leg of the triangle (or a joystick x value)
 * @param y
 *        second leg of the triangle (or a joystick y value)
 *
 * @return The hypotenuse of the triangle (as a double & unrounded).
 */
double cHypo(int x, int y)
{
    return sqrt( (x * x) + (y * y) );
    //use x*x instead of pow(x, 2) to decrease computation time.
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
    return (int)(roundf((h1 + h2) / 2));
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
    int left = thetaSector( getJoyTheta(l4, l3) ), //left joystick
        right = thetaSector( getJoyTheta(r1, r2) ); // right joystick

    //Up / Down
    if ( (left  == 3 || left  == 4) &&
         (right == 3 || right == 4)    )
        ChassisSet( aJoy(r1, l4),
                    aJoy(r1, l4),
                    false);
    else if ( (left  == -3 || left  == -4) &&
              (right == -3 || right == -4)    )
        ChassisSet( -aJoy(r1, l4),
                    -aJoy(r1, l4),
                    false);


    //Left / Right
    else if ( (left  == 0 || left  == -0) &&
              (right == 0 || right == -0)    )
        ChassisSetMecanum(-M_PI_2, aJoy(l4, r1), 0, false);
    else if ( (left  == 7 || left  == -7) &&
              (right == 7 || right == -7)    )
        ChassisSetMecanum(M_PI_2, 
                          aJoy(l4, r1), 
                          0, false);


    //Tank Drive ^+v or v+^
    else if ( (left  == 3 || left  == 4) &&
              (right == -3 || right == -4)    )
        ChassisSet( aJoy(r1, l4),
                    -aJoy(r1, l4),
                    false);
    else if ( (left  == -3 || left  == -4) &&
              (right == 3  || right == 4)    )
        ChassisSet( -aJoy(r1, l4),
                    aJoy(r1, l4),
                    false);


    //northeast / northwest
    else if ( (left  == 2 || left  == 3) &&
              (right == 2 || right == 3)    )
        ChassisSetMecanum(-M_PI_4,
                          aHypo( cHypo(l4, l3), cHypo(r1, r2) ),
                          0, false);
    else if ( (left  == 5 || left  == 6) &&
              (right == 5 || right == 6)    )
        ChassisSetMecanum(M_PI_4, 
                          aHypo( cHypo(l4, l3), cHypo(r1, r2) ),
                          0, false);


    //southeast / southwest
    else if ( (left  == -2 || left  == -3) &&
              (right == -2 || right == -3)    )
        ChassisSetMecanum(-3.0 * M_PI_4,
                          aHypo( cHypo(l4, l3), cHypo(r1, r2) ),
                          0, false);
    else if ( (left  == -5 || left  == -6) &&
              (right == -5 || right == -6)    )
        ChassisSetMecanum(3.0 * M_PI_4,
                          aHypo( cHypo(l4, l3), cHypo(r1, r2) ),
                          0, false);


    //strafe right / left
    else if ( ( (left == 3 || left == 4) || (left == -3 || left == -4) ) &&
              (right == 0 || right == -0)                                    )
        ChassisSet(l3,
                   (int)(l3 * (1.0 - abs( ((double)r1) / STRAFE_CONST ) ) ),
                   false);
    else if ( (left == 7 || left == -7) && 
              ( (right == 3 || right == 4) || (right == -3 || right == -4) ) )
        ChassisSet( (int)(r2 * (1.0 - abs( ((double)l4) / STRAFE_CONST ) ) ),
                    r2,
                    false);


    //TODO: Strafe up / down

    //By default: stop, so motors don't break
    else
        ChassisSet(0, 0, false);
}