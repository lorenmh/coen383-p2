#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "Process.h"
#include "Output.h"
#include <stdbool.h>

#define MAX_BUFF_SIZE (2000)
#define PRIORITY_MULTIPLIER (10000)
#define TIME_MULTIPLIER (10000)
#define AGING_QUANTUM (5)
#define RR_QUANTUM (1)



typedef struct {
    uint32_t (*key_policy)(process_t *);
    uint32_t (*interrupt_policy)(uint32_t, process_t*);
} scheduler_context;


extern scheduler_context fcfs_context;
extern scheduler_context srt_context;
extern scheduler_context hpf_npe_context;
extern scheduler_context sjf_context;
extern scheduler_context rr_context;
extern scheduler_context hpf_pe_context;

void Scheduler(process_queue_t *pq, history_t *h, scheduler_context *scheduler_policy, bool with_aging);


/**
 * Shortest Job First scheduler
 *
 * @param pq pointer to the process queue
 * @param h pointer to the result
 */
void sjf(process_queue_t *pq, history_t *h);


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
