#include "main.h"

#include "dios/encoder.h"

/**
 * @brief Gives an array that contains ime tick counts.
 *
 * @param *clicks
 *        A pointer to an array of length 10 to store the tick count values in.
 *
 * @note Obtained by using the & operator.
 * @example GetAllEncoders( &tickCount );
 */
void GetAllEncoders(int *clicks[10])//not sure if syntax is correct.
{
	int encoderPorts[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; //all 10
	int clickValues[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //array to store click values.

	int i = 0; //incrementor
	int temp = 0; //temporary value for storing ime click numbers.

	for (i = 0; i < 10; i++)
	{
		//Gets encoder values, otherwise sets clickValues to zero.
		clickValues[i] = imeGet(encoderPorts[i], &temp) ? temp : 0;
	}
	clicks = &clickValues; //passes location of clickValues to clicks.
}