#include <stdio.h>
#include <stdlib.h>

#include "min-heap.h"

/* Given a node with id == k (k = 0...n) then:
 * the id of the left child is 2k + 1.
 * the id of the right child is 2k + 2.
 * the id of the parent is (k + 1) / 2 - 1.
 */

#define GET_PARENT_ID(ID) ((ID + 1) / 2 - 1)

struct _MinHeap {
  int max_size;
  int size;
  int *elems;
};

static void
min_heap_heapify (MinHeap *heap,
                  int      id)
{
  int left = 2 * id + 1;
  int right = 2 * id + 2;
  int min = id;

  /* Get min (node, left child). */
  if (left < heap->size && heap->elems[left] < heap->elems[id])
    min = left;

  /* Get min (right child, min (node, left child)). */
  if (right < heap->size && heap->elems[right] < heap->elems[min])
    min = right;

  /* If the current node is not lower than its children,
   * then filter it downwards until this condition is met. */
  if (min != id) {
    int tmp = heap->elems[id];
    heap->elems[id] = heap->elems[min];
    heap->elems[min] = tmp;

    /* Recursive call on the new id of the node. */
    min_heap_heapify (heap, min);
  }
}

MinHeap *
min_heap_new (int max_size)
{
  MinHeap *heap = malloc (sizeof (MinHeap));

  heap->max_size = max_size;
  heap->size = 0;
  heap->elems = malloc (max_size * sizeof (int));

  return heap;
}

MinHeap *
min_heap_new_from_array (int *array,
                         int  size)
{
  MinHeap *heap = min_heap_new (size);

  for (int i = 0; i < size; i++)
    heap->elems[i] = array[i];

  /* Set the heap size. */
  heap->size = size;

  /* Start from the parent of the last node (there is no point to heapify the
   * nodes on the last level) and heapify all the nodes up to the root. */
  for (int i = GET_PARENT_ID (size - 1); i >= 0; i--)
    min_heap_heapify (heap, i);

  return heap;
}

void
min_heap_free (MinHeap *heap)
{
  free (heap->elems);
  free (heap);
}

int
min_heap_get_size (MinHeap *heap)
{
  return heap->size;
}

int
min_heap_peek (MinHeap *heap)
{
  return heap->elems[0];
}

void
min_heap_insert (MinHeap *heap,
                 int      data)
{
  int i = heap->size++;
  int p = GET_PARENT_ID (i);

  /* Add the new node to last position. */
  heap->elems[i] = data;

  /* While the new node is lower than its parent, swap them.  */
  while (i > 0 && data < heap->elems[p]) {
    int tmp = heap->elems[i];
    heap->elems[i] = heap->elems[p];
    heap->elems[p] = tmp;

    i = p;
    p = GET_PARENT_ID (p);
  }
}

int
min_heap_pop (MinHeap *heap)
{
  int retval;

  if (heap->size == 0)
    return -1;

  /* Save the root (the lowest element). */
  retval = heap->elems[0];

  /* Move the last element to the root. */
  heap->elems[0] = heap->elems[heap->size - 1];

  /* Update the heap size. */
  heap->size--;

  /* Preserve the heap condition. */
  min_heap_heapify (heap, 0);

  return retval;
}

void
min_heap_sort_array (int *array,
                     int  size)
{
  /* Heapsort: build a min heap from the the initial array, then pop every
   * element from the heap and write it back in the initial array. */

  MinHeap *heap = min_heap_new_from_array (array, size);

  for (int i = size - 1; i >= 0; i--)
    array[size - 1 - i] = min_heap_pop (heap);

  min_heap_free (heap);
}
