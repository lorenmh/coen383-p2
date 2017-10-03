#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define MAX_PRIORITY 4
#define MAX_ACCEPTABLE_ARRIVAL_TIME 99
#define MAX_SERVICE_TIME 20


typedef struct {
    uint32_t id;
    uint32_t arrival_time;
    uint32_t service_time;
    uint32_t priority;

    uint32_t remaining_time;
    uint32_t context_switch_time;
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

#endif //COEN383_P2_PROCESS_H
