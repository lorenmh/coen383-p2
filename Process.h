#ifndef PROCESS_H
#define PROCESS_H

typedef struct __Process {
    int arravalTime;
    int serviceTime;
    int priority;

    int remainingTime;
    int lastExecutionTime;
};


#endif //COEN383_P2_PROCESS_H
