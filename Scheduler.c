#include "Scheduler.h"
#include "Process.h"
#include "Output.h"
#include "MinHeap.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>




void Scheduler(process_queue_t *pq, history_t *h, scheduler_context *scheduler_policy, bool with_aging) {
    if (pq == NULL || pq->entry == NULL || h == NULL) {
        return;
    }

    char buff_for_history[MAX_BUFF_SIZE] = {0};
    int history_size = 0;

    for (int idle_time = 0; idle_time < (pq->entry)[0].arrival_time; ++idle_time) {
        buff_for_history[history_size] = '0';
        history_size += 1;
    }
    uint32_t current_quanta = (pq->entry)[0].arrival_time;
    int process_index = 0;

    heap_t *process_pool = create_heap();


    while (true) {
        if (current_quanta > 99 && is_empty(process_pool)) {
            break;
        }
        while (process_index < pq->size && (pq->entry)[process_index].arrival_time <= current_quanta) {
            if (current_quanta > 99) {
                break;
            }
            process_t *new_process = &((pq->entry)[process_index]);
            uint32_t key = scheduler_policy->key_policy(new_process);
            insert(process_pool, key, (void*)new_process);
            process_index += 1;
        }
        process_t *current_process = NULL;

        while (!is_empty(process_pool)) {
            current_process = extract(process_pool);
            if (current_quanta > 99 && current_process->response_time == INT32_MAX) {
                // this process has not gotten its first cpu before quanta 100
                // just ignore it
                current_process = NULL;
                continue;
            }else {
                current_process->arrival_flag = 1;
                break;
            }
        }
        if (current_process == NULL) {
            if (current_quanta > 99) {
                continue;
            }
            uint32_t end_of_idle = 99;
            if (process_index < pq->size) {
                end_of_idle = (pq->entry)[process_index].arrival_time;
            }
            for (int idle_time = current_quanta; idle_time < end_of_idle; ++idle_time) {
                buff_for_history[history_size] = '0';
                history_size += 1;
            }
            current_quanta = end_of_idle;
            continue;
        }
        if (current_process->response_time == INT32_MAX) {
            current_process->response_time = current_quanta - current_process->arrival_time;
        }


        uint32_t next_interrupt_time;
        if (process_index < pq->size) {
            uint32_t next_arrival = (pq->entry)[process_index].arrival_time;
            next_interrupt_time = scheduler_policy->interrupt_policy(current_quanta, current_process);
        }else {
            next_interrupt_time = scheduler_policy->interrupt_policy(current_quanta, current_process);
        }
        for (uint32_t i = current_quanta; i < next_interrupt_time; ++i) {
            buff_for_history[history_size] = current_process->id;
            history_size += 1;
        }
        uint32_t current_process_running_time = next_interrupt_time - current_quanta;
        current_process->execution_time += current_process_running_time;
        current_process->remaining_run_time -= current_process_running_time;
        current_process->virtual_priority = current_process->priority;


        current_quanta = next_interrupt_time;
        current_process->context_switch_time = current_quanta;
        if (current_process->remaining_run_time > 0) {
            uint32_t key = scheduler_policy->key_policy(current_process);
            insert(process_pool, key, (void*)current_process);
        }else {
            current_process->turnaround_time = current_quanta - current_process->arrival_time;
            current_process->completed_flag = 1;
        }

        if (with_aging && !is_empty(process_pool)) {
            // ugly!
            // but works
            heap_t *new_pool = create_heap();
            while (!is_empty(process_pool)) {
                process_t *process = extract(process_pool);
                uint8_t new_priority_offset = (uint8_t)((current_quanta - process->context_switch_time) / AGING_QUANTUM);
                if (new_priority_offset > process->priority - MIN_PRIORITY) {
                    process->virtual_priority = MIN_PRIORITY;
                }else {
                    process->virtual_priority = process->priority - new_priority_offset;
                }
                uint32_t key = scheduler_policy->key_policy(process);
                insert(new_pool, key, (void*)process);
            }
            heap_t *delete = process_pool;
            process_pool = new_pool;
            free_heap(delete);
        }


    }

    h->pid = malloc(sizeof(char) * (history_size + 1));
    memcpy(h->pid, buff_for_history, sizeof(char) * history_size);
    (h->pid)[history_size] = '\0';
    h->size = history_size;

    // clean up
    free_heap(process_pool);
}

