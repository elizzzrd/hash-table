#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "hash_table.h"
#include "list.h"


bool hashtable_init(Hashtable_t * ht, size_t capacity)
{
    assert(ht && capacity > 0);

    ht->buckets = (Node_t **) calloc(capacity, sizeof(Node_t *));
    if (ht->buckets == NULL)
        return false;
    
    ht->capacity = capacity;
    ht->size = 0;
    ht->hash = NULL;

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
    ht->buckets = NULL;
    ht->size = 0;
    ht->capacity = 0;
    ht->hash = NULL;
}


bool hashtable_insert(Hashtable_t * ht, const Elem_t word, hash_func_t hash)
{
    assert(ht && word);

    if (ht->size >= ht->capacity)
    {
        fprintf(stderr, "hashtable is full");
        return false;
    }    

    int index = hash(word) % (ht->capacity);
    Node_t * current = ht->buckets[index];

    // searching for word in bucket
    while (current != NULL)
    {
        if (strcmp(current->word, word) == 0)
        {
            current->size++;
            return true;
        }
        current = current->next;
    }
    
    // not found -> create new node
    Node_t * new_node = (Node_t *)calloc(1, sizeof(Node_t));
    if (!new_node)
    {
        fprintf(stderr, "memory allocation error\n");
        return false;
    }

    new_node->word = strdup(word);
    if (!new_node->word)
    {
        fprintf(stderr, "memory allocation error\n");
        free(new_node);
        return false;
    }

    new_node->size = 1;
    new_node->prev = NULL;
    new_node->next = ht->buckets[index];
    

    // if not first elem in bucket
    if (ht->buckets[index] != NULL)
    {
        ht->buckets[index]->prev = new_node;
    }

    ht->buckets[index] = new_node;
    ht->size++;

    return true;
}


void build_hashtable(Hashtable_t * ht, StringArray_t * arr, hash_func_t hash)
{
    assert(ht && arr);

    for (size_t i = 0; i < arr->size; i++)
        if (!hashtable_insert(ht, arr->data[i], hash))
            fprintf(stderr, "warning: failed to insert '%s' at index %zu\n", arr->data[i], i);
}


int hashtable_get_item_count(Hashtable_t * ht, Elem_t key_word, hash_func_t hash)
{
    assert(key_word && ht);

    int index = hash(key_word) % (ht->capacity);
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


bool hashtable_delete_item(Hashtable_t * ht, const Elem_t key_word, hash_func_t hash)
{
    assert(key_word && ht);

    int index = hash(key_word) % (ht->capacity);
        
    Node_t * current = ht->buckets[index];
    Node_t * prev = NULL;
    
    while (current != NULL)
    {
        if (strcmp(current->word, key_word) == 0)
        {
            if (prev == NULL)
            {
                // delete first node in bucket
                ht->buckets[index] = current->next;
            }
            else
            {
                current->prev->next = current->next;
            }

            if (current->next != NULL)
                current->next->prev = current->prev;
              
            free(current->word);
            free(current);

            ht->size--;
            return true;
        }
        
        current = current->next;
    }
    return false;
}



