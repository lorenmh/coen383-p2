#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"

// function prototypes
heap_t *create_heap();
void insert(heap_t*, uint16_t, void*);
void *extract(heap_t*);
void print(heap_t*);
int is_empty(heap_t*);
void free_heap(heap_t*);


int main(int argc, char* argv[]) {
	printf("Hello World\n");

	heap_t *h = create_heap();

	insert(h, 3, (void *)1);
	insert(h, 2, (void *)2);
	insert(h, 1, (void *)3);
	insert(h, 4, (void *)4);

	printf("initial heap: ");
	print(h);

	while (!is_empty(h)) {
		int extracted = (int) extract(h);
		printf("extracted: %d, heap: ", extracted);
		print(h);
	}

	uint64_t arr[] = { 10, 2, 6, 8, 4, 1, 3, 5, 9, 7 };

	printf("unsorted arr: [ ");
	for (int i = 0; i < 10; i++) {
		insert(h, arr[i], (void *) arr[i]);
		printf("%d ", (int) arr[i]);
	}
	printf("]\n");

	for (int i = 0; i < 10; i++) {
		arr[i] = (int) extract(h);
	}

	printf("sorted arr: [ ");
	for (int i = 0; i < 10; i++) {
		printf("%d ", (int) arr[i]);
	}
	printf("]\n");

	free_heap(h);

	return 0;
}
