#ifndef MAX_HEAP_H
#define MAX_HEAP_H

struct _quadnode_;
typedef struct _quadnode_ quadnode;

typedef struct {
    quadnode **nodes;
    int size;
    int capacity;
} max_heap;

max_heap* create_max_heap(int capacity);
void insert_max_heap(max_heap *heap, quadnode *node);
quadnode* extract_max(max_heap *heap);
void free_max_heap(max_heap *heap);

#endif // MAX_HEAP_H
