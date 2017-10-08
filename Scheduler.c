#include "Scheduler.h"
#include "Process.h"
#include "Output.h"
#include "MinHeap.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_BUFF_SIZE 2000
#define PREEMPT_QUANTUM 5

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

    heap_t *process_heap = create_heap();

    int arriving_process_index = 0;
    int quantum = 0;

    while (quantum < 100) {
        // at the beginning of the quantum, check for arrived processes
        while (arriving_process_index < pq->size) {
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

        if (is_empty(process_heap)) {
            // the process heap is empty, so let's skip to the quantum where
            // the next process arrives

            process_t *next_process = &pq->entry[arriving_process_index];

            if (next_process->arrival_time >= 100) break;

            int quantum_delta = next_process->arrival_time - quantum;

            // the CPU is idle while waiting until next process arrives
            memset(
                  &history_buf[quantum],
                  '0',
                  quantum_delta
            );

            quantum += quantum_delta;

            continue;
        }

        // If we've gotten this far then the heap is not empty, lets grab the
        // next shortest job
        process_t *process = (process_t *) extract(process_heap);

        // update results
        process->execution_time = process->expected_run_time;
        process->response_time = quantum - process->arrival_time;

        // update history
        memset(
              &history_buf[quantum],
              process->id,
              process->expected_run_time
        );

        // increment quantum because we 'finished the task'
        quantum += process->expected_run_time;

        // update turnaround time
        process->turnaround_time = quantum - process->arrival_time;
    }

    int history_size = quantum + 1;

    h->pid = malloc(sizeof(char) * (history_size + 1));

    memcpy(h->pid, history_buf, sizeof(char) * history_size);

    (h->pid)[history_size] = '\0';
    h->size = history_size;
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
        if (current_quanta > 100 && is_empty(process_pool)) {
            break;
        }
        while (process_index < pq->size && (pq->entry)[process_index].arrival_time <= current_quanta) {
            if (current_quanta > 99) {
                break;
            }
            process_t *new_process = &((pq->entry)[process_index]);
            insert(process_pool, new_process->expected_run_time, (void*)new_process);
            process_index += 1;
        }
        process_t *current_process = NULL;

        while (!is_empty(process_pool)) {
            current_process = extract(process_pool);
            if (current_quanta > 99 && current_process->response_time == INT32_MAX) {
                // this process has not get its first cpu before quanta 100
                // just ignore it
                current_process = NULL;
                continue;
            }else {
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

    free_heap(process_pool);

}

void rr(process_queue_t *pq, history_t *h) {
  if (h == NULL) {
            return;
    }

    int time = 0, time_slice = 1, remaining_processes, current_quanta = 0;
    uint32_t process_size = pq->size;               //process queue size
    remaining_processes = process_size;
    
    char buff_for_history[MAX_BUFF_SIZE];                //history buffer
    int history_size = 0;                           //History buffer size

    //Idle time before the CPU gets the first process
    for (int idle_time = 0; idle_time < (pq->entry)[0].arrival_time; ++idle_time) {
        buff_for_history[history_size] = '0';
        history_size += 1;
        current_quanta++;

    }

    current_quanta = (pq->entry)[0].arrival_time;
    int process_queue_index = 0, count = 0;
    time = current_quanta;

    while(remaining_processes != 0){
        process_t *current_process = &((pq->entry)[process_queue_index]);
        process_t *next_process = &((pq->entry)[process_queue_index+1]);

        if (current_quanta > 100) {
            break;
        }

        if(current_process->remaining_run_time < time_slice && current_process->remaining_run_time > 0){
            printf("made it far");

            time+=current_process->remaining_run_time;
            current_process->remaining_run_time = 0;
            current_process->completed_flag = 1;
            remaining_processes--;
        }
        else if(current_process->remaining_run_time > 0){
            if(current_process->arrival_flag == 0){
                current_process->response_time = current_quanta;
                current_process->arrival_flag = 1;
            }
            buff_for_history[history_size] = current_process->id;       
            history_size += 1;
            current_process->remaining_run_time-=time_slice;
            time+=time_slice;
            current_quanta += 1;
        }
        if(current_process->remaining_run_time == 0 && current_process->completed_flag == 1){
            printf("made it this far");
            remaining_processes--;
            current_process->turnaround_time+=time - current_process->arrival_time;
            current_process->completed_flag = 0;
        }
        

        if(process_queue_index == process_size - 1)
            process_queue_index = 0;
        else if(next_process->arrival_time <= time)
            process_queue_index++;
        else
            process_queue_index = 0;        
            
    }

    h->pid = malloc(sizeof(char) * (history_size + 1));
    memcpy(h->pid, buff_for_history, sizeof(char) * history_size);
    (h->pid)[history_size] = '\0';
    h->size = history_size;

  /*  
if (h == NULL) {
        return;
    }

    int time, count, time_slice = 1, remaining_processes, current_quanta = 0;
    uint32_t process_size = pq->size;               
    remaining_processes = process_size;
    
    char buff_for_history[MAX_BUFF_SIZE];               
    int history_size = 0;                          

    for (int idle_time = 0; idle_time < (pq->entry)[0].arrival_time; ++idle_time) {
        buff_for_history[history_size] = '0';
        history_size += 1;
        current_quanta += 1;
    }

    current_quanta = (pq->entry)[0].arrival_time;
    int process_queue_index = 0;

    for(time = current_quanta, count = 0; remaining_processes != 0;){
        process_t *current_process = &((pq->entry)[count]);
        process_t *next_process = &((pq->entry)[count+1]);

        if (current_quanta > 100) {
            break;
        }
        if(current_process->remaining_run_time <= time_slice && current_process->remaining_run_time > 0){
            time+=current_process->remaining_run_time;
            current_process->remaining_run_time = 0;
            current_process->completed_flag = 1;
        }
        else if(current_process->remaining_run_time > 0){
            if(current_process->arrival_flag == 0){
                current_process->response_time = current_quanta;
                current_process->arrival_flag = 1;
            }
            buff_for_history[history_size] = current_process->id;       
            history_size += 1;
            current_process->remaining_run_time-=time_slice;
            time+=time_slice;
            current_quanta += 1;
        }
        if(current_process->remaining_run_time == 0 && current_process->completed_flag == 1){
            remaining_processes--;
            current_process->turnaround_time+=time - current_process->arrival_time;
            current_process->completed_flag = 0;
        }
        if(count == process_size - 1)
            count = 0;
        else if(next_process->arrival_time <= time)
            count++;
        else
            count = 0;
    }

     h->pid = malloc(sizeof(char) * (history_size + 1));
    memcpy(h->pid, buff_for_history, sizeof(char) * history_size);
    (h->pid)[history_size] = '\0';
    h->size = history_size;
    */
    /*
    if (h == NULL) {
            return;
    }

    int time = 0, time_slice = 1, remaining_processes, current_quanta = 0;
    uint32_t process_size = pq->size;               //process queue size
    remaining_processes = process_size;
    
    char buff_for_history[MAX_BUFF_SIZE];                //history buffer
    int history_size = 0;                           //History buffer size

    //Idle time before the CPU gets the first process
    for (int idle_time = 0; idle_time < (pq->entry)[0].arrival_time; ++idle_time) {
        buff_for_history[history_size] = '0';
        history_size += 1;
    }


    int process_queue_index = 0, count = 0;
    //current_quanta++;


    while(remaining_processes != 0){
        process_t *current_process = &((pq->entry)[process_queue_index]);
        


        if(current_process->remaining_run_time == 0 && current_process->completed_flag != 1){
            current_process->turnaround_time = current_quanta - (process_size-1);
            current_process->completed_flag = 1;
            remaining_processes--;
        }
        if(current_process->completed_flag != 1 && current_process->remaining_run_time != 0){
            if(current_process->arrival_flag == 0){
                if(process_queue_index == 0){
                    current_process->response_time = count;
                    count++;
                }
                else{
                    current_process->response_time = count++;
                }
                current_process->turnaround_time = 0;
                current_process->arrival_flag = 1;
            }
            buff_for_history[history_size] = current_process->id;       
            history_size += 1;
            current_process->remaining_run_time--;
            current_quanta++;
        }

        if(process_queue_index == process_size - 1)
            process_queue_index = 0;
        else
            process_queue_index++;        
            
    }

    h->pid = malloc(sizeof(char) * (history_size + 1));
    memcpy(h->pid, buff_for_history, sizeof(char) * history_size);
    (h->pid)[history_size] = '\0';
    h->size = history_size;
    */
}

void hpf_npe(process_queue_t *pq, history_t *h) {
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

    heap_t *process_pools[MAX_PRIORITY];
    for (int i = 0; i < MAX_PRIORITY; ++i) {
        process_pools[i] = create_heap();
    }
    int32_t process_pools_bitmap = 0;


    while (true) {
        if (current_quanta > 99) {
            break;
        }
        while (process_index < pq->size && (pq->entry)[process_index].arrival_time <= current_quanta) {
            if (current_quanta > 99) {
                break;
            }
            process_t *new_process = &((pq->entry)[process_index]);
            int32_t priority = new_process->priority - 1;
            insert(process_pools[priority], new_process->arrival_time, (void*)new_process);
            process_pools_bitmap |= (1 << priority);
            process_index += 1;
        }

        process_t *current_process = NULL;
        if (process_pools_bitmap != 0) {

            int first_nonempty = 0;
            while (((process_pools_bitmap >> first_nonempty) & 1) == 0) {
                first_nonempty += 1;
            }
            current_process = extract(process_pools[first_nonempty]);
            if (is_empty(process_pools[first_nonempty])) {
                process_pools_bitmap &= (~(1 << first_nonempty));
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
        current_process->response_time = current_quanta - current_process->arrival_time;

        uint32_t end_of_exec = current_quanta + current_process->expected_run_time;
        current_process->turnaround_time = end_of_exec - current_process->arrival_time;
        for (int i = current_quanta; i < end_of_exec; ++i) {
            buff_for_history[history_size] = current_process->id;
            history_size += 1;
        }
        current_process->remaining_run_time = 0;
        current_process->context_switch_time = end_of_exec;
        current_process->flag = 1;

        current_quanta = end_of_exec;


    }

    h->pid = malloc(sizeof(char) * (history_size + 1));
    memcpy(h->pid, buff_for_history, sizeof(char) * history_size);
    (h->pid)[history_size] = '\0';
    h->size = history_size;

    // clean up
    for (int i = 0; i < MAX_PRIORITY; ++i) {
        free_heap(process_pools[i]);
    }

}

void hpf_pe(process_queue_t *pq, history_t *h) {
    uint32_t process_size = pq->size;

    char history_buf[MAX_BUFF_SIZE];

    heap_t *priority_heaps[4] = {
        create_heap(), // priority 1
        create_heap(), // priority 2
        create_heap(), // priority 3
        create_heap()  // priority 4
    };

    int arriving_process_index = 0;
    int quantum = 0;

    while (quantum < 100) {
        // at the beginning of the quantum, check for arrived processes
        while (arriving_process_index < pq->size) {
            process_t *arriving_process = &pq->entry[arriving_process_index];
            if (arriving_process->arrival_time <= quantum) {
                // if the process is arriving, then insert it into the SJF heap
                insert(
                    priority_heaps[arriving_process->priority - 1],
                    arriving_process->expected_run_time,
                    arriving_process
                );
                arriving_process_index++;
            } else {
                break;
            }
        }

        //if (is_empty(process_heap)) {
        //    // the process heap is empty, so let's skip to the quantum where
        //    // the next process arrives

        //    process_t *next_process = &pq->entry[arriving_process_index];

        //    if (next_process->arrival_time >= 100) break;

        //    int quantum_delta = next_process->arrival_time - quantum;

        //    // the CPU is idle while waiting until next process arrives
        //    memset(
        //          &history_buf[quantum],
        //          '0',
        //          quantum_delta
        //    );

        //    quantum += quantum_delta;

        //    continue;
        //}

        //// If we've gotten this far then the heap is not empty, lets grab the
        //// next shortest job
        //process_t *process = (process_t *) extract(process_heap);

        //// update results
        //process->execution_time = process->expected_run_time;
        //process->response_time = quantum - process->arrival_time;

        //// update history
        //memset(
        //      &history_buf[quantum],
        //      process->id,
        //      process->expected_run_time
        //);

        //// increment quantum because we 'finished the task'
        //quantum += process->expected_run_time;

        //// update turnaround time
        //process->turnaround_time = quantum - process->arrival_time;
    }

    int history_size = quantum + 1;

    h->pid = malloc(sizeof(char) * (history_size + 1));

    memcpy(h->pid, history_buf, sizeof(char) * history_size);

    (h->pid)[history_size] = '\0';
    h->size = history_size;

}
