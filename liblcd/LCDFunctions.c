/************************************************************************/
/* @file LCDFunctions.c			@brief Source file LCD functions.		*/
/*														                */
/* Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved.	*/
/* Portions of this file may contain elements from the PROS API.		*/
/* See include/API.h for additional notice.								*/
/************************************************************************/

#include "main.h"
#include "lcd\LCDFunctions.h"

static Mutex mutex_line1, mutex_line2;
static char * sleep_liine1, sleep_line2;

/**
 * @brief Returns the length of a string. Same functionality as strlen in <cstring>
 *        Code duplicated from http://fxr.watson.org/fxr/source/libkern/strlen.c?v=DFBSD
 */
static size_t strlen(const char *str)
{
	const char *s;
	for (s = str; *s; ++s);
	return (s - str);
}

/**
 * @brief Intializes the LCD screen at uart1, turns on backlight, and creates mutexes
 */
void lcdInitialize()
{
	lcdInit(uart1);
	lcdSetBacklight(uart1, true);
	mutex_line1 = mutexCreate();
	mutex_line2 = mutexCreate();
}

/**
 * @brief Prints a string on the LCD Screen. If the length of the string is greater than 16 (the max number of character spaces), the text will scroll across the screen.
 *
 * @param string
 *        An array of characters to write to the LCD Screen
 *
 * @param justification
 *        A text justification strategy to use to print if the number of characters is less than 16
 *        Left, Centered, and Right
 *        If the number of characters in the string is greater than 16, the justification is used as a scrolling strategy (from LEFT->right or RIGHT->left) with default to LEFT->right
 *
 * @param line
 *        The line to write the text to [1,2]
 */
bool printText(char * string, textJustifications justification, unsigned char line)
{
	if (mutex_line1 == NULL || mutex_line2 == NULL)
	{
		lcdInit(uart1);
		lcdSetBacklight(uart1, true);
		mutex_line1 = mutexCreate();
		mutex_line2 = mutexCreate();
	}
	if (line == 1)
	{
		if (!mutexTake(mutex_line1, timeout))
			return false;
	}
	else if (line == 2)
	{
		if (!mutexTake(mutex_line2, timeout))
			return false;
	}
	else return false;

	if (strlen(string) > 16)
	{
		char out[16];
		switch (justification)
		{
			case Right:
				for (int i = 0, j = strlen(string) - 16; i < 16; i++, j++)
					out[i] = string[j];
				lcdPrint(uart1, line, out);
				delay(450);
				for (int i = strlen(string) - 2; i >= 0; i--)
				{
					for (int j = 15; j > 0; j--)
						out[j - 1] = out[j];

					out[15] = string[i];
					lcdPrint(uart1, line, out);
					delay(175);
				}
				break;
			default:
				for (int i = 0; i < 16; i++)
					out[i] = string[i];
				lcdPrint(uart1, line, out);
				delay(450);
				for (int i = 16; i <= strlen(string); i++)
				{
					for (int j = 0; j < 15; j++)
						out[j] = out[j + 1];

					out[15] = string[i];
					lcdPrint(uart1, line, out);
					delay(175);
				}
				break;
		delay(450);
	}
	else
	{
		char out[16];
		for (int i = 0; i < 16; i++)
			out[i] = ' ';
		switch (justification)
		{
			case Left:;
				lcdPrint(uart1, line, string);
				break;
			case Centered:
				for (int i = (16 - (strlen(string) % 16)) / 2, j = 0; j < strlen(string); i++, j++)
					out[i] = string[j];

				lcdPrint(uart1, line, out);
				break;
			case Right:
				for (int i = 16 - strlen(string), j = 0; i < 16; i++, j++)
					out[i] = string[j];
				lcdPrint(uart1, line, out);
				break;
			default:
				lcdPrint(uart1, line, string);
				break;
		}

		delay(550);
	}

	if (line == 1)
		mutexGive(mutex_line1);
	else if (line == 2)
		mutexGive(mutex_line2);

	return true;
}

static LCDSleeper(void *none)
{
	while (true)
	{
		
	}
}