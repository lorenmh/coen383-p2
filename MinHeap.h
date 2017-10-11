#ifndef MINHEAP_H
#define MINHEAP_H

#include <stdint.h>

#define MINHEAP_BUF_SIZE_INIT 8
#define MINHEAP_BUF_RESIZE_FACTOR 2

typedef struct {
    uint32_t key;
    void *value;
} node_t;

typedef struct {
    uint16_t size;
    uint16_t buf_size;
    node_t *buf;
} heap_t;


/**
 * Creates a min heap
 *
 * @return pointer to the heap
 */
heap_t *create_heap(void);


/**
 * Inserts a value into the heap with a compared key
 *
 * @param heap a pointer to the heap to insert into
 * @param key a key which will be used to compare against
 * @param value the value which will returned when extract is called
 */
void insert(heap_t *heap, uint32_t key, void *value);

void delete(heap_t *heap, int deleted_index);


/**
 * Extracts the minimum value from the heap
 *
 * @param heap a pointer to the heap which will have its min node removed
 * @return void pointer of the value
 */
void *extract(heap_t *heap);


/**
 * Prints the values in the heap (for debug)
 *
 * @param heap a pointer to the heap which will be printed
 */
void print_heap(heap_t *heap);


/**
 * Returns 1 if the heap is empty, else returns 0
 *
 * @param heap a pointer to the heap
 * @return int [0,1]
 */
int is_empty(heap_t *heap);


/**
 * Frees the memory used by the heap
 *
 * @param heap a pointer to the heap which will be freed
 */
void free_heap(heap_t *heap);

#endif
