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
#define MAX_PRIORITY_BOOST_QUANTUM (2)



typedef struct {
    uint32_t (*key_policy)(process_t *);
    uint32_t (*interrupt_policy)(process_t*);
} scheduler_context;


extern scheduler_context fcfs_context;
extern scheduler_context srt_context;
extern scheduler_context hpf_npe_context;
extern scheduler_context sjf_context;
extern scheduler_context rr_context;
extern scheduler_context hpf_pe_context;

void Scheduler(process_queue_t *pq, history_t *h, scheduler_context *scheduler_policy, bool with_aging);




#endif //COEN383_P2_SCHEDULER_H
