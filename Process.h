#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MIN_PRIORITY 1
#define MAX_PRIORITY 4
#define MAX_ACCEPTABLE_ARRIVAL_TIME 99
#define MAX_RUN_TIME 20

//#define INIT_SEED (3143211)
#define INIT_SEED (314)

typedef struct {
    char id;
    uint32_t arrival_time;
    uint32_t expected_run_time;
    uint32_t remaining_run_time;

    uint8_t initial_priority; // for debug
    uint8_t priority;
    uint8_t has_already_run;
    uint16_t age;

    // for results
    uint32_t turnaround_time;
    uint32_t response_time;
} process_t;


typedef struct {
    process_t *entry;
    uint32_t size;
} process_queue_t;


/**
 * create a process queue and sorted it by arrival time
 *
 * @param size total number of process inside the queue
 * @return pointer to the queue structure
 */
process_queue_t *create_process_queue(int size);


/**
 * free a allocated process queue
 *
 * @param process_queue pointer to the queue need to be free
*/
void free_process_queue(process_queue_t *process_queue);


/**
 * print information about a process
 *
 * @param process pointer to a process to print
*/
void print_process(process_t const *process);


/**
 * print all processes in a process queue
 *
 * @param process_queue pointer to the queue to be printed
*/
void print_process_queue(process_queue_t const *process_queue);


/**
 * clone an existing process queue
 *
 * @param process_queue pointer to the original process queue
 * @return a pointer to the new process queue or NULL if input is NULL
 */
process_queue_t *clone_process_queue(process_queue_t *process_queue);


#endif //COEN383_P2_PROCESS_H
