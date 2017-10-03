#include "RandomNumberGenerator.h"
#include <stdlib.h>

uint32_t RandNum_seed = 0;
uint32_t RandNum_start = 0;
uint32_t RandNum_end = RAND_MAX;

void RandNum_set_parameter(int newSeed, int startValue, int endValue) {
    RandNum_seed = (uint32_t)newSeed;
    RandNum_start = (uint32_t)startValue;
    RandNum_end = (uint32_t)endValue;
}


uint32_t RandNum_get_random(void) {
    uint32_t upperBound = RandNum_end - RandNum_start + 1;
    return arc4random_uniform(upperBound) + RandNum_start;
}