#include <assert.h>
#include <stdlib.h>

#include "doubly-linked-list.h"
#include "utils.h"

struct _Node {
  Node *next;
  Node *prev;
  void *data;
};

struct _DoublyLinkedList {
  Node            *head;
  Node            *tail;
  long             length;
  DataCompareFunc  compare;
  DataDestroyFunc  destroy;
};

static Node *
node_new (void *data)
{
  Node *node;

  node = malloc (sizeof (Node));
  DIE (node == NULL, "malloc");

  node->next = node->prev = NULL;
  node->data = data;

  return node;
}

static void
doubly_linked_list_remove_existing_node (DoublyLinkedList *list,
                                         Node             *node)
{
  /* Since this is a private function, passing
   * NULL values is the programmer's fault. */
  assert (list != NULL);
  assert (node != NULL);

  /* Update the head and tail pointers accordingly. */
  if (node == list->head && node == list->tail)
    list->head = list->tail = NULL;
  else if (node == list->head)
    list->head = list->head->next;
  else if (node == list->tail)
    list->tail = list->tail->prev;

  /* Update the next pointer for the prev node. */
  if (node->prev != NULL)
    node->prev->next = node->next;

  /* Update the prev pointer for the next node. */
  if (node->next != NULL)
    node->next->prev = node->prev;

  /* Free the memory of the data stored inside the node. */
  if (list->destroy != NULL)
    list->destroy (node->data);

  /* Free the memory of the node. */
  free (node);

  /* Update the length of the list. */
  list->length--;
}

/**
 * doubly_linked_list_new_full:
 * @cmp_func: A function to compare the elements of the list. This function is
 *            passed the data from two nodes of the list and should return 0
 *            if they are equal, a negative value if the first one is lower
 *            than the second, or a positive value if the first one is greater
 *            than the second.
 * @destroy_func: A function to free the memory of the data stored inside the
 *                nodes of the list upon nodes removal or list destruction. Use
 *                NULL if there is no need for such a function.
 *
 * Creates a new empty list that uses the given comparison function to compare
 * its elements.
 *
 * Returns: The newly created list.
 */
DoublyLinkedList *
doubly_linked_list_new_full (DataCompareFunc cmp_func,
                             DataDestroyFunc destroy_func)
{
  DoublyLinkedList *list;

  list = malloc (sizeof (DoublyLinkedList));
  DIE (list == NULL, "malloc");

  list->head = list->tail = NULL;
  list->length = 0;
  list->compare = cmp_func;
  list->destroy = destroy_func;

  return list;
}

/**
 * doubly_linked_list_new:
 * @cmp_func: A function to compare the elements of the list. This function is
 *            passed the data from two nodes of the list and should return 0
 *            if they are equal, a negative value if the first one is lower
 *            than the second, or a positive value if the first one is greater
 *            than the second.
 *
 * Creates a new empty list that uses the given comparison function to compare
 * its elements.
 *
 * Returns: The newly created list.
 */
DoublyLinkedList *
doubly_linked_list_new (DataCompareFunc func)
{
  return doubly_linked_list_new_full (func, NULL);
}

/**
 * doubly_linked_list_length:
 * @list: A list.
 *
 * Returns the number of elements in the list.
 *
 * Returns: The number of elements in the list, or -1 if the list is NULL.
 */
long
doubly_linked_list_length (DoublyLinkedList *list)
{
  return list == NULL ? -1 : list->length;
}

/**
 * doubly_linked_list_prepend:
 * @list: A list.
 * @data: The data for the new element.
 *
 * Adds a new element to the head of the list.
 */
void
doubly_linked_list_prepend (DoublyLinkedList *list,
                            void             *data)
{
  Node *node;

  /* Sanity check. */
  if (list == NULL)
    return;

  /* Create a new node. */
  node = node_new (data);

  /* Set the new pointers accordingly. */
  if (list->length == 0) {
    list->head = list->tail = node;
  } else {
    list->head->prev = node;
    node->next = list->head;
    list->head = node;
  }

  /* Update the length of the list. */
  list->length++;
}

