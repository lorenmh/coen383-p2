#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MinHeap.h"

heap_t *create_heap(void) {
    heap_t *h = malloc(sizeof(heap_t));

    h->size = 0;
    h->buf_size = MINHEAP_BUF_SIZE_INIT;
    h->buf = malloc(MINHEAP_BUF_SIZE_INIT * sizeof(node_t));

    return h;
}

void print_heap(heap_t *heap) {
    for (int i = 0; i < heap->size; i++) {
        node_t *curr = &heap->buf[i];
        printf("{k: %d, v: %p} ", curr->key, curr->value);
    }
    printf("\n");
}

int is_empty(heap_t *heap) {
    return heap->size == 0;
}

void insert(heap_t *heap, uint32_t key, void *value) {
    // resize the heap's buffer
    if (heap->size + 1 >= heap->buf_size) {
        heap->buf_size = heap->buf_size * MINHEAP_BUF_RESIZE_FACTOR;
        heap->buf = realloc(heap->buf, heap->buf_size * sizeof(node_t));
    }

    uint16_t index = heap->size++;
    node_t *buf = heap->buf;

    // add element to bottom of heap
    buf[index] = (node_t) { key, value };

    // swap with parents until heap constraints satisfied
    while(index > 0) {
        uint16_t parent_index = (index - 1) / 2;

        node_t parent = buf[parent_index];

        // parent is already smaller, do not swap
        if (key >= parent.key) break;

        // swap parent with child
        node_t tmp = buf[parent_index];
        buf[parent_index] = buf[index];
        buf[index] = tmp;

        // set index to parent index
        index = parent_index;
    }
}

void delete(heap_t *heap, int index) {
    node_t *buf = heap->buf;

    node_t swapped = buf[--(heap->size)];
    buf[index] = swapped;

    while (index <= heap->size / 2 - 1) {
        uint16_t child0_index = index * 2 + 1;
        uint16_t child1_index = index * 2 + 2;

        // NULL for child0 or child1 means that the element does not exist
        node_t *child0 = child0_index < heap->size ? &buf[child0_index] : NULL;
        node_t *child1 = child1_index < heap->size ? &buf[child1_index] : NULL;

        if (child0 == NULL) break;

        // we only compare with the smallest child and swap if its smaller
        node_t compared;
        uint16_t compared_index = -1;

        if (child1 == NULL || child0->key < child1->key) {
            compared = *child0;
            compared_index = child0_index;
        } else {
            compared = *child1;
            compared_index = child1_index;
        }

        // no need to do any more swaps, break
        if (swapped.key < compared.key) break;

        // the child has a lower key so we swap
        node_t tmp = buf[compared_index];
        buf[compared_index] = buf[index];
        buf[index] = tmp;

        index = compared_index;
    }
}

void *extract(heap_t *heap) {
    if (is_empty(heap)) return NULL;

    node_t *buf = heap->buf;
    void *value = buf[0].value;

    // move last element to root, decrement size
    node_t swapped = buf[--(heap->size)];

    buf[0] = swapped;
    uint16_t index = 0;

    while (index <= heap->size / 2 - 1) {
        uint16_t child0_index = index * 2 + 1;
        uint16_t child1_index = index * 2 + 2;

        // NULL for child0 or child1 means that the element does not exist
        node_t *child0 = child0_index < heap->size ? &buf[child0_index] : NULL;
        node_t *child1 = child1_index < heap->size ? &buf[child1_index] : NULL;

        if (child0 == NULL) break;

        // we only compare with the smallest child and swap if its smaller
        node_t compared;
        uint16_t compared_index = -1;

        if (child1 == NULL || child0->key < child1->key) {
            compared = *child0;
            compared_index = child0_index;
        } else {
            compared = *child1;
            compared_index = child1_index;
        }

        // no need to do any more swaps, break
        if (swapped.key < compared.key) break;

        // the child has a lower key so we swap
        node_t tmp = buf[compared_index];
        buf[compared_index] = buf[index];
        buf[index] = tmp;

        index = compared_index;
    }

    return value;
}

void free_heap(heap_t *heap) {
    free(heap->buf);
    free(heap);
}
