/**
 * @file libsml/recorder.c
 *
 * @brief Records data about motor speeds, sensor data, etc. for analysis.
 */

#include "main.h"
//#include "stdlib.h" //necessary for file r/w; may already be included in main.h
#include "sml/recorder.h"

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