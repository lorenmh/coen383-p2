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
    uint32_t (*interrupt_policy)(process_t*);
} scheduler_context;

typedef struct {
    uint32_t (*sort_key)(process_t*, int);
    uint32_t (*process_run_time)(process_t*);
    uint8_t is_aging;
} policy_t;

extern policy_t fcfs_policy;
extern policy_t sjf_policy;
extern policy_t srt_policy;
extern policy_t rr_policy;
extern policy_t hpf_npe_policy;
extern policy_t hpf_pe_policy;
extern policy_t hpf_npe_age_policy;
extern policy_t hpf_pe_age_policy;

uint32_t sort_by_arrival_time(process_t *process, int quantum);

uint32_t sort_by_expected_run_time(process_t *process, int quantum);

uint32_t sort_by_remaining_run_time(process_t *process, int quantum);

uint32_t sort_by_priority_and_last_run(process_t *process, int quantum);

uint32_t sort_by_last_run(process_t *process, int quantum);

uint32_t no_preempt_run_time(process_t *process);

uint32_t preempt_run_time(process_t *process);

void schedule(process_queue_t *pq, history_t *history, policy_t *policy);



#endif //COEN383_P2_SCHEDULER_H