//////////////
uint32_t context_switch_key_policy(process_t *process) {
    return process->context_switch_time * TIME_MULTIPLIER + process->arrival_time;

}

uint32_t non_preemptive_interrupt_policy(uint32_t current_quanta, process_t *current_process) {
    return current_quanta + current_process->remaining_run_time;
}

scheduler_context fcfs_context = {
     context_switch_key_policy,
     non_preemptive_interrupt_policy,
};

//////////////
uint32_t shortest_job_key_policy(process_t *process) {
    return process->remaining_run_time * TIME_MULTIPLIER + process->arrival_time;
}

uint32_t preemptive_interrupt_policy(uint32_t current_quanta, process_t *current_process) {
    return current_quanta + 1;
}

scheduler_context srt_context = {
    shortest_job_key_policy,
    preemptive_interrupt_policy,
};

/////////////
uint32_t hpf_key_policy(process_t *process) {
    return process->virtual_priority * PRIORITY_MULTIPLIER + process->context_switch_time;
}

scheduler_context hpf_npe_context = {
    hpf_key_policy,
    non_preemptive_interrupt_policy
};


///////////
scheduler_context sjf_context = {
    shortest_job_key_policy,
    non_preemptive_interrupt_policy
};



////////
uint32_t rr_interrupt_policy(uint32_t current_quanta, process_t *current_process) {
    uint32_t end_of_process = current_quanta + current_process->remaining_run_time;
    uint32_t end_of_rr = current_quanta + RR_QUANTUM;
    if (end_of_process > end_of_rr) {
        return end_of_rr;
    }else {
        return end_of_process;
    }

}

scheduler_context rr_context = {
    context_switch_key_policy,
    rr_interrupt_policy
};


scheduler_context hpf_pe_context = {
    hpf_key_policy,
    preemptive_interrupt_policy
};


uint32_t to_key(int priority, int quantum) {
    return priority * PRIORITY_MULTIPLIER + quantum;
}

void hpf_pe(process_queue_t *pq, history_t *h) {
    uint32_t process_size = pq->size;

    char history_buf[MAX_BUFF_SIZE];

    heap_t *process_heap = create_heap();

    int arriving_process_index = 0;
    int quantum = 0;

    while (quantum < 100 || !is_empty(process_heap)) {
        // at the beginning of the quantum, check for arrived processes
        while (arriving_process_index < pq->size) {
            process_t *arriving_process = &pq->entry[arriving_process_index];
            if (arriving_process->arrival_time <= quantum) {
                // if the process is arriving, then insert it into the SJF heap
                insert(
                    process_heap,
                    to_key(arriving_process->priority, quantum),
                    arriving_process
                );
                arriving_process_index++;
            } else {
                break;
            }
        }

        if (is_empty(process_heap)) {
            // the process heap is empty, so let's skip to the quantum where
            // the next process arrives

            process_t *next_process = &pq->entry[arriving_process_index];

            if (next_process->arrival_time >= 100) break;

            int quantum_delta = next_process->arrival_time - quantum;

            // the CPU is idle while waiting until next process arrives
            memset(&history_buf[quantum], '0', quantum_delta);

            quantum += quantum_delta;

            continue;
        }

        // process_heap is not empty, so lets grab the next process
        process_t *process = (process_t *) extract(process_heap);

        if (!process->arrival_flag){
            if (quantum >= 100) continue;
            process->response_time = quantum - process->arrival_time;
            process->arrival_flag = 1;
        }

        process->remaining_run_time -= 1;

        memset(&history_buf[quantum], process->id, 1);

        quantum += 1;

        if (process->remaining_run_time == 0) {
            process->execution_time = process->expected_run_time;
            process->response_time = quantum - process->arrival_time;
            process->turnaround_time = quantum - process->arrival_time;
        } else {
            insert(
                process_heap,
                to_key(process->priority, quantum),
                process
            );
        }

    }

    int history_size = quantum + 1;

    h->pid = malloc(sizeof(char) * (history_size + 1));

    memcpy(h->pid, history_buf, sizeof(char) * history_size);

    (h->pid)[history_size] = '\0';
    h->size = history_size;

}


