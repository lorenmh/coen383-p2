#include <stdint.h>

#ifndef __MINHEAP_H__
#define __MINHEAP_H__

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


heap_t *create_heap(void);
void insert(heap_t*, uint32_t, void*);
void *extract(heap_t*);
void print(heap_t*);
int is_empty(heap_t*);
void free_heap(heap_t*);

#endif
