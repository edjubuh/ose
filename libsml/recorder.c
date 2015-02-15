/**
 * @file libsml/recorder.c
 *
 * @brief Records data about motor speeds, sensor data, etc. for analysis.
 */

#include "main.h"
//#include "stdlib.h" //necessary for file r/w; may already be included in main.h
#include "sml/recorder.h"
#include "vulcan/mechop.h"
#include "lcd/LCDFunctions.h"
#include <math.h>
#include "vulcan/Chassis.h"


/**
 * @brief Initializes and begins recording of the run of the robot.
 *
 * @param mode
 *        'a' for recording autonomous, 'e' for recording when robot is enabled.
 * @param time
 *        time in between recording intervals.
 *
 * @return 1 or 0, depends on failure or success of the program, respectively.
 */
int recorderInit(char mode, const unsigned long time)
{
	fdelete("trial.csv"); //delete the previous file and start with fresh slate

	FILE *data_stream = fopen("trial.csv", "w"); //points to file to "w"rite
	bool (*record_mode)() = NULL; //initialize to NULL

	if (mode == 'a')
		record_mode = &isAutonomous;
	else if (mode == 'e')
		record_mode = &isEnabled;
	else
	{
		lcdSetText(uart1, 1, "Recorder Failed ");
		lcdSetText(uart1, 2, "to Initialize!  ");
		return EXIT_FAILURE; //constant in stdlib: equivalent to program failure (1)
	}
	
	//variable necessary for recording and writing
	unsigned long curr_time = 0;
	int imeVelocity[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int imeCount[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i = 0;
	
	fprintf( data_stream, "curr_time,imeCount[0],imeVelocity[0],imeCount[1],imeVelocity[1],imeCount[2],imeVelocity[2],imeCount[3],imeVelocity[3],imeCount[4],imeVelocity[4],imeCount[5],imeVelocity[5],imeCount[6],imeVelocity[6],imeCount[7],imeVelocity[7],imeCount[8],imeVelocity[8],imeCount[9],imeVelocity[9]\n");

	while (*record_mode) //usage of pointers will retrieve the right input
	{
		for (i = 0; i < 10; i++)
		{
			imeGetVelocity( i, &imeVelocity[i]);
			imeGet( i, &imeCount[i]);
		}
		fprintf( data_stream, "%lu,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",curr_time,imeCount[0],imeVelocity[0],imeCount[1],imeVelocity[1],imeCount[2],imeVelocity[2],imeCount[3],imeVelocity[3],imeCount[4],imeVelocity[4],imeCount[5],imeVelocity[5],imeCount[6],imeVelocity[6],imeCount[7],imeVelocity[7],imeCount[8],imeVelocity[8],imeCount[9],imeVelocity[9]);
		delay(time);
		curr_time += time;
	}
	fclose( data_stream);
	return EXIT_SUCCESS; //program success (0)
}

//unsigned long start;
#define CURRENT_T millis() - start
#define MOTOROPTION false // used for the function below

int recorderUser(int l3, int l4, int r1, int r2)
{
	FILE *trial_cvs = fopen("trial.cvs", "a");

	int left = thetaSector(getJoyTheta(l4, l3)), //left joystick
		right = thetaSector(getJoyTheta(r1, r2)); // right joystick

	lcdprintf(Left, 2, "L:%dR:%d", left, right);

	if (abs(l3) < THRESHOLD && abs(l4) < THRESHOLD && abs(r1) < THRESHOLD && abs(r2) < THRESHOLD)
	{
		ChassisSet(0, 0, MOTOROPTION);
		fprintf(trial_cvs, "0,0,%lu,Threshold\n", CURRENT_T);
		return EXIT_FAILURE;
	}

	//Up / Down
	if ((abs(left)  == 3 || abs(left)  == 4) &&
		(abs(right) == 3 || abs(right) == 4))
	{
		ChassisSet(l3, r2, MOTOROPTION);
		fprintf(trial_cvs, "%d,%d,%lu,Up-Down\n", l3, r2, CURRENT_T);
		lcdprint(Centered, 1, "tank");
	}
	//Left / Right
	else if ((abs(left) == 0) &&
		(abs(right) == 0))
	{
		int p = aJoy(l4, r1);
		ChassisSetMecanum(M_PI_2,
			p,
			0, MOTOROPTION);
		fprintf(trial_cvs, "M_PI_2,%d,0,%lu,Right\n", p, CURRENT_T);
		lcdprint(Centered, 1, "strafe right");
	}
	else if ((abs(left)  == 7) &&
		(abs(right) == 7))
	{
		int p = aJoy(l4, r1);
		ChassisSetMecanum(-M_PI_2,
			p,
			0, MOTOROPTION);
		fprintf(trial_cvs, "-M_PI_2,%d,0,%lu,Left\n", p, CURRENT_T);
		lcdprint(Centered, 1, "strafe left");
	}


	//northeast / northwest
	else if ((left  == 2 || left  == 1) &&
		(right == 2 || right == 1))
	{
		int h = aHypo(cHypo(l4, l3), cHypo(r1, r2));
		ChassisSetMecanum(M_PI_4,
			h,
			0, MOTOROPTION);
		fprintf(trial_cvs, "M_PI_4,%d,0,%lu,Northeast\n", h, CURRENT_T);
		lcdprintf(Centered, 1, "northeast%d", h);
	}
	else if ((left  == 5 || left  == 6) &&
		(right == 5 || right == 6))
	{
		int p = aHypo(cHypo(l4, l3), cHypo(r1, r2));
		ChassisSetMecanum(-M_PI_4,
			p,
			0, MOTOROPTION);
		fprintf(trial_cvs, "-M_PI_4,%d,0,%lu,Northwest\n", p, CURRENT_T);
		lcdprint(Centered, 1, "northwest");
	}


	//southeast / southwest
	else if ((left  == -2 || left  == -1) &&
		(right == -2 || right == -1))
	{
		int p = aHypo(cHypo(l4, l3), cHypo(r1, r2));
		ChassisSetMecanum(-3.0 * M_PI_4,
			p,
			0, MOTOROPTION);
		fprintf(trial_cvs, "-3.0*M_PI_4,%d,0,%lu,Southeast\n", p, CURRENT_T);
		lcdprint(Centered, 1, "southeast");
	}
	else if ((left  == -5 || left  == -6) &&
		(right == -5 || right == -6))
	{
		int p = aHypo(cHypo(l4, l3), cHypo(r1, r2));
		ChassisSetMecanum(3.0 * M_PI_4,
			p,
			0, MOTOROPTION);
		fprintf(trial_cvs, "3.0*M_PI_4,%d,0,%lu,Southwest\n", p, CURRENT_T);
		lcdprint(Centered, 1, "southwest");
	}

	/*
	//strafe right / left
	else if ( ( (left == 3 || left == 4) || (left == -3 || left == -4) ) &&
	(right == 0 || right == -0)                                    )
	ChassisSet(l3,
	(int)(l3 * (1.0 - abs( ((double)r1) / STRAFE_CONST ) ) ),
	MOTOROPTION);
	else if ( (left == 7 || left == -7) &&
	( (right == 3 || right == 4) || (right == -3 || right == -4) ) )
	ChassisSet( (int)(r2 * (1.0 - abs( ((double)l4) / STRAFE_CONST ) ) ),
	r2,
	MOTOROPTION);


	/// @TODO Strafe up / down
	
	else if ( (left == 0 || left == -0) &&
	(right == 3 || right = 4) || (right == -3 || right == -4) )
	mechanumDrive( l4, some equation with r1, false);
	else if ( ( left == 3 || left = 4) || (left == -3 || left == -4) &&
	( right == 7 || right == -7)                              )
	mechanumDrive( r1, some equation with l3, false);
	*/

	//By default: stop, so motors don't break
	else
	{
		ChassisSet(l3, r2, MOTOROPTION);
		fprintf(trial_cvs, "%d,%d,%lu,Default:Tank\n", l3, r2, CURRENT_T);
		lcdprint(Centered, 1, "default: tank");
	}

	fclose(trial_cvs); //for safety
	return EXIT_SUCCESS;
}