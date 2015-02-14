/**
 * @file vulcan/Chassis.c
 * @brief Source file for Chassis functions
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include <math.h>

#include "main.h"
#include "vulcan/Chassis.h"

#include "sml/SmartMotorLibrary.h"
#include "vulcan/CortexDefinitions.h"

static Gyro gyro;

/**
 * @brief Sets the left and right motors of the chassis as specified by the parameters.
 *        Speeds of left and right will be scaled down relative to each other if either
 *        is greater than 127.
 *
 * @param left
 *        The speed of the left side of the chassis.
 * @param right
 *        The speed of the right side of the chassis.
 * @param immediate
 *        Determines if speed input change is immediate or ramped according to SML
 */
void ChassisSet(int left, int right, bool immediate)
{
	//statements below catch speeds out of range
	//catches left
	if (abs(left) > 127)
		left = signbit(left) ? -127 : 127;
	//catches right
	if (abs(right) > 127)
		right = signbit(right) ? -127 : 127;

	//set left and right wheel speeds according to parameters above.
	MotorSet(MOTOR_CHASSIS_FRONTLEFT, left, immediate);
	MotorSet(MOTOR_CHASSIS_FRONTRIGHT, right, immediate);
	MotorSet(MOTOR_CHASSIS_REARLEFT,   left,  immediate);
	MotorSet(MOTOR_CHASSIS_REARRIGHT,  right, immediate);
}

/**
 * @brief Sets the chassis to go at heading with speed and rotation. See additional
 *        mecanum conversion description in notebook.
 *
 * @param heading
 *        Direction of the chassis to go in radians (0 rad is forward, PI/2 is strafe
 *        left)
 * @param speed
 *        [-127,127] Speed of the chassis. At least one motor will go this speed, all
 *        others scaled appropriately to get kinematics
 * @param rotation
 *        [-127,127] Rotation of the chassis where 127 is clockwise (right) rotation.
 *        This is a final additive, so can overpower heading calculation
 * @param immediate
 *        Determines if speed input change is immediate or ramped according to SML
 */
void ChassisSetMecanum(double heading, int speed, int rotation, bool immediate)
{
	if (abs(speed) > 127)
		speed = signbit(speed) ? -127 : 127;
	if (abs(rotation) > 127)
		rotation = signbit(rotation) ? -127 : 127;

	int rightDiag = speed * cos(heading + (M_PI/4));
	int leftDiag = speed * cos(heading - (M_PI/4));

	int frontRight = rightDiag + rotation,
		rearRight = leftDiag + rotation,
		frontLeft = leftDiag - rotation,
		rearLeft = rightDiag - rotation;

	int max = 127;
	if (abs(frontRight) > max)
		max = abs(frontRight);
	if (abs(rearRight) > max)
		max = abs(rearRight);
	if (abs(frontLeft) > max)
		max = abs(frontLeft);
	if (abs(rearLeft) > max)
		max = abs(rearLeft);

	double speedScale = 127.00 / max;

	MotorSet(MOTOR_CHASSIS_FRONTRIGHT, (int)(frontRight * speedScale), immediate);
	MotorSet(MOTOR_CHASSIS_REARRIGHT,  (int)(rearRight * speedScale),  immediate);
	MotorSet(MOTOR_CHASSIS_FRONTLEFT,  (int)(frontLeft * speedScale),  immediate);
	MotorSet(MOTOR_CHASSIS_REARLEFT,   (int)(rearLeft * speedScale),   immediate);

}

/**
 * @brief Input radians are snapped/rounded to the nearest PI/4 (45°)
 *
 * @param rad
 *        The angle to snap
 */
static double snapDegrees(double rad)
{
	double abs_rad = abs(rad);
	if (abs_rad < (M_PI / 6)) 
		return 0;
	
	if (abs_rad < (M_PI / 3))
		return (M_PI / 4) * (signbit(rad) ? -1 : 1);

	if (abs_rad < ((2 * M_PI) / 3))
		return (M_PI / 2) * (signbit(rad) ? -1 : 1);

	if (abs_rad < ((5 * M_PI) / 6))
		return (3 * M_PI / 4) * (signbit(rad) ? -1 : 1);

	else
		return M_PI;
}

/**
 * @brief Calculates the chassis output from the given conditions. See documentation addendum covering Mecanum control
 *
 * @param l_x
 *        Left x-coordinate
 *
 * @param l_y
 *        Left y-coordinate
 *
 * @param r_x
 *        Right x-coordinate
 *
 * @param r_y
 *        Right y-coordinatef
 */
void ChassisSnappedJoystickControl(int l_x, int l_y, int r_x, int r_y)
{
	// l_* for left and r_* for right, * for average
	// x: x-coordinate, y: y-coordinate, a: angle (rad)
	// s: speed (hypotenuse)

	if (abs(l_y) < 20 || abs(r_y) < 20) // If the y-component is less than 20, always be in tank control
		ChassisSet(l_y, r_y, false);

	// Calculate angle
	double l_a = snapDegrees(atan2(l_y, l_x)),
		r_a = snapDegrees(atan2(r_y, l_y));

	if (l_a != r_a) // Angles aren't equal so set chassis to 0.
		ChassisSet(0, 0, false);

	// Calculate speed by Pythagorean theorem
	double l_s = sqrt(pow(l_x, 2) + pow(l_y, 2)),
		r_s = sqrt(pow(r_x, 2) + pow(r_y, 2));

	double s = (l_s + r_s) / 2;

	if (l_a == 0)
		ChassisSetMecanum(M_PI / 2, s, 0, false);

	else if (l_a == (M_PI / 4))
		ChassisSetMecanum(M_PI / 4, s, 0, false);

	else if (l_a == (M_PI / 2))
		ChassisSetMecanum(0, s, 0, false);

	else if (l_a == (3 * M_PI / 4))
		ChassisSetMecanum(-1 * M_PI / 4, s, 0, false);

	else if (l_a == M_PI)
		ChassisSetMecanum((-1 * M_PI / 2), s, 0, false);

	else if (l_a == (-1 * M_PI / 4))
		ChassisSetMecanum((3 * M_PI / 4), s, 0, false);

	else if (l_a == (-1 * M_PI / 2))
		ChassisSetMecanum(M_PI, s, 0, false);

	else if (l_a == (-3 * M_PI / 2))
		ChassisSetMecanum((-3 * M_PI / 4), s, 0, false);

	// TODO: Implement hyperbolic strafing, see addendum documentation covering mecanum control

	else ChassisSet(0, 0, false);
}

/**
* @brief Initializes the chassis motors with the SML and creates the PID controllers for the
*        chassis.
*/
void ChassisInitialize()
{
	MotorConfigure(MOTOR_CHASSIS_FRONTLEFT,  true, 3);
	MotorConfigure(MOTOR_CHASSIS_FRONTRIGHT, false,  3);
	MotorConfigure(MOTOR_CHASSIS_REARLEFT,   false, 3);
	MotorConfigure(MOTOR_CHASSIS_REARRIGHT,  false,  3);

	gyro = gyroInit(ANA_GYROSCOPE, 196);
}