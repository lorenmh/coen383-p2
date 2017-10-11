#include "Scheduler.h"
#include "Process.h"
#include "Output.h"
#include "MinHeap.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define PREEMPT_TIME_SLICE 1
#define AGE_THRESHOLD 5

// first come first serve
uint32_t sort_by_arrival_time(process_t *process, int quantum) {
    return process->arrival_time;
}

// shortest job first
uint32_t sort_by_expected_run_time(process_t *process, int quantum) {
    return process->expected_run_time;
}

// shortest remaining time
uint32_t sort_by_remaining_run_time(process_t *process, int quantum) {
    return process->remaining_run_time;
}

// highest priority preemptive & non-preemptive
uint32_t sort_by_priority_and_last_run(process_t *process, int quantum) {
    return 1000 * process->priority + quantum;
}

// round robin
uint32_t sort_by_last_run(process_t *process, int quantum) {
    return quantum;
}

// non-preemptive will run the process to completion
uint32_t no_preempt_run_time(process_t *process) {
    return process->expected_run_time;
}

// preemptive will run until process completes OR it is preempted
uint32_t preempt_run_time(process_t *process) {
    uint32_t rrt = process->remaining_run_time;
    return rrt < PREEMPT_TIME_SLICE ? rrt : PREEMPT_TIME_SLICE;
}

policy_t fcfs_policy = {
    sort_by_arrival_time,
    no_preempt_run_time,
    0 // no aging
};

policy_t sjf_policy = {
    sort_by_expected_run_time,
    no_preempt_run_time,
    0 // no aging
};

policy_t srt_policy = {
    sort_by_remaining_run_time,
    preempt_run_time,
    0 // no aging
};

policy_t rr_policy = {
    sort_by_last_run,
    preempt_run_time,
    0 // no aging
};

policy_t hpf_npe_policy = {
    sort_by_priority_and_last_run,
    no_preempt_run_time,
    0
};

policy_t hpf_pe_policy = {
    sort_by_priority_and_last_run,
    preempt_run_time,
    0
};

policy_t hpf_npe_age_policy = {
    sort_by_priority_and_last_run,
    no_preempt_run_time,
    1
};

policy_t hpf_pe_age_policy = {
    sort_by_priority_and_last_run,
    preempt_run_time,
    1
};

void schedule(process_queue_t *pq, history_t *history, policy_t *policy) {
    heap_t *process_heap = create_heap();
    int pq_size = pq->size;

    int arriving_process_index = 0;
    int quantum = 0;

    // execute loop while there are items in the heap or quantum < 100
    while (quantum < 100 || !is_empty(process_heap)) {
        // this loop will grab all processes arriving this quantum and will
        // add them to the heap
        while (arriving_process_index < pq_size) {
            process_t *arriving_process = &pq->entry[arriving_process_index];
            if (arriving_process->arrival_time <= quantum) {
                // if the process is arriving, then insert it into the SJF heap
                insert(
                    process_heap,
                    policy->sort_key(arriving_process, arriving_process->arrival_time),
                    arriving_process
                );

                // continue loop to see if more processes are arriving
                arriving_process_index++;
            } else {
                break;
            }
        }

        // the heap is empty, skip to the next quantum where a process arrives
        if (is_empty(process_heap)) {
            process_t *next_process = &pq->entry[arriving_process_index];

            if (next_process->arrival_time >= 100) break;

            int quantum_delta = next_process->arrival_time - quantum;

            memset(&(*history)[quantum], '0', quantum_delta);

            quantum += quantum_delta;

            continue;
        }

        // process_heap is not empty, so lets grab the next process
        process_t *process = (process_t *) extract(process_heap);

        // the process has just begun running, so lets set the response time
        if (!process->has_already_run){
            if (quantum >= 100) continue;
            process->response_time = quantum - process->arrival_time;
            process->has_already_run = 1;
        }

        // reset the age to 0
        process->age = 0;

        // run the process until it completes or is preempted
        int process_run_time = policy->process_run_time(process);

        // decrement remaining time the amount that we have run the process
        process->remaining_run_time -= process_run_time;

        // update the history with this process id
        memset(&(*history)[quantum], process->id, process_run_time);

        // increase quantum the amount of time we have been running the proc
        quantum += process_run_time;

        // for every process in the heap, increment the age and change the
        // priority if the age >= AGE_THRESHOLD
        if (policy->is_aging) {
            int process_heap_size = process_heap->size;

            for (int i = 0; i < process_heap->size; i++) {
                // ap == 'aging process', the process which is aging this iter
                process_t *ap = process_heap->buf[i].value;

                // the process has been waiting while the process run
                int new_age = ap->age + process_run_time;

                // if the total wait time is greater than the threshold then
                // increase the priority
                if (new_age >= AGE_THRESHOLD && ap->priority != MIN_PRIORITY) {
                    int change_in_priority = new_age / AGE_THRESHOLD;
                    int new_priority = ap->priority - change_in_priority;

                    // make sure that we dont go under the min priority
                    ap->priority = (
                          new_priority >= MIN_PRIORITY ? new_priority : MIN_PRIORITY
                    );

                    new_age = new_age % AGE_THRESHOLD;

                    // remove the process from the heap
                    delete(process_heap, i);

                    // re-insert the process into the heap
                    insert(
                        process_heap,
                        policy->sort_key(ap, quantum),
                        ap
                    );
                }

                ap->age = new_age;
            }
        }

        if (process->remaining_run_time == 0) {
            // if the process is completed, set the analysis values
            process->turnaround_time = quantum - process->arrival_time;
        } else {
            // else the process is not yet completed, reinsert into the heap
            insert(
                process_heap,
                policy->sort_key(process, quantum),
                process
            );
        }
    }

    // null out the string
    (*history)[quantum] = '\0';
}
