#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


#include "hash_table.h"
#include "benchmark.h"


bool hashtable_init(Hashtable_t * ht, size_t capacity, hash_func_t hash)
{
    assert(ht && capacity > 0);

    ht->buckets = (Node_t **) calloc(capacity, sizeof(Node_t *));
    if (ht->buckets == NULL)
        return false;

    ht->bucket_sizes = (int *) calloc(capacity, sizeof(int));
    if (ht->bucket_sizes == NULL)
    {
        free(ht->buckets);
        return false;
    }
    
    ht->capacity = capacity;
    ht->size = 0;
    ht->hash = hash;

    return true;
}


void hashtable_destroy(Hashtable_t * ht)
{
    assert(ht);

    for (size_t i = 0; i < ht->capacity; i++)
    {
        Node_t * current = ht->buckets[i];
        while (current != NULL)
        {
            Node_t * to_delete = current;
            current = current->next;
            
            free(to_delete->word);
            free(to_delete);
        }
    }

    free(ht->buckets);
    free(ht->bucket_sizes);
    ht->buckets = NULL;
    ht->size = 0;
    ht->capacity = 0;
    ht->hash = NULL;
}


HT_Err hashtable_insert(Hashtable_t * ht, const Elem_t word)
{
    assert(ht && word);

    // if (ht->size > ht->capacity)
    // {
    //     fprintf(stderr, "hashtable is full\n");
    //     return HT_OVERFLOW;
    // }    

    size_t index = (ht->hash(word)) % ((uint64_t)(ht->capacity));
    Node_t * current = ht->buckets[index];

    // searching for word in bucket
    while (current != NULL)
    {
        if (strcmp(current->word, word) == 0)
        {
            current->size++;
            return HT_OK;
        }
        current = current->next;
    }
    
    // not found -> create new node
    Node_t * new_node = (Node_t *)calloc(1, sizeof(Node_t));
    if (!new_node)
    {
        fprintf(stderr, "memory allocation error\n");
        return HT_MEMORY_ALLOCATION_ERROR;
    }

    new_node->word = strdup(word);
    if (!new_node->word)
    {
        fprintf(stderr, "memory allocation error\n");
        free(new_node);
        return HT_MEMORY_ALLOCATION_ERROR;
    }

    new_node->size = 1;
    new_node->next = ht->buckets[index];
    new_node->prev = NULL;
    
    if (ht->buckets[index] != NULL) {
        ht->buckets[index]->prev = new_node;
    }
    
    ht->buckets[index] = new_node;
    ht->bucket_sizes[index]++;  
    ht->size++;

    return HT_OK;
}


void build_hashtable(Hashtable_t * ht, StringArray_t * arr)
{
    assert(ht && arr);

    for (size_t i = 0; i < arr->size; i++)
        if (hashtable_insert(ht, arr->data[i]) != HT_OK)
            fprintf(stderr, "warning: failed to insert '%s' at index %zu\n", arr->data[i], i);
}


int hashtable_get_item_count(Hashtable_t * ht, Elem_t key_word)
{
    assert(key_word && ht);

    size_t index = ht->hash(key_word) % ((uint64_t)(ht->capacity));
    Node_t * current = ht->buckets[index];
    
    while (current != NULL)
    {
        if (strcmp(current->word, key_word) == 0)
        {
            return current->size;
        }
        current = current->next;
    }
    
    return 0;
}


bool hashtable_delete_item(Hashtable_t * ht, const Elem_t key_word)
{
    assert(key_word && ht);

    size_t index = ht->hash(key_word) % ((uint64_t)(ht->capacity));
        
    Node_t * current = ht->buckets[index];
    
    while (current != NULL)
    {
        if (strcmp(current->word, key_word) == 0)
        {
            if (current->prev != NULL)
            {
                current->prev->next = current->next;
            }
            else
            {
                ht->buckets[index] = current->next;
            }

            if (current->next != NULL)
            {
                current->next->prev = current->prev;
            }
              
            free(current->word);
            free(current);

            ht->size--;
            ht->bucket_sizes[index]--;
            return true;
        }
        
        current = current->next;
    }
    return false;
}



bool hashtable_find_item(Hashtable_t * ht, const Elem_t key_word)
{
    assert(ht);
    if (!key_word) {
        LOG_MESSAGE("Warning: NULL key_word in hashtable_find_item\n");
        return false;
    }

    size_t index = ht->hash(key_word) % ((uint64_t)(ht->capacity));
    Node_t * current = ht->buckets[index];

    while (current != NULL)
    {
        if (strcmp(current->word, key_word) == 0)
            return true;
        current = current->next;
    }

    return false;
}