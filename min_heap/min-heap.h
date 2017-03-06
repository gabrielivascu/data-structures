#ifndef MIN_HEAP_H
#define MIN_HEAP_H

typedef struct _MinHeap MinHeap;

MinHeap *min_heap_new            (int max_size);
MinHeap *min_heap_new_from_array (int *array,
                                  int  size);
void     min_heap_free           (MinHeap *heap);
int      min_heap_get_size       (MinHeap *heap);
int      min_heap_peek           (MinHeap *heap);
void     min_heap_insert         (MinHeap *heap,
                                  int      data);
int      min_heap_pop            (MinHeap *heap);
void     min_heap_sort_array     (int *array,
                                  int  size);

#endif
