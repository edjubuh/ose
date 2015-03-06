/**
 * @file liblcd/LCDFunctions.c
 * @author Elliot Berman
 *
 * @htmlonly
 * @copyright Copyright (c) 2014-2015 Olympic Steel Eagles. All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See ReadMe.md (Main Page) for additional notice.
 * @endhtmlonly
 ********************************************************************************/

#include "lcd\LCDFunctions.h"

#include "main.h"
#include <stdarg.h>
#include "lcd/vexprintf.h"

static Mutex mutex_line1, mutex_line2;

/**
 * @brief Returns the length of a string. Same functionality as strlen in <cstring>
 *        Code duplicated from http://fxr.watson.org/fxr/source/libkern/strlen.c?v=DFBSD
 *
 * @param str
 *			The string to size
 *
 * @returns Returns the length of the string
 */
static size_t strlen(const char *str)
{
	const char *s;
	for (s = str; *s; ++s);
	return (s - str);
}

/**
 * @brief Subtracts a number from an unsigned long, not allowing to to flip over, a - b
 *
 * @param a
 *			A pointer to an unsigned long
 *
 * @param b
 *			The number to subtract
 */
static void subtract(unsigned long *a, int b)
{
	if (*a > b) *a -= b;
	else *a = 0;
}

/**
 * @brief Intializes the LCD screen at uart1, turns on backlight, and creates mutexes
 */
void lcdInitialize()
{
	lcdShutdown(uart1);
	lcdInit(uart1);
	lcdSetBacklight(uart1, true);
	delay(10);
	mutex_line1 = mutexCreate();
	mutex_line2 = mutexCreate();
}

/**
 * @brief Prints a string on the LCD Screen. If the length of the string is greater than 16 (the max number of character spaces), the text will scroll across the screen.
 *
 * @param justification
 *        A text justification strategy to use to print if the number of characters is less than 16
 *        Left, Centered, and Right
 *        If the number of characters in the string is greater than 16, the justification is used as a scrolling strategy (from LEFT->right or RIGHT->left) with default to LEFT->right
 *
 * @param line
 *        The line to write the text to [1,2]
 *
 * @param duration
 *		Minimum duration of display in milliseconds
 *
 * @param string
 *        An array of characters to write to the LCD Screen
 *
 * @returns Returns true if lcdprintf() was successful
 */
bool lcdprint_d(textJustifications justification, unsigned char line, unsigned long duration, char * string)
{
	if (mutex_line1 == NULL || mutex_line2 == NULL)
		lcdInitialize();

	// If we're unable to take the mutex of the given line or the queried line is not valid, return false b/c unable to print
	if ((line == 1 && !mutexTake(mutex_line1, 2000)) ||
		(line == 2 && !mutexTake(mutex_line2, 2000)) ||
		(line != 1 && line != 2))
		return false;

	if (strlen(string) > 16)
	{
		char out[16];
		switch (justification)
		{
			case Right:
				// Fill text from right to left of what we can (last 16 characters)
				for (int i = 0, j = strlen(string) - 16; i < 16; i++, j++)
					out[i] = string[j];
				lcdPrint(uart1, line, out);
				delay(450);
				subtract(&duration, 450);
				for (int i = strlen(string) - 16; i >= 0; i--)
				{
					// Shift all characters on screen to the right by one
					for (int j = 15; j > 0; j--)
						out[j - 1] = out[j];
					// Fill last character on screen with new character
					out[15] = string[i];
					lcdPrint(uart1, line, out);
					delay(175);
					subtract(&duration, 175);
				}
				break;
			default:
				// Fill text from left to right of what we can (first 16 characters)
				for (int i = 0; i < 16; i++)
					out[i] = string[i];
				lcdPrint(uart1, line, out);
				delay(450);
				subtract(&duration, 450);
				// Iterate through each of the rest of the characters
				for (int i = 16; i <= strlen(string); i++)
				{
					// Shift all characters on screen left by one
					for (int j = 0; j < 15; j++)
						out[j] = out[j + 1];
					// Fill last character on screen with new character
					out[15] = string[i];
					lcdPrint(uart1, line, out);
					subtract(&duration, 175);
				}

				break;
		}
	}
	else
	{ // Less than 16 characters, use text justification strategy to align text on screen
		char out[16];
		for (int i = 0; i < 16; i++)
			out[i] = ' ';
		switch (justification)
		{
			case Left: // Left justification, nothing to do but print
				lcdPrint(uart1, line, string);
				break;
			case Centered: // Center justification
				for (int i = (16 - (strlen(string) % 16)) / 2, j = 0; j < strlen(string); i++, j++)
					out[i] = string[j];

				lcdPrint(uart1, line, out);
				break;
			case Right: // Right justification
				for (int i = 16 - strlen(string), j = 0; i < 16; i++, j++)
					out[i] = string[j];
				lcdPrint(uart1, line, out);
				break;
			default:
				lcdPrint(uart1, line, string);
				break;
		}
	}

	delay(duration);

	if (line == 1)
		mutexGive(mutex_line1);
	else if (line == 2)
		mutexGive(mutex_line2);
	return true;
}