/**
 * doubly_linked_list_append:
 * @list: A list.
 * @data: The data for the new element.
 *
 * Adds a new element to the tail of the list.
 */
void
doubly_linked_list_append (DoublyLinkedList *list,
                           void             *data)
{
  Node *node;

  /* Sanity check. */
  if (list == NULL)
    return;

  /* Create a new node. */
  node = node_new (data);

  /* Set the new pointers accordingly. */
  if (list->length == 0) {
    list->head = list->tail = node;
  } else {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
  }

  /* Update the length of the list. */
  list->length++;
}

/**
 * doubly_linked_list_insert_at:
 * @list: A list.
 * @data: The data for the new element.
 * @position: The position to insert the element at, starting from 0. If this is
 *            negative or greater than the number of elements in the list, the
 *            new element will be added to the end of the list.
 *
 * Inserts a new element into the list at the given position.
 */
void
doubly_linked_list_insert_at (DoublyLinkedList *list,
                              void             *data,
                              int               position)
{
  Node *node;
  Node *tmp;
  int i;

  /* Sanity check. */
  if (list == NULL)
    return;

  /* In case of an invalid position, append to the end of the list. */
  if (position < 0 || position >= list->length) {
    doubly_linked_list_append (list, data);
    return;
  }

  /* If the position is 0, use the prepend function. */
  if (position == 0) {
    doubly_linked_list_prepend (list, data);
    return;
  }

  /* Create a new node with the given data. */
  node = node_new (data);

  /* Iterate over the list and retrieve the node at position - 1. */
  for (i = 0, tmp = list->head; i < position - 1; i++, tmp = tmp->next);

  /* Set the new pointers accordingly. */
  node->next = tmp->next;
  node->prev = tmp;
  tmp->next->prev = node;
  tmp->next = node;

  /* Update the length of the list. */
  list->length++;
}

/**
 * doubly_linked_list_reverse:
 * @list: A list.
 *
 * Reverses a list. It simply swaps the next and prev pointers of each node.
 */
void
doubly_linked_list_reverse (DoublyLinkedList *list)
{
  Node *node;
  Node *tmp;
  Node *head;
  Node *tail;

  /* Sanity check. */
  if (list == NULL)
    return;

  /* Save the initial head and tail. */
  head = list->head;
  tail = list->tail;

  /* Iterate over every node and swap next and prev pointers.
   * Since the pointers are swapped at every step, the increment
   * will be node->prev, and not node->next. */
  for (node = list->head; node != NULL; node = node->prev) {
    tmp = node->next;
    node->next = node->prev;
    node->prev = tmp;
  }

  /* Finally, reverse the head and the tail of the list. */
  list->head = tail;
  list->tail = head;
}

/**
 * doubly_linked_list_remove:
 * @list: A list.
 * @data: The data of the element to be removed.
 *
 * Removes an element from the list. If two or more nodes contain the same data,
 * only the first one is removed. If none of the nodes contain the data, the
 * list remains unchanged.
 *
 * Returns: TRUE if the removal was successful, FALSE otherwise.
 */
