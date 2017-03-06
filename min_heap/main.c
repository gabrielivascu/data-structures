#include <stdio.h>

#include "min-heap.h"

int main(int argc, char **argv)
{
  int v[] = {8, 4, 2, 5, 1, 3, 7, 6};
  int n = sizeof (v) / sizeof (v[0]);

  min_heap_sort_array (v, n);

  for (int i = 0; i < n; i++)
    printf("%d ", v[i]);
  printf("\n");

  return 0;
}
