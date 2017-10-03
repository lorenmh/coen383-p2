#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>


typedef struct {
    uint32_t id;
    uint32_t arrival_time;
    uint32_t service_time;
    uint32_t priority;

    uint32_t remaining_time;
    uint32_t last_execution_time;
} process_t;

struct process_queue_node_t {
    process_t process;
    struct process_queue_node_t *next;
};


typedef struct {
    struct process_queue_node_t *entry;

} process_queue_t;


/**
 * create a process queue and soted it by arrical time
 *
 * @param size total number of process inside the queue
 * @return pointer to the queue
 */
process_queue_t *create_process_queue(int size);


/**
 * free a allocated process queue
 *
 * @param process_queue pointer to the queue need to be free
*/
void free_process_queue(process_queue_t *process_queue);

#endif //COEN383_P2_PROCESS_H
