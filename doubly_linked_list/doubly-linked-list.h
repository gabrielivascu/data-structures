#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#define FALSE (0)
#define TRUE  (!FALSE)

typedef int boolean;

typedef struct _Node Node;
typedef struct _DoublyLinkedList DoublyLinkedList;

typedef int  (*DataCompareFunc) (const void *,
                                 const void *);
typedef void (*DataDestroyFunc) (void *);

DoublyLinkedList *doubly_linked_list_new        (DataCompareFunc cmp_func);
DoublyLinkedList *doubly_linked_list_new_full   (DataCompareFunc cmp_func,
                                                 DataDestroyFunc destroy_func);
long              doubly_linked_list_length     (DoublyLinkedList *list);
void              doubly_linked_list_prepend    (DoublyLinkedList *list,
                                                 void             *data);
void              doubly_linked_list_append     (DoublyLinkedList *list,
                                                 void             *data);
void              doubly_linked_list_insert_at  (DoublyLinkedList *list,
                                                 void             *data,
                                                 int               position);
boolean           doubly_linked_list_remove     (DoublyLinkedList *list,
                                                 void             *data);
boolean           doubly_linked_list_remove_all (DoublyLinkedList *list,
                                                 void             *data);
boolean           doubly_linked_list_remove_at  (DoublyLinkedList *list,
                                                 unsigned int      position);
void             *doubly_linked_list_get        (DoublyLinkedList *list,
                                                 unsigned int      position);
int               doubly_linked_list_index_of   (DoublyLinkedList *list,
                                                 void             *data);
void              doubly_linked_list_reverse    (DoublyLinkedList *list);
void              doubly_linked_list_destroy    (DoublyLinkedList *list);

#endif
