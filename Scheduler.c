#include "Scheduler.h"
#include "Process.h"
#include "Output.h"
#include "MinHeap.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BUFF_SIZE 2000

void fcfs(process_queue_t *pq, history_t *h) {
    if (h == NULL) {
        return;
    }
    uint32_t current_quanta = 0;
    uint32_t process_size = pq->size;
    char buff_for_history[MAX_BUFF_SIZE] = {0};
    int history_size = 0;
    for (int process_queue_index = 0; process_queue_index < process_size; ++process_queue_index) {

        process_t *current_process = &((pq->entry)[process_queue_index]);
        if (current_quanta < current_process->arrival_time) {
            uint32_t end_of_idle = current_process->arrival_time;
            for (uint32_t i = current_quanta; i < end_of_idle; ++i) {
                if (current_quanta > 100) {
                    break;
                }
                buff_for_history[history_size] = '0';
                // 0 for idle
                history_size += 1;
                current_quanta += 1;
            }

        }
        if (current_quanta > 100) {
            break;
        }
        current_process->response_time = current_quanta - current_process->arrival_time;
        current_process->turnaround_time = current_process->response_time + current_process->expected_run_time;

        uint32_t end_of_exec = current_quanta + current_process->expected_run_time;
        for (uint32_t j = current_quanta; j < end_of_exec; ++j) {
            buff_for_history[history_size] = current_process->id;
            history_size += 1;
        }
        current_quanta = end_of_exec;
    }
    h->pid = malloc(sizeof(char) * (history_size + 1));
    memcpy(h->pid, buff_for_history, sizeof(char) * history_size);
    (h->pid)[history_size] = '\0';
    h->size = history_size;
}


void sjf(process_queue_t *pq, history_t *h) {
    uint32_t process_size = pq->size;
    char history_buf[MAX_BUFF_SIZE];
    int history_size = 0;

    heap_t *process_heap = create_heap();
    process_t *current_process = &pq->entry[0];

    int start_quantum = current_process->arrival_time;

    // the CPU is idle while waiting until first process arrives
    memset(history_buf, '0', start_quantum);

    int arriving_process_index = 1;

    for (int quantum = start_quantum ;; quantum++) {
      // at the beginning of the quantum, check for arrived processes
      while (1) {
        process_t *arriving_process = &pq->entry[arriving_process_index];
        if (arriving_process->arrival_time <= quantum) {
          // if the process is arriving, then insert it into the SJF heap
          insert(
              process_heap,
              arriving_process->expected_run_time,
              arriving_process
          );
          arriving_process_index++;
        } else {
          break;
        }
      }

      //quantum += current_process->
      if (current_process == NULL) continue;

    }
}

void srt(process_queue_t *pq, history_t *h) {
    if (h == NULL) {
        return;
    }
    heap_t *process_pool = create_heap();
    int process_index = 0;

    char buff_for_history[MAX_BUFF_SIZE] = {0};
    int history_size = 0;

    for (int idle_time = 0; idle_time < (pq->entry)[0].arrival_time; ++idle_time) {
        buff_for_history[history_size] = '0';
        history_size += 1;
    }
    uint32_t current_quanta = (pq->entry)[0].arrival_time;


    while (true) {
        if (current_quanta > 100) {
            break;
        }
        while (process_index < pq->size && (pq->entry)[process_index].arrival_time <= current_quanta) {
            process_t *new_process = &((pq->entry)[process_index]);
            insert(process_pool, new_process->expected_run_time, (void*)new_process);
            process_index += 1;
        }
        process_t *current_process = extract(process_pool);
        if (current_process == NULL) {
            uint32_t end_of_idle = 100;
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


        uint32_t current_process_remaining_time = current_process->expected_run_time - current_process->execution_time;
        uint32_t next_interrupt_time = current_quanta + current_process_remaining_time;
        if (process_index < pq->size) {
            uint32_t next_arrival = (pq->entry)[process_index].arrival_time;
            if (next_arrival < next_interrupt_time) {
                next_interrupt_time = next_arrival;
            }
        }
        for (uint32_t i = current_quanta; i < next_interrupt_time; ++i) {
            buff_for_history[history_size] = current_process->id;
            history_size += 1;
        }
        uint32_t current_process_running_time = next_interrupt_time - current_quanta;
        current_process->execution_time += current_process_running_time;
        current_process_remaining_time -= current_process_running_time;

        if (current_process_remaining_time < 0) {
            printf("%d\n", current_process_remaining_time);
            exit(8);
        }
        current_quanta = next_interrupt_time;
        if (current_process_remaining_time > 0) {
            insert(process_pool, current_process_remaining_time, (void*)current_process);
        }else {
            current_process->turnaround_time = current_quanta - current_process->arrival_time;
        }



    }

    h->pid = malloc(sizeof(char) * (history_size + 1));
    memcpy(h->pid, buff_for_history, sizeof(char) * history_size);
    (h->pid)[history_size] = '\0';
    h->size = history_size;
}

void rr(process_queue_t *pq, history_t *h) {

}

void hpf_npe(process_queue_t *pq, history_t *h) {

}

void hpf_pe(process_queue_t *pq, history_t *h) {

}
