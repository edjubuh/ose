/**
 * @file libsml/SmartMotorLibrary.c
 * @author Elliot Berman
 * @brief Source file for SML functions.
 *
 * Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.
 * Portions of this file may contain elements from the PROS API.
 * See include/API.h for additional notice.
 ********************************************************************/

#include "main.h"
#include <math.h>
#include "sml/SmartMotorLibrary.h"
#include "lcd/LCDFunctions.h"

#define MOTOR_SKEWER_DELTAT	15

static Motor Motors[10];
static Mutex Mutexes[10];
static TaskHandle MotorManagerTaskHandle;

/**
 * @brief The default recalculate function for RecalculateCommanded (takes input and returns it)
 *        This method is only accessible to this file for organizational purposes and may be opened to other files.
 * @param in
 *        Input value
 * @returns Returns in
 */
static int DefaultRecalculate(int in)
{
	return in;
}

/**
 * @brief Initializes the Motor Manager Task by creating the Motor Mutexes and starting the task.
 */
void InitializeMotorManager()
{
	for (int i = 0; i < 10; i++)
	{
		Mutexes[i] = mutexCreate();
		Motors[i].RecalculateCommanded = &DefaultRecalculate;
	}
	MotorManagerTaskHandle = taskCreate(MotorManagerTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_HIGHEST-1);
}

/**
 * @brief Kills the motor manager task, if it exists
 */
void StopMotorManager()
{
	if (MotorManagerTaskHandle != NULL) // passing NULL kills current thread, so don't allow that to happen
		taskDelete(MotorManagerTaskHandle);
}

/**
 * @brief The motor manager task processes all the motors and determines if a change needs to be made to the motor speed and executes the change if necessary
 *        This task is initialized by the InitializeMotorManager method. Do not manually create this task.
 */
void MotorManagerTask(void *none)
{
	while (true)
	{
		for (int i = 0; i < 10; i++)
		{
			if (motorGet(i+1) != Motors[i].commanded) // Motor has not been set to target
			{
				int current = motorGet(i+1);
				int command = Motors[i].commanded;
				double skew = Motors[i].skewPerMsec;
				int out = 0;
				if (abs(command - current) < (skew * (millis() - Motors[i].lastUpdate))) // If skew is less than required delta-PWM, set commanded to output
					out = command;
				else
					out = (current + (int)(skew * (millis() - Motors[i].lastUpdate) * (command - current > 0 ? 1 : -1)));

				if (!mutexTake(Mutexes[i], 5)) // Grab mutex if possible, if it's not available (being changed by MotorSet()), skip the motor check.
					continue;

				motorSet(i+1, out);

				mutexGive(Mutexes[i]);
			}
			Motors[i].lastUpdate = millis();
		}
		delay(MOTOR_SKEWER_DELTAT);
	}
}

/**
 * @brief Change the motor speed
 *
 * @param channel
 *        The port of the motor (1-10)
 *
 * @param set
 *        The PWM value of the motor to set it to. Will be checked and forced back into the bounds [-127,127]
 *
 * @param immediate
 *        Will change the speed of the motor immediately, bypassing the motor manager ramping if set to true.
 *
 * @returns Returns true if MotorSet was successful.
 */
bool MotorSet(int channel, int set, bool immediate)
{
	if (channel > 10 || channel < 1)
		return false;
	if (set > 127)
		set = 127;
	else if (set < -127)
		set = -127;

	channel--;

	if (immediate)
	{
		if (!mutexTake(Mutexes[channel], MUTEX_TAKE_TIMEOUT)) return false;

		motorSet(channel + 1, set * Motors[channel].inverted);
		mutexGive(Mutexes[channel]);
	}

	if (Motors[channel].commanded == set) return true;

	Motors[channel].commanded = set * Motors[channel].inverted;

	return true;
}

/**
 * @brief Returns the normalized commanded speed of the motor
 *
 * @param channel
 *			The port of the motor [1,10]
 *
 * @returns Returns the commanded speed of the motor
 */
int MotorGet(int channel)
{
	if (channel > 10 || channel < 1)
		return 0;
	channel--;

	return Motors[channel].commanded * (Motors[channel].inverted ? 1 : -1);
}

/**
 * @brief Configures a motor port with inversion and skew
 *
 * @param channel
 *        The port of the motor [1,10]
 *
 * @param inverted
 *        If the motor port is inverted, then set to true (127 will become -127 and vice versa)
 *
 * @param skewPerMsec
 *        The acceleration of the motor in dPWM/millisecond. DEFAULT_SKEW is available, which sets dPWM/millisecond to 0.5
 *
 * Example usage:
 * @code
 *		void MechanismConfigure()
 *		{
 *			MotorConfigure(1, false, DEFAULT_SKEW);
 *			// Any related PID Controllers would go in here too, see the example for libsml/SingleThreadPIDController.c:PIDControllerCreate()
 *		}
 * @endcode
 */
void MotorConfigure(int channel, bool inverted, double skewPerMsec)
{
	if (channel < 0 || channel > 11)
		return;

	channel--;

	Motors[channel].channel = channel + 1;
	Motors[channel].inverted = inverted ? 1 : -1;
	Motors[channel].skewPerMsec = skewPerMsec;
	Motors[channel].RecalculateCommanded = &DefaultRecalculate;
}

/**
 * @brief Sets the recalculate commanded to the provided function pointer.
 *        Raw input values will be recalculated using the function.
 *        Example usage: convert raw speed to tune to a speed of an encoder (i.e. consistent speed)
 *
 * @param channel
 *        The port of the motor [1,10]
 *
 * @param func
 *        A pointer to a function taking an integer input (the raw input) and returning the corrected output as an int
 *
 * Example usage:
 * @code
 *		int RecalculateCommandedRequest(int in)
 *		{
 *			static int lastValue;
 *			static int lastTime;
 *			int out = in;
 *			if (in != 0)
 *			{
 *				int speed = (getSensorValue() - lastValue) / ((millis() - lastTime) * 0.001);
 *				if (speed != 0 && motorGet(1) != 0) 
 *				// This calculation will change the output to scale to the speed. For simplification, a PIDController was not used but is recommended
 *					out = (in * in * 40) / (127 * speed);
 *				else out = in;
 *			}
 *			else out = 0;
 *			lastValue = LiftGetCalibratedPotentiometerLeft();
 *			lastTime = millis();
 *			return out;
 *		}
 *		void MechanismConfigure()
 *		{
 *			MotorConfigure(1, false, DEFAULT_SKEW);
 *			MotorChangeRecalculateCommanded(1, &RecalculateCommandedRequest);
 *			// Any related PID Controllers would go in here too, see the example for libsml/SingleThreadPIDController.c:PIDControllerCreate()
 *		}
 * @endcode
 */
void MotorChangeRecalculateCommanded(int channel, int(*func)(int))
{
	if (channel < 0 || channel > 11)
		return;

	channel--;

	Motors[channel].RecalculateCommanded = func;
}