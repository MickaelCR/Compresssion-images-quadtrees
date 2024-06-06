#include "../../include/model/Heap.h"
#include <stdlib.h>
#include <stdio.h>

static void swap(quadnode **a, quadnode **b) {
    quadnode *temp = *a;
    *a = *b;
    *b = temp;
}

max_heap* create_max_heap(int capacity) {
    max_heap *heap = malloc(sizeof(max_heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->nodes = malloc(capacity * sizeof(quadnode *));
    return heap;
}

void insert_max_heap(max_heap *heap, quadnode *node) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->nodes = realloc(heap->nodes, heap->capacity * sizeof(quadnode *));
    }

    heap->nodes[heap->size] = node;
    int current = heap->size;
    heap->size++;

    while (current > 0 && heap->nodes[current]->error > heap->nodes[(current - 1) / 2]->error) {
        swap(&heap->nodes[current], &heap->nodes[(current - 1) / 2]);
        current = (current - 1) / 2;
    }
}

quadnode* extract_max(max_heap *heap) {
    if (heap->size == 0) {
        return NULL;
    }

    quadnode *max = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;

    int current = 0;
    while (current < heap->size) {
        int left = 2 * current + 1;
        int right = 2 * current + 2;
        int largest = current;

        if (left < heap->size && heap->nodes[left]->error > heap->nodes[largest]->error) {
            largest = left;
        }
        if (right < heap->size && heap->nodes[right]->error > heap->nodes[largest]->error) {
            largest = right;
        }
        if (largest != current) {
            swap(&heap->nodes[current], &heap->nodes[largest]);
            current = largest;
        } else {
            break;
        }
    }

    return max;
}

void free_max_heap(max_heap *heap) {
    free(heap->nodes);
    free(heap);
}
