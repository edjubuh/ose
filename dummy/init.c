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


char * Text1()
{
	return "Hello World!";
}

char * Text2()
{
	char * string;
	snprintf(string, 16, "V: %1.4f", powerLevelMain() / 1000.0);
	return string;
}

void initializeIO() {
}

void initialize() {
	//lcdInitialize();
	initLCDManager();
	DisplayText text1 = { &Text1, Centered };
	DisplayText text2 = { &Text2, Centered };
	//addCycleText(text1, 1);
	//addCycleText(text2, 1);
	//lcdprintf("Hello people from all the different worlds", Centered, 1);
}
