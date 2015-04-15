/**
 * @file include/sml/recorder.h
 * @brief Header file for recording capabilities <br>
 * See libsml/recorder.c for details of all functions
 *
 * @copyright Copyright(c) 2014-2015 Olympic Steel Eagles.All rights reserved. <br>
 * Portions of this file may contain elements from the PROS API. <br>
 * See include/main.h for additional notice.
 ********************************************************************************/

int recorderInit(char mode, const unsigned long time);
extern unsigned long start;
int recorderUser(int l3, int l4, int r1, int r2);