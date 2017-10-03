#include "Process.h"
#include <stdlib.h>
#include "RandomNumberGenerator.h"
#include <time.h>
#include <string.h>


int process_comparator(const void* p1, const void* p2) {
    int64_t p1_arrival = (int64_t)(((process_t*)p1)->arrival_time);
    int64_t p2_arrival = (int64_t)(((process_t*)p2)->arrival_time);
    return (int)(p1_arrival - p2_arrival);
}


process_queue_t *create_process_queue(int size) {
    process_t *newProcessArray = malloc(sizeof(process_t) * size);
    // id and arrival time
    RandNum_set_parameter(time(NULL), 0, MAX_ACCEPTABLE_ARRIVAL_TIME);
    for (int i = 0; i < size; ++i) {
        newProcessArray[i].id = i;
        newProcessArray[i].arrival_time = RandNum_get_random();
        newProcessArray[i].context_switch_time = 0;
    }

    // service time
    RandNum_set_parameter(time(NULL), 1, MAX_SERVICE_TIME);
    for (int i = 0; i < size; ++i) {
        newProcessArray[i].service_time = RandNum_get_random();
        newProcessArray[i].remaining_time = newProcessArray[i].service_time;
    }

    // priority
    RandNum_set_parameter(time(NULL), 0, MAX_PRIORITY);
    for (int i = 0; i < size; ++i) {
        newProcessArray[i].priority = RandNum_get_random();
    }

    qsort(newProcessArray, (size_t)size, sizeof(process_t), process_comparator);
    process_queue_t *new_queue = malloc(sizeof(process_queue_t));
    new_queue->entry = newProcessArray;
    new_queue->size = size;
    return new_queue;

}

void free_process_queue(process_queue_t *process_queue) {
    free(process_queue->entry);
}

process_queue_t *clone_process_queue(process_queue_t *process_queue) {
    if (process_queue == NULL) {
        return NULL;
    }
    process_queue_t *new = malloc(sizeof(process_queue_t));
    size_t size = process_queue->size;
    new->size = size;
    new->entry = malloc(sizeof(process_t) * size);
    memcpy(new->entry, process_queue->entry, sizeof(process_t) * size);
    return new;
}
