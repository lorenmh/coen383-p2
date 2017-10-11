//
// Created by Yujian Zhang on 10/3/17.
//

#ifndef COEN383_P2_OUTPUT_H
#define COEN383_P2_OUTPUT_H

#include <stdio.h>
#include <stdint.h>
#include "Process.h"

#define HISTORY_BUF_SIZE 400

typedef char history_t[HISTORY_BUF_SIZE];

typedef struct {
    double avg_response_time;
    double avg_turnaround_time;
    double avg_waiting_time;
    double throughput;
} result_t;

/**
 * print out the history into the console
 *
 * @param history pointer to the history structure
 */
void print_history(history_t *history);
void print_result(result_t *result);


/**
 * Allocates mem for a history struct and returns a pointer to it
 *
 * @return history pointer to the history structure
 */
history_t *create_history(void);


/**
 * frees the memory allocated for a history_t
 *
 * @param history pointer to the history structure
 */
void free_history(history_t *history);


/**
 * generate the result accroding to the scheduler
 *
 * @param process_queue pointer to the process queue
 * @param result pointer to the result structrue
 */
void generate_result(process_queue_t* process_queue, result_t *result);

#endif //COEN383_P2_OUTPUT_H
