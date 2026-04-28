#ifndef HASH_TABLE_H
#define HASH_TABLE_H


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "hash_functions.h"
#include "text.h"


#define HT_MAX_SIZE 40000
#define LIST_SIZE_MAX 25


#define LOG_MESSAGE(format, ...) log_message(__FILE__, __LINE__, format, ##__VA_ARGS__)
bool log_initt(void);
void log_close(void);
void log_message(const char * file, int line,const char * format, ...);



#define HT_CHECK(ht_ptr) \
    do { \
        HT_Err err = hashtable_validate(ht_ptr); \
        if (err != HT_OK) { \
            ht_dump(ht_ptr, err, __FILE__, __LINE__);  \
        } \
        return err; \
    } while (0)
    

typedef enum errors_ht
{
    HT_OK,

    HT_NULL_PTR,
    HT_MEMORY_ALLOCATION_ERROR,
    HT_INVALID_INSERT,
    HT_INVALID_DELETE,
    HT_OVERFLOW
} HT_Err;


typedef struct node
{
    Elem_t word;
    union
    {
        int size;               // word occurrence
        int list_size;          // for list_head
    };
    struct node * prev;
    struct node * next;
} Node_t;



typedef struct 
{
    Node_t ** buckets;
    int * bucket_sizes;

    size_t size;
    size_t capacity;
    hash_func_t hash;
} Hashtable_t;


bool hashtable_init(Hashtable_t * ht, size_t capacity, hash_func_t hash);
HT_Err hashtable_insert(Hashtable_t * ht, const Elem_t word);
void build_hashtable(Hashtable_t * ht, StringArray_t * arr);
int hashtable_get_item_count(Hashtable_t * ht, Elem_t key_word);
bool hashtable_delete_item(Hashtable_t * ht, const Elem_t key_word);
void hashtable_destroy(Hashtable_t * ht);
bool hashtable_find_item(Hashtable_t * ht, const Elem_t key_word);


size_t build_hashtable_from_file(Hashtable_t * ht, const char * filename);
void export_to_csv(Hashtable_t * ht, const char * filename);

void dump_ht(const Hashtable_t * ht, const char * filename);
bool hashtable_validate(const Hashtable_t * ht);
const char * ht_error_string(HT_Err error);

#endif