#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

extern int RandNum_seed;
extern int RandNum_start;
extern int RandNum_end;


void RandNum_setParameter(int newSeed, int startValue, int endValue);
int RandNum_getRandom(void);



#endif //COEN383_P2_RANDOMNUMBERGENERATOR_H
