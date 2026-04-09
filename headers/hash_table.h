#pragma once
#include <stdio.h>
#include <stdint.h>
#include "list.h"
#include "text/text.h"

#define HT_MAX_SIZE 40000
typedef uint64_t (*hash_func_t)(const Elem_t);

typedef struct 
{
    Node_t ** buckets;
    size_t size;
    size_t capacity;
    hash_func_t hash;
} Hashtable_t;


bool hashtable_init(Hashtable_t * ht, size_t capacity);
bool hashtable_insert(Hashtable_t * ht, const Elem_t word, hash_func_t hash);
void build_hashtable(Hashtable_t * ht, StringArray_t * arr, hash_func_t hash);
int hashtable_get_item_count(Hashtable_t * ht, Elem_t key_word, hash_func_t hash);
bool hashtable_delete_item(Hashtable_t * ht, const Elem_t key_word, hash_func_t hash);
void hashtable_destroy(Hashtable_t * ht);
