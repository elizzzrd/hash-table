#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "open_hash.h"

// Collisions: 
// open addressing 

unsigned int hash(char * name, int size)
{
    assert(name);

    int length = strlen(name);
    unsigned int hash_value = 0;
    int a = 3;
    for (int i = 0; i < length; i++)
    {
        hash_value += name[i] * a;
    } 
    return (hash_value - 'a' + 1) % size;
}


Person_t ** init_hashtable(int size)
{
    Person_t ** hash_table = (Person_t **) calloc(size, sizeof(Person_t*));
    if (!hash_table)
    {
        fprintf(stderr, "memory allocation error");
        return NULL;
    }

    for (int i = 0; i < size; i++)
        hash_table[i] = NULL;

    return hash_table;
}


void destroy_hashtable(Person_t ** hash_table, int size)
{
    assert(hash_table);

    for (int i = 0; i < size; i++)
    {
        Person_t *current = hash_table[i];
        while (current != NULL)
        {
            Person_t *to_delete = current;
            current = current->next;
            
            free(to_delete->name);
            free(to_delete);
        }
        hash_table[i] = NULL;
    }

    free(hash_table);
}


void print_hashtable(Person_t ** hash_table, int size)
{
    assert(hash_table);

    printf("\n============HASH TABLE============\n");
    for (int i = 0; i < size; i++)
    {
        if (!hash_table[i])  
        {
            printf("\t%d\t---\n", i);
        }
        else
        {
            Person_t *current = hash_table[i];
            printf("\t%d\t", i);
            
            while (current != NULL)
            {
                printf("%-10s\t%u", current->name, current->value);
                current = current->next;  
                
                if (current != NULL)
                    printf(" -> ");
            }
            printf("\n");
        }
    }
    printf("\n");
}


bool insert_item(Person_t ** hash_table, Person_t * person, int size)
{
    assert(hash_table && person);

    int index = hash(person->name, size);
    

    Person_t *current = hash_table[index];
    while (current != NULL)
    {
        if (strcmp(current->name, person->name) == 0)
            return false;  // Дубликат
        current = current->next;
    }
    

    Person_t * new_node = (Person_t *)malloc(sizeof(Person_t));
    if (!new_node)
    {
        fprintf(stderr, "memory allocation error\n");
        return false;
    }

    new_node->name = strdup(person->name);
    if (!new_node->name)
    {
        fprintf(stderr, "memory allocation error\n");
        free(new_node);
        return false;
    }
    new_node->value = person->value;
    new_node->next = NULL;
    
    if (hash_table[index] == NULL)
    {
        hash_table[index] = new_node;
    }
    else
    {
        current = hash_table[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
    
    return true;
}



Person_t * get_item(Person_t ** hash_table, char * key, int size)
{
    assert(key && hash_table);

    int index = hash(key, size);
    Person_t * current = hash_table[index];
    
    while (current != NULL)
    {
        if (strcmp(current->name, key) == 0)
        {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}



Person_t * erase_item(Person_t ** hash_table, char * key, int size)
{
    assert(key && hash_table);

    int index = hash(key, size);
    
    if (hash_table[index] == NULL)
        return NULL;
    
    Person_t * current = hash_table[index];
    Person_t * prev = NULL;
    
    while (current != NULL)
    {
        if (strcmp(current->name, key) == 0)
        {
            if (prev == NULL)
            {
                hash_table[index] = current->next;
            }
            else
            {
                prev->next = current->next;
            }
        
            current->next = NULL;  
            return current;
        }
        
        prev = current;
        current = current->next;
    }
    
    return NULL;
}



