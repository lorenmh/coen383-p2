#include <stdio.h>
#include <stdlib.h>
#include "MinHeap.h"
#include "Output.h"
#include "Process.h"
#include "Scheduler.h"

int main(int argc, char* argv[]) {
    printf("Hello World\n");

    heap_t *heap = create_heap();

    insert(heap, 3, (void *)1);
    insert(heap, 2, (void *)2);
    insert(heap, 1, (void *)3);
    insert(heap, 4, (void *)4);

    printf("initial heap: ");
    print_heap(heap);

    while (!is_empty(heap)) {
        int extracted = (int) extract(heap);
        printf("extracted: %d, heap: ", extracted);
        print_heap(heap);
    }

    uint64_t arr[] = { 10, 2, 6, 8, 4, 1, 3, 5, 9, 7 };

    printf("unsorted arr: [ ");
    for (int i = 0; i < 10; i++) {
        insert(heap, arr[i], (void *) arr[i]);
        printf("%d ", (int) arr[i]);
    }
    printf("]\n");

    for (int i = 0; i < 10; i++) {
        arr[i] = (int) extract(heap);
    }

    printf("sorted arr: [ ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", (int) arr[i]);
    }
    printf("]\n");

    free_heap(heap);

    process_queue_t *pq = create_process_queue(20);

    print_process_queue(pq);

    history_t h = { NULL, 0 };

    rr(pq, &h);


    print_process_queue(pq);
    printf("%s\n", h.pid);

    free_process_queue(pq);

    return 0;
}
