/**
 * @file dummy/init.c		
 * @brief Source file for initialize functions.
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#include "main.h"
#include "lcd/LCDManager.h"
#include "lcd/LCDFunctions.h"


char * VulcanText()
{
	return "VULCAN";
}

static char * battext;
char * BatteryText()
{
	free(battext);
	battext = (char*)malloc(16*sizeof(char));
	snprintf(battext, 16, "M:%1.2fV", (double)powerLevelMain()/1000.0);
	if (battext == NULL) return "";
	return battext;
}

char * OSEText()
{
	return "OSE | 7701";
}

char * ModeText()
{
	if (isEnabled())
	{
		if (isAutonomous())
			return "Autonomous";
		else
			return "Teleop";
	}
	else
		return "Disabled";
}

void initializeIO() {
}

void initialize() {
	lcdInitialize();
	initLCDManager();
	DisplayText text1 = { &VulcanText, Centered };
	DisplayText text2 = { &BatteryText, Centered };
	DisplayText text3 = { &OSEText, Centered };
	DisplayText text4 = { &ModeText, Centered };
	addCycleText(text1, 1);
	addCycleText(text2, 1);
	addCycleText(text3, 1);
	addCycleText(text4, 1);
	//lcdprintf(Centered, 1, "%s", Text2());
}
