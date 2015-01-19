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

void initLCDManager()
{
	lcdInitialize();
	m_l1 = mutexCreate();
	m_l2 = mutexCreate();
	m_cT = mutexCreate();
	handle = taskCreate(&lcdManagerTask, 85, NULL, 2);
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
			if (i > (NUM_CYCLE_TEXT_DISPLAYS - 1)) i = (NUM_CYCLE_TEXT_DISPLAYS - 1);
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