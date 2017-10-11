#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MinHeap.h"
#include "Output.h"
#include "Process.h"
#include "Scheduler.h"

int main(int argc, char* argv[]) {
    heap_t *heap = create_heap();

    insert(heap, 3, (void *)1);
    insert(heap, 2, (void *)2);
    insert(heap, 1, (void *)3);
    insert(heap, 4, (void *)4);

    printf("initial heap: ");
    print_heap(heap);
    delete(heap, 2);
    print_heap(heap);

    process_queue_t *pq = create_process_queue(20);

    print_process_queue(pq);

    history_t history;

    schedule(pq, &history, &hpf_pe_age_policy);
    printf("%s\n", (char *) history);

    print_process_queue(pq);

    free_process_queue(pq);

    return 0;
}
