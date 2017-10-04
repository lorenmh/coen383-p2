#include "Scheduler.h"
#include "Process.h"
#include "Output.h"
#include <stdlib.h>
#include <string.h>

#define MAX_BUFF_SIZE 2000

void fcfs(process_queue_t *pq, history_t *h) {
    uint32_t current_quanta = 0;
    uint32_t process_size = pq->size;
    char buff_for_history[MAX_BUFF_SIZE] = {0};
    int history_size = 0;
    for (int process_queue_index = 0; process_queue_index < process_size; ++process_queue_index) {
        process_t *current_process = &((pq->entry)[process_queue_index]);
        if (current_quanta < current_process->arrival_time) {
            uint32_t end_of_idle = current_process->arrival_time;
            for (uint32_t i = current_quanta; i < end_of_idle; ++i) {
                buff_for_history[history_size] = '0';
                // 0 for idle
                history_size += 1;
            }
            current_quanta = end_of_idle;
        }
        char process_identifier;


        if (current_process->id < 26) {
            process_identifier = (char)((int)'A' + current_process->id);
        }else if (current_process < 52) {
            process_identifier = (char)((int)'a' + current_process->id - 26);
        }else {
            // we cant use letter to represent new process
            // because it would collide with existing process's identifier
            process_identifier = '1';
        }

        current_process->response_time = current_process->arrival_time - current_quanta;
        current_process->turnaround_time = current_process->response_time + current_process->expected_run_time;

        uint32_t end_of_exec = current_quanta + current_process->expected_run_time;
        for (uint32_t j = current_quanta; j < end_of_exec; ++j) {
            // if we could manage to limit the total number of process, we could
            // use A - Z and a - z to represent our process ID
            buff_for_history[history_size] = process_identifier;
            history_size += 1;
        }
    }
    h->pid = malloc(sizeof(char) * (history_size + 1));
    memcpy(h->pid, buff_for_history, sizeof(char) * history_size);
    (h->pid)[history_size] = '\0';
    h->size = history_size;
}


void sjf(process_queue_t *pq, history_t *h) {

}

void srt(process_queue_t *pq, history_t *h) {

}

void rr(process_queue_t *pq, history_t *h) {

}

void hpf_npe(process_queue_t *pq, history_t *h) {

}

void hpf_pe(process_queue_t *pq, history_t *h) {

}
