#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MinHeap.h"
#include "Output.h"
#include "Process.h"
#include "Scheduler.h"

#define NUM_PROC 12

typedef struct {
    policy_t *policy;
    char *title;
} sched_run_t;

void run() {
    sched_run_t sched_runs[8] = {
        {&fcfs_policy, "FCFS"},
        {&sjf_policy, "SJF"},
        {&srt_policy, "SRT"},
        {&rr_policy, "RR"},
        {&hpf_npe_policy, "HPF - Non-Preemptive"},
        {&hpf_pe_policy, "HPF - Preemptive"},
        {&hpf_npe_age_policy, "HPF - Non-Preemptive with Aging"},
        {&hpf_pe_age_policy, "HPF - Preemptive with Aging"},
    };

    result_t result_accumulators[8] = {0};

    printf(
        "================================================================="
        "===============\n"
    );

    for (int i = 0; i < 5; i++) {
        printf(
            "RUN %d =========================================================="
            "================\n\n",
            i + 1
        );

        printf("Process Queue:\n");
        process_queue_t *pq_template = create_process_queue(NUM_PROC);
        print_process_queue(pq_template);
        printf("\n");

        for (int j = 0; j < 8; j++) {
            process_queue_t *pq = clone_process_queue(pq_template);
            history_t h;
            policy_t *policy = sched_runs[j].policy;

            schedule(pq, &h, policy);

            printf("%s\n", sched_runs[j].title);
            print_history(&h);

            result_t result;
            generate_result(pq, &result);
            print_result(&result);

            printf("\n");

            free_process_queue(pq);

            result_t *result_acc = &result_accumulators[j];

            result_acc->avg_response_time += result.avg_response_time;
            result_acc->avg_turnaround_time += result.avg_turnaround_time;
            result_acc->avg_waiting_time += result.avg_waiting_time;
            result_acc->throughput += result.throughput;
        }

        printf(
            "================================================================="
            "===============\n"
        );
    }

    printf(
        "Total Results Over All Runs ========================================="
        "===========\n\n"
    );

    for (int i = 0; i < 8; i++) {
        printf("%s\n", sched_runs[i].title);
        result_t *result_acc = &result_accumulators[i];

        result_acc->avg_response_time /= 5.0;
        result_acc->avg_turnaround_time /= 5.0;
        result_acc->avg_waiting_time /= 5.0;
        result_acc->throughput /= 5.0;

        print_result(result_acc);
        printf("\n");
    }

    printf(
        "================================================================="
        "===============\n"
    );
    printf(
        "================================================================="
        "===============\n\n"
    );
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    run();
    return 0;
}
