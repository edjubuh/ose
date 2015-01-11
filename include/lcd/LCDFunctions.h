/**
 * @file include/lcd/LCDFunctions.h
 * @brief Header file for primary LCD functions
 * See liblcd/liblcd.c for details of all functions
 *
 * Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/API.h for additional notice.
 ********************************************************************************/

#ifndef LCDFUNC_H_
#define LCDFUNC_H_

#include "main.h"

typedef enum
{
	Left,
	Right,
	Centered
} textJustifications;

void lcdInitialize();

bool printText(char *, textJustifications, unsigned char);

#endif