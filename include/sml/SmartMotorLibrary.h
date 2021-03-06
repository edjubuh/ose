/**
 * @file include/sml/SmartMotorLibrary.h
 * @sa libsml//SmartMotorLibrary.c  @link libsml/SmartMotorLibrary.c 
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#ifndef SMARTPIDLIBRARY_H_
#define SMARTPIDLIBRARY_H_

#define DEFAULT_SKEW 0.5 // default SkewPerMsec
#define MUTEX_TAKE_TIMEOUT	100 // default timeout when attempting to take a mutex

/**
 * @struct PIDController
 * Represents the variables and constants of a PID Controller.
 */
typedef struct
{
	/**
	 * @brief A function pointer to the function that executes the PID Controller's output.
	 *
	 * This function's signature should comply with the standard libsml motor controller function.
	 */
	void(*Execute)(int, bool);

	/**
	 * @brief A function pointer to the call function that returns the PID Controller's input
	 *
	 * This functions' signature does not take any arguments and returns an int.
	 */
	int(*Call)(void);

	/**
	 * @brief The desired end state of the controller, represented directly by the call function
	 */
	int Goal;

	/**
	 * @brief The proportional gain constant.
	 */
	double Kp,
		/**
		 * @brief The integral gain constant.
		 */
		 Ki,
		 /**
		  * @brief The derivative gain constant.
		  */
		  Kd;
	/**
	 * @brief The maximum the integral can build up to, preventing integral windup and take over of control
	 *
	 * If the integral is above this value, sets the integral to this value. (Does not reset to 0)
	 */
	int MaxIntegral,
		/**
		 * @brief The minimum the integral build up to, preventing integral windup and take over of control.
		 *
		 * If the integral is below this value, the controller will set the integral to this value. <br>
		 * MinIntegral is almost always the corresponding negative value of MaxIntegral.
		 */
		MinIntegral;

	/**
	 * @brief The tolerance acceptable of the goal and the current value.
	 *
	 * If the input is within +/- AcceptableTolerance of the goal, output will be set to 0 and a "completed" state is triggered.
	 */
	unsigned int AcceptableTolerance;

	/**
	 * @brief FOR INTERNAL USAGE ONLY
	 *
	 * Represents the integral over all of the runs.
	 */
	int integral,
		/**
		 * @brief FOR INTERNAL USAGE ONLY
		 *
		 * Represents the previous error from the last process of the loop
		 */
		prevError;
	/**
	 * @brief FOR INTERNAL USAGE ONLY
	 *
	 * Represents the last time the controller was computed, from the millis() function
	 */
	unsigned long prevTime;
} PIDController;

/**
* @struct Motor
* Represents a physical motor for usage with the Smart Motor Libary
*/
typedef struct
{
	/**
	 * @brief This pointer will recalculate the commanded speed
	 *
	 * This method's signature takes the motor's commanded and returns a new goal speed. <br>
	 * This method is usually NULL, but if not null, will be run
	 * @deprecated RecalculatedCommanded is no longer implemented to increase computational load
	 */
	int(*RecalculateCommanded)(int);
	/**
	 * @brief Represents the port on the Cotrex Microcontroller where the motor is plugged into
	 * @pre Bounds: [1,10]
	 */
	unsigned char channel;

	/**
	 * @brief If set to true in MotorConfigure(), commanded values will be flipped to be -1 * commanded. Used to flip motors
	 */
	int inverted;

	/**
	 * @brief The goal speed PWM setting. <br>
	 * @pre Bounds: [-127,127]
	 */
	int commanded;

	/**
	 * @brief Calculates the amount to delta change per millisecond of a motor's PWM value.
	 *
	 * If the current required change is less than appropriate skewPerMsec, the motor's output will be set to the commanded.
	 */
	double skewPerMsec;
	/**
	* @brief FOR INTERNAL USE ONLY
	*
	* The last time the motor was updated from the smart motor library from millis()
	*/
	long lastUpdate;
} Motor;
///@cond
void InitializeMotorManager();
void StopMotorManager();
void MotorManagerTask(void *);
void MotorConfigure(int, bool, double);
void MotorChangeRecalculateCommanded(int, int(*foo)(int));
bool MotorSet(int, int, bool);
int MotorGet(int);
///@endcond
#endif