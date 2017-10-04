//
// Created by Yujian Zhang on 10/3/17.
//

#ifndef COEN383_P2_OUTPUT_H
#define COEN383_P2_OUTPUT_H

#include <stdio.h>
#include <stdint.h>
#include "Process.h"

typedef struct {
    char *pid;

    // total number of char in "pid" string, excluding '\0'
    int size;
} history_t;

typedef struct {
    uint32_t avg_response_time;
    uint32_t avg_turnaround_time;

} result_t;


/**
 * print out the history into the console
 *
 * @param history pointer to the history structure
 */
void print_history(history_t *history);


/**
 * generate the result accroding to the scheduler
 *
 * @param process_queue pointer to the process queue
 * @param result pointer to the result structrue
 */
void generate_result(process_queue_t* process_queue, result_t *result);

#endif //COEN383_P2_OUTPUT_H
