/**
 * @file liblcd/LCDManager.c
 * @author Elliot Berman
 * @brief Source for the LCD Manager. LCD Manager is a background task that cycles
 *	      through user defined text displays when not being overriden by another
 *        display.
 *
 * @todo Work out bugs in LCDManager. LCD Manager is not ready for prime production 
 *		 code
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#include "main.h"
#include "lcd/LCDManager.h"
#include "lcd/LCDFunctions.h"

#define NUM_CYCLE_TEXT_DISPLAYS 8

/**
 * @brief Array of cycle text structs for usage on line 1.
 */
static DisplayText cycleText_l1[NUM_CYCLE_TEXT_DISPLAYS];

/**
* @brief Array of cycle text structs for usage on line 2.
*/
static DisplayText cycleText_l2[NUM_CYCLE_TEXT_DISPLAYS];
static Mutex m_l1, m_l2, m_cT;
static TaskHandle handle;

static void lcdManagerTask()
{
	while (true)
	{
		for (int i = 0; i < NUM_CYCLE_TEXT_DISPLAYS; i++)
		{
			lcdprintf(Centered, 2, "%d / %d", i, NUM_CYCLE_TEXT_DISPLAYS);
			//if(!mutexTake(m_cT, 500)) continue;
			/*
			if (cycleText_l1[i].GetText != NULL)
				lcdprintf((cycleText_l1[i].GetText)(), cycleText_l1[i].justification, 1);
			if (cycleText_l2[i].GetText != NULL)
				lcdprintf((cycleText_l2[i].GetText)(), cycleText_l2[i].justification, 2);
				*/
			//mutexGive(m_cT);
			delay(100);
		}
		delay(30);
	}
}

/**
 * @brief Initializes the LCD Manager and creates the LCDManager Task. Invokes lcdInitialze()
 */
void initLCDManager()
{
	lcdInitialize();
	m_l1 = mutexCreate();
	m_l2 = mutexCreate();
	m_cT = mutexCreate();
	handle = taskCreate(&lcdManagerTask, 85, NULL, 2);
}

/**
 * @brief Adds a cycle text struct to the line in the next available position
 *
 * @param text
 *		  Struct to add to LCD Manager
 *
 * @param line
 *		  The LCD line number to display on
 */
void addCycleText(DisplayText text, int line)
{
	if (line < 0 || line > 2) return;
	int i = 0;
	if (!mutexTake(m_cT, 1000)) return;
	switch (line)
	{
		case 1:
			for (; cycleText_l1[i].GetText != NULL; i++);
			if (i > (NUM_CYCLE_TEXT_DISPLAYS - 1)) i = (NUM_CYCLE_TEXT_DISPLAYS - 1);
			cycleText_l1[i] = text;
			break;
		case 2:
			for (; cycleText_l2[i].GetText != NULL; i++);
			if (i > (NUM_CYCLE_TEXT_DISPLAYS - 1)) i = (NUM_CYCLE_TEXT_DISPLAYS - 1);
			cycleText_l2[i] = text;
			break;
		default:
			break;
	}
	mutexGive(m_cT);
}

/**
 * @brief Places a DisplayText struct at the given position, overwriting anything that was
 *        previously on that location
 *
 * @param text
 *		  Struct to add to LCD Manager
 *
 * @param line
 *		  The LCD line number to display on
 *
 * @param pos
 *		  The position in the array to place it in (will safely return if invalid position)
 */
void replaceCycleText(DisplayText text, int line, int pos)
{
	if (pos < 0 || pos >= NUM_CYCLE_TEXT_DISPLAYS) return;
	if (line < 0 || line > 2) return;
	if (!mutexTake(m_cT, 1000)) return;
	switch (line)
	{
		case 1:
			cycleText_l1[pos] = text;
			break;
		case 2:
			cycleText_l2[pos] = text;
			break;
		default:
			break;
	}
	mutexGive(m_cT);
}