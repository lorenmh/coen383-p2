#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MIN_PRIORITY 1
#define MAX_PRIORITY 4
#define MAX_ACCEPTABLE_ARRIVAL_TIME 99
#define MAX_SERVICE_TIME 20


typedef struct {
    uint32_t id;
    uint32_t arrival_time;
    uint32_t service_time;
    uint32_t priority;

    uint32_t remaining_time;  // for shortest remaining time first
    uint32_t context_switch_time;  // for aging


    // for result part
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
void print_process(process_t const *p);
void print_process_queue(process_queue_t const *pq);


/**
 * clone an existing process queue
 *
 * @param process_queue pointer to the original process queue
 * @return a pointer to the new process queue or NULL if input is NULL
 */
process_queue_t *clone_process_queue(process_queue_t *process_queue);


#endif //COEN383_P2_PROCESS_H
