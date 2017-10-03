#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

#include <stdint.h>

extern uint32_t RandNum_seed;
extern uint32_t RandNum_start;
extern uint32_t RandNum_end;



/**
 * set up parameter for the build-in random function
 *
 * @param new_seed seed for the random function
 * @param start_value the minimum value the generator might output
 * @param end_value the maxmum value the generator might output
 */
void RandNum_set_parameter(int new_seed, int start_value, int end_value);



/**
 *
 * @return uniform distributed random interger within the predefined range
 */
uint32_t RandNum_get_random(void);



#endif //COEN383_P2_RANDOMNUMBERGENERATOR_H
