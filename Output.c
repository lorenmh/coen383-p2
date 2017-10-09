//
// Created by Yujian Zhang on 10/3/17.
//

#include "Output.h"
#include "Process.h"

void generate_result(process_queue_t *process_queue, result_t *result) {
    if (process_queue == NULL || process_queue->entry == NULL || result == NULL) {
        return;
    }


    uint32_t sum_response_time = 0;
    uint32_t sum_turnaround_time = 0;
    uint32_t sum_waiting_time = 0;

    int quanta_length = 100;

    int finished_process_count = 0;

    for (int i = 0; i < process_queue->size; ++i) {
        process_t *current_process = &(process_queue->entry)[i] ;
        if (current_process->turnaround_time != UINT32_MAX) {
            finished_process_count += 1;
            sum_response_time += current_process->response_time;
            sum_waiting_time += current_process->turnaround_time - current_process->expected_run_time;
            sum_turnaround_time += current_process->turnaround_time;
            int end_quanta = current_process->turnaround_time + current_process->arrival_time;
            if (end_quanta > quanta_length) {
                quanta_length = end_quanta;
            }
        }
    }

    result->avg_response_time = (double)sum_response_time / (double)finished_process_count;
    result->avg_turnaround_time = (double)sum_turnaround_time / (double)finished_process_count;
    result->avg_waiting_time = (double)sum_waiting_time / (double)finished_process_count;
    result->throughput = (double)finished_process_count / (double)quanta_length;
}

void print_history(history_t *history) {

}
