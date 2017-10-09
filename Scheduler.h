#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "Process.h"
#include "Output.h"
#include <stdbool.h>


/**
 * First Come First Serve scheduler
 *
 * @param pq pointer to the process queue
 * @param h pointer to the result
 */
void fcfs(process_queue_t *pq, history_t *h);


/**
 * Shortest Job First scheduler
 *
 * @param pq pointer to the process queue
 * @param h pointer to the result
 */
void sjf(process_queue_t *pq, history_t *h);


/**
 * Shortest Remaining Time First scheduler
 *
 * @param pq pointer to the process queue
 * @param h pointer to the result
 */
void srt(process_queue_t *pq, history_t *h);

/**
 * Round Robin scheduler
 *
 * @param pq pointer to the process queue
 * @param h pointer to the result
 */
void rr(process_queue_t *pq, history_t *h);

/**
 * Highest Priority First, non-preemptive scheduler
 *
 * @param pq pointer to the process queue
 * @param h pointer to the result
 * @param with_aging whether use aging support or not
 */
void hpf_npe(process_queue_t *pq, history_t *h, bool with_aging);

/**
 * Highest Priority First, preemptive scheduler
 *
 * @param pq pointer to the process queue
 * @param h pointer to the result
 */
void hpf_pe(process_queue_t *pq, history_t *h);





#endif //COEN383_P2_SCHEDULER_H