/**
 * @brief Prints a string on the LCD Screen. If the length of the string is greater than 16 (the max number of character spaces), the text will scroll across the screen.
 *
 * @param justification
 *        A text justification strategy to use to print if the number of characters is less than 16
 *        Left, Centered, and Right
 *        If the number of characters in the string is greater than 16, the justification is used as a scrolling strategy (from LEFT->right or RIGHT->left) with default to LEFT->right
 *
 * @param line
 *        The line to write the text to [1,2]
 *
 * @param stringFormat
 *        An array of characters to write to the LCD Screen
 *
 * @param duration
 *		Minimum duration of display in milliseconds
 *
 * @param args
 *			A list of optional arguments for the string format
 *
 * @returns Returns true if lcdprintv() was successful
 */
bool lcdprint_dv(textJustifications justification, unsigned char line, unsigned long duration, char * stringFormat, va_list args)
{
	char string[128];
	vex_vsprintf(string, stringFormat, args);
	return lcdprint_d(justification, line, duration, string);
}

/**
 * @brief Prints a string on the LCD Screen. If the length of the string is greater than 16 (the max number of character spaces), the text will scroll across the screen. 
 *
 * @param justification
 *        A text justification strategy to use to print if the number of characters is less than 16
 *        Left, Centered, and Right
 *        If the number of characters in the string is greater than 16, the justification is used as a scrolling strategy (from LEFT->right or RIGHT->left) with default to LEFT->right
 *
 * @param line
 *        The line to write the text to [1,2]
 *
 * @param duration
 *		Minimum duration of display in milliseconds
 *
 * @param stringFormat
 *         An array of characters to write to the LCD Screen
 *
 * @param ...
 *			A list of optional arguments for the string format
 *
 * @returns Returns true if lcdprintf() was successful
 */
bool lcdprint_df(textJustifications justification, unsigned char line, unsigned long duration, char * stringFormat, ...)
{
	char string[128];
	va_list args;
	va_start(args, stringFormat);
	vex_vsprintf(string, stringFormat, args);
	va_end(args);

	return lcdprint_d(justification, line, duration, string);
}

/**
 * @brief Prints a string on the LCD Screen. If the length of the string is greater than 16 (the max number of character spaces), the text will scroll across the screen.
 *
 * @param justification 
 *        A text justification strategy to use to print if the number of characters is less than 16
 *        Left, Centered, and Right
 *        If the number of characters in the string is greater than 16, the justification is used as a scrolling strategy (from LEFT->right or RIGHT->left) with default to LEFT->right
 *
 * @param line
 *        The line to write the text to [1,2]
 *
 * @param string
 *        An array of characters to write to the LCD Screen
 *
 * @returns Returns true if lcdprintf() was successful
 *
 * @warning Text will take longer to display if longer than 16 characters (LCD screen limitation) in order to cycle through all characters. This may cause undesired delays in program execution
 */
bool lcdprint(textJustifications justification, unsigned char line, char * string)
{
	return lcdprint_d(justification, line, 0, string);
}

/**
 * @brief Prints a string on the LCD Screen. If the length of the string is greater than 16 (the max number of character spaces), the text will scroll across the screen.

 * @param justification
 *        A text justification strategy to use to print if the number of characters is less than 16
 *        Left, Centered, and Right
 *        If the number of characters in the string is greater than 16, the justification is used as a scrolling strategy (from LEFT->right or RIGHT->left) with default to LEFT->right
 *
 * @param line
 *        The line to write the text to [1,2]
 *
 * @param stringFormat
 *        An array of characters to write to the LCD Screen
 *
 * @param args
 *			A list of optional arguments for the string format
 *
 * @returns Returns true if lcdprintv() was successful

 * @warning Text will take longer to display if longer than 16 characters (LCD screen limitation) in order to cycle through all characters. This may cause undesired delays in program execution
 */
bool lcdprintv(textJustifications justification, unsigned char line, char * stringFormat, va_list args)
{
	char string[128];
	vex_vsprintf(string, stringFormat, args);
	return lcdprint(justification, line, string);
}

/**
 * @brief Prints a string on the LCD Screen. If the length of the string is greater than 16 (the max number of character spaces), the text will scroll across the screen.
 *
 * @param justification
 *        A text justification strategy to use to print if the number of characters is less than 16
 *        Left, Centered, and Right
 *        If the number of characters in the string is greater than 16, the justification is used as a scrolling strategy (from LEFT->right or RIGHT->left) with default to LEFT->right
 *
 * @param line
 *        The line to write the text to [1,2]
 *
 * @param stringFormat
 *        An array of characters to write to the LCD Screen
 *
 * @param ...
 *			A list of optional arguments for the string format
 *
 * @returns Returns true if lcdprintf() was successful
 *
 * @warning Text will take longer to display if longer than 16 characters (LCD screen limitation) in order to cycle through all characters. This may cause undesired delays in program execution
 */
bool lcdprintf(textJustifications justification, unsigned char line, char * stringFormat, ...)
{
	char string[128];
	va_list args;
	va_start(args, stringFormat);
	vex_vsprintf(string, stringFormat, args);
	va_end(args);
	
	return lcdprint(justification, line, string);
}