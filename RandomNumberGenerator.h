#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

#include <stdint.h>

extern uint32_t RandNum_seed;
extern uint32_t RandNum_start;
extern uint32_t RandNum_end;


void RandNum_set_parameter(int newSeed, int startValue, int endValue);
uint32_t RandNum_get_random(void);



#endif //COEN383_P2_RANDOMNUMBERGENERATOR_H
