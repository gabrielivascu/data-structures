#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "doubly-linked-list.h"

static int
integer_comparison_func (const void *a,
                         const void *b)
{
  return ((intptr_t) a) - ((intptr_t) b);
}

int main (int argc, char **argv)
{
  DoublyLinkedList *list;

  list = doubly_linked_list_new (integer_comparison_func);
  assert (doubly_linked_list_length (list) == 0);

  doubly_linked_list_append (list, (void *) (intptr_t) 5);
  assert (doubly_linked_list_length (list) == 1);
  assert ((intptr_t) doubly_linked_list_get (list, 0) == 5);

  doubly_linked_list_prepend (list, (void *) (intptr_t) 4);
  assert (doubly_linked_list_length (list) == 2);
  assert ((intptr_t) doubly_linked_list_get (list, 0) == 4);

  doubly_linked_list_prepend (list, (void *) (intptr_t) 2);
  doubly_linked_list_prepend (list, (void *) (intptr_t) 1);
  doubly_linked_list_append (list, (void *) (intptr_t) 2);
  assert (doubly_linked_list_length (list) == 5);
  assert ((intptr_t) doubly_linked_list_get (list, 1) == 2);
  assert ((intptr_t) doubly_linked_list_index_of (list, (void *) (intptr_t) 5) == 3);

  doubly_linked_list_insert_at (list, (void *) (intptr_t) 2, 2);
  doubly_linked_list_insert_at (list, (void *) (intptr_t) 3, 0);
  doubly_linked_list_insert_at (list, (void *) (intptr_t) 9, 6);
  assert ((intptr_t) doubly_linked_list_get (list, 7) == 2);
  assert ((intptr_t) doubly_linked_list_index_of (list, (void *) (intptr_t) 3) == 0);
  assert ((intptr_t) doubly_linked_list_index_of (list, (void *) (intptr_t) 9) == 6);

  assert (doubly_linked_list_remove (list, (void *) (intptr_t) 4) == TRUE);
  assert ((intptr_t) doubly_linked_list_index_of (list, (void *) (intptr_t) 5) == 4);
  assert (doubly_linked_list_remove (list, (void *) (intptr_t) 5) == TRUE);
  assert (doubly_linked_list_remove (list, (void *) (intptr_t) 5) == FALSE);

  assert (doubly_linked_list_remove_all (list, (void *) (intptr_t) 2) == TRUE);
  assert (doubly_linked_list_length (list) == 3);
  assert ((intptr_t) doubly_linked_list_get (list, 2) == 9);
  assert (doubly_linked_list_remove_at (list, 2) == TRUE);
  assert ((intptr_t) doubly_linked_list_index_of (list, (void *) (intptr_t) 9) == -1);

  doubly_linked_list_reverse (list);
  assert ((intptr_t) doubly_linked_list_get (list, 0) == 1);
  assert ((intptr_t) doubly_linked_list_get (list, 1) == 3);

  doubly_linked_list_destroy (list);

  return 0;
}
