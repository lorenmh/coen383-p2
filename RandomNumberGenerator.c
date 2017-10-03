#include "RandomNumberGenerator.h"
#include <stdlib.h>

int RandNum_seed = 0;
int RandNum_start = 0;
int RandNum_end = RAND_MAX;

void RandNum_setParameter(int newSeed, int startValue, int endValue) {
    RandNum_seed = newSeed;
    RandNum_start = startValue;
    RandNum_end = endValue;
}


int RandNum_getRandom(void) {
    int upperBound = RandNum_end - RandNum_start + 1;
    return arc4random_uniform(upperBound) + RandNum_start;
}