boolean
doubly_linked_list_remove (DoublyLinkedList *list,
                           void             *data)
{
  Node *node;

  /* Sanity check. */
  if (list == NULL || list->length == 0)
    return FALSE;

  /* Iterate over the list and compare the data stored in every node. */
  for (node = list->head; node != NULL; node = node->next) {
    /* Remove the first node that contains the given data. */
    if (list->compare (node->data, data) == 0) {
      doubly_linked_list_remove_existing_node (list, node);
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * doubly_linked_list_remove_all:
 * @list: A list.
 * @data: The data of the element to be removed.
 *
 * Removes all the elements of the list that contain the given data. Contrasts
 * with doubly_linked_list_remove() which removes only the first node matching
 * the given data.
 *
 * Returns: TRUE if the removal was successful, FALSE otherwise.
 */
boolean
doubly_linked_list_remove_all (DoublyLinkedList *list,
                               void             *data)
{
  boolean retval;

  /* Sanity check. Don't test data against NULL, since that will cause
   * values such as the number zero to be considered as invalid. */
  if (list == NULL)
    return FALSE;

  /* Get the return value of the first remove call. */
  retval = doubly_linked_list_remove (list, data);

  /* While there are nodes that contains the same data, remove them. */
  while (doubly_linked_list_remove (list, data) == TRUE);

  return retval;
}

/**
 * doubly_linked_list_remove_at:
 * @list: A list.
 * @position: The position of the element to be removed, starting from 0. If
 *            this is greater than or equal to the number of the elements in the
 *            list, then no element is removed.
 *
 * Removes the element of the list at the given position.
 *
 * Returns: TRUE if the removal was successful, FALSE otherwise.
 */
boolean
doubly_linked_list_remove_at (DoublyLinkedList *list,
                              unsigned int      position)
{
  Node *node;
  unsigned int i;

  /* Sanity check. */
  if (list == NULL || position >= list->length)
    return FALSE;

  /* Retrieve the node at the given position. */
  for (i = 0, node = list->head; i < position; i++, node = node->next);

  /* Remove the node from the list. */
  doubly_linked_list_remove_existing_node (list, node);

  return TRUE;
}


/**
 * doubly_linked_list_get:
 * @list: A list.
 * @position: The index of the element. If this is greater than or equal to the
 *            number of the elements in the list, NULL is returned.
 *
 * Gets the data of the element at the given position.
 *
 * Returns: The element's data, or NULL if the index is off the end of the list.
 */
void *
doubly_linked_list_get (DoublyLinkedList *list,
                        unsigned int      position)
{
  Node *node;
  unsigned int i;

  /* Sanity check. */
  if (list == NULL || position >= list->length)
    return NULL;

  /* Retrieve the node at the given position. */
  for (i = 0, node = list->head; i < position; i++, node = node->next);

  return node->data;
}

/**
 * doubly_linked_list_index_of:
 * @list: A list.
 * @data: The data to find.
 *
 * Gets the index of the element containing the given data (starting from 0).
 * If two elements contain the same data, only the index of the first one is
 * returned.
 *
 * Returns: The index of the element containing the data, or -1 if the data is
 *          not found.
 */
int
doubly_linked_list_index_of (DoublyLinkedList *list,
                             void             *data)
{
  Node *node;
  int index;

  /* Sanity check. Don't test data against NULL, since that will cause
   * values such as the number zero to be considered as invalid. */
  if (list == NULL)
    return -1;

  /* Iterate over the list and return the index where the data is found. */
  for (node = list->head, index = 0; node != NULL; node = node->next, index++)
    if (list->compare (node->data, data) == 0)
      return index;

  return -1;
}

/**
 * doubly_linked_list_destroy:
 * @list: A list.
 *
 * Destroys all nodes in the list and frees the memory of both the nodes and the
 * list itself. If the data stored in the nodes is dynamically allocated, it
 * should be freed before calling this function. Another option to free it is to
 * create the list with by calling doubly_linked_list_new_full() and passing a
 * destroy function. In the latter case, the supplied destroy function will be
 * called on all data during the destruction phase.
 */
void
doubly_linked_list_destroy (DoublyLinkedList *list)
{
  Node *node;
  Node *tmp;

  /* Sanity check. */
  if (list == NULL)
    return;

  /* Keep removing the head of the list until the list becomes empty. */
  while (list->length > 0)
    doubly_linked_list_remove_existing_node (list, list->head);

  /* Free the memory of the list. */
  free (list);

  /* Set the list pointer to null. */
  list = NULL;
}
