#include "Process.h"
#include <stdlib.h>
#include <stdio.h>
#include "RandomNumberGenerator.h"
#include <time.h>
#include <string.h>

char const *PIDS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int process_comparator(const void* p1, const void* p2) {
    int64_t p1_arrival = (int64_t)(((process_t*)p1)->arrival_time);
    int64_t p2_arrival = (int64_t)(((process_t*)p2)->arrival_time);
    return (int)(p1_arrival - p2_arrival);
}


void print_process(process_t const *process) {
    printf("{id: %c, arrival_time: %2d, priority: %d, expected_run_time: %2d}",
        process->id,
        process->arrival_time,
        process->priority,
        process->expected_run_time
    );
}

void print_process_queue(process_queue_t const *process_queue) {
    printf("[\n");
    for (int i = 0; i < process_queue->size; i++) {
        printf("\t");
        print_process(&process_queue->entry[i]);
        printf("\n");
    }
    printf("]\n");
}

process_queue_t *create_process_queue(int size) {
    process_t *newProcessArray = malloc(sizeof(process_t) * size);

    int seed_for_arrival = rand();
    int seed_for_run_time = rand();
    int seed_for_priority = rand();

    // id and arrival time, other non-random values
    RandNum_set_parameter(seed_for_arrival, 0, MAX_ACCEPTABLE_ARRIVAL_TIME);
    for (uint32_t i = 0; i < size; ++i) {
        newProcessArray[i].id = PIDS[i]; // will error if size > 52
        newProcessArray[i].arrival_time = RandNum_get_random();

        newProcessArray[i].has_already_run = 0;
        newProcessArray[i].turnaround_time = INT32_MAX;
        newProcessArray[i].response_time = INT32_MAX;
    }

    // expected run time
    RandNum_set_parameter(seed_for_run_time, 1, MAX_RUN_TIME);
    for (int i = 0; i < size; ++i) {
        newProcessArray[i].expected_run_time = RandNum_get_random();
        newProcessArray[i].remaining_run_time = newProcessArray[i].expected_run_time;
    }

    // priority
    RandNum_set_parameter(seed_for_priority, MIN_PRIORITY, MAX_PRIORITY);
    for (int i = 0; i < size; ++i) {
        uint8_t priority = (uint8_t)RandNum_get_random();
        newProcessArray[i].initial_priority = priority;
        newProcessArray[i].priority = priority;
    }

    qsort(newProcessArray, (size_t)size, sizeof(process_t), process_comparator);

    process_queue_t *new_queue = malloc(sizeof(process_queue_t));

    new_queue->entry = newProcessArray;
    new_queue->size = (uint32_t)size;

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
    uint32_t size = process_queue->size;
    new->size = size;
    new->entry = malloc(sizeof(process_t) * size);
    memcpy(new->entry, process_queue->entry, sizeof(process_t) * size);
    return new;
}
