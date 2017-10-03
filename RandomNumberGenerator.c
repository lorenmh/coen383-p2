#include "RandomNumberGenerator.h"
#include <stdlib.h>

uint32_t RandNum_seed = 0;
uint32_t RandNum_start = 0;
uint32_t RandNum_end = RAND_MAX;

void RandNum_set_parameter(int new_seed, int start_value, int end_value) {
    RandNum_seed = (uint32_t)new_seed;
    RandNum_start = (uint32_t)start_value;
    RandNum_end = (uint32_t)end_value;
}


uint32_t RandNum_get_random(void) {
    uint32_t upperBound = RandNum_end - RandNum_start + 1;
    return arc4random_uniform(upperBound) + RandNum_start;
}