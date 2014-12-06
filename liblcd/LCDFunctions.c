/************************************************************************/
/* @file LCDFunctions.c			@brief Source file LCD functions.		*/
/*														                */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#include "main.h"
#include "lcd\LCDFunctions.h"

static FILE* uartPort = uart1;

void lcdInitialize()
{
	lcdInit(uartPort);
	
}