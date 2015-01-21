/**
 * @file liblcd/LCDManager.c
 * @author Elliot Berman
 * @brief Source for the LCD Manager
 *
 * @copyright Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/main.h for additional notice.
 ********************************************************************************/

#include "main.h"
#include "lcd/LCDManager.h"
#include "lcd/LCDFunctions.h"

#define NUM_CYCLE_TEXT_DISPLAYS 8

static DisplayText cycleText_l1[NUM_CYCLE_TEXT_DISPLAYS];
static DisplayText cycleText_l2[NUM_CYCLE_TEXT_DISPLAYS];
static Mutex m_cT;
static TaskHandle handle;

static int i = 0, j = 0;
static unsigned long lastCycle = 0;

void lcdManagerTask()
{
	while (true)
	{
		
		if ((millis() - lastCycle) >= 1750)
		{
			lastCycle = millis();
			i++;
			j++;
		}

		if (i >= NUM_CYCLE_TEXT_DISPLAYS)
			i = 0;

		if (j >= NUM_CYCLE_TEXT_DISPLAYS)
			j = 0;

		mutexTake(m_cT, 500);
		if ((cycleText_l1[i]).GetText)
			lcdprintf(Centered, 1, "%s", (cycleText_l1[i]).GetText());
		else i++;

		//lcdprintf(Centered, 2, "%d", i);
		
		if ((cycleText_l2[j]).GetText)
			lcdprintf(Centered, 2, "%s", (cycleText_l2[2]).GetText());
		else j++;
		
		mutexGive(m_cT);
		delay(40);
	}
}

void initLCDManager()
{
	m_cT = mutexCreate();
	delay(100);
	handle = taskCreate(&lcdManagerTask, TASK_DEFAULT_STACK_SIZE, NULL, 0);
}

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
			if (i > (NUM_CYCLE_TEXT_DISPLAYS - 1)) 
				i = (NUM_CYCLE_TEXT_DISPLAYS - 1);
			cycleText_l2[i] = text;
			break;
		default:
			break;
	}
	mutexGive(m_cT);
}

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