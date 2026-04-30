#ifndef LIST_H
#define LIST_H

#define LIST_SIZE 12

typedef int List_Err; 
typedef int Item;

typedef struct 
{
    int * prev;
    int * data;
    int * next;
    int head;
    int tail;
    int free_head;
} list_t;

typedef struct 
{
    Item value;
    int next_index;
    int prev_index;
} List_Elem;

typedef enum errors
{
    LIST_OK,

    LIST_NULL_PTR,
    LIST_MEMORY_ALLOCATION_ERROR,
    LIST_INVALID_INSERT,
    LIST_INVALID_DELETE,
    LIST_OVERFLOW,
} List_Err;

const char * list_error_string(List_Err error);


List_Err list_init(list_t * list);
List_Err list_destroy(list_t * list);

List_Err list_insert(list_t * list, int index, Item value);

List_Err delete_elem(list_t * list, int deleted_index);
List_Err get_elem(list_t * list, int index);

#endif