#include "main.h"

#include "dios/encoder.h"


/**
 * @brief Puts click values of ime motors into an array passed to the function.
 *
 * @param *clicks
 *        A pointer to an array to store the tick count values in.
 *        @note Obtained by using the & operator
 *        @example GetAllEncoders( &tickCount );
 */
void scanAllEncoders(int *clicks, unsigned int size)
{
	int i = 0; //incrementor
	int temp = 0; //temporary value for storing ime click numbers.

	for (i = 0; i < size; i++)
	{
		//Gets encoder values, otherwise sets clicks to zero.
		clicks[i] = imeGet(i, &temp) ? temp : 0;
	}
}