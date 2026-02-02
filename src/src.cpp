#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "src.h"


unsigned int hash(char * name)
{
    assert(name);

    int len = strlen(name);
    unsigned int hash_value = 0;
    for (int i = 0; i < len; i++)
    {
        hash_value += name[i];
        hash_value *= name[i];
    } 
    return (hash_value % TABLE_SIZE);
}


Person_t ** init_hash_table(int size)
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

void destroy_hash_table(Person_t ** hash_table, int size)
{
    assert(hash_table);

    for (int i = 0; i < size; i++)
    {
        if (hash_table[i])
        {
            free(hash_table[i]->name);
            free(hash_table[i]);
            hash_table[i] = NULL;
        }
    }

    free(hash_table);
}

void print_hash_table(Person_t ** hash_table, int size)
{
    assert(hash_table);

    printf("\n============HASH TABLE============\n");
    for (int i = 0; i < size; i++)
    {
        if (!hash_table[i])
            printf("\t%d\t---", i);
        else
            printf("\t%d\t%s", i, hash_table[i]->name);
        printf("\n");
    }
    printf("\n");
}


bool insert_hash_table(Person_t ** hash_table, Person_t * person, int size)
{
    assert(hash_table && person);

    if (!person)    return false;
    int index = hash(person->name);

    if (hash_table[index] != NULL)
        return false; //collision
    else
    {
        hash_table[index] = (Person_t *) malloc(sizeof(Person_t));
        if (!hash_table[index])     return false;

        hash_table[index]->age = person->age;
        hash_table[index]->name = strdup(person->name);

        if (hash_table[index]->name == NULL) 
        {
            free(hash_table[index]);
            hash_table[index] = NULL;
            return false;
        }
    }

    return true;
}

Person_t * lookup_hash_table(Person_t ** hash_table, char * name)
{
    assert(name && hash_table);

    int index = hash(name);
    if (hash_table[index] != NULL && strcmp(hash_table[index]->name, name) == 0)
        return hash_table[index];
    else 
        return NULL;
}

Person_t * delete_item_hash_table(Person_t ** hash_table, char * name)
{
    assert(name && hash_table);

    int index = hash(name);
    if (hash_table[index] != NULL && strcmp(hash_table[index]->name, name) == 0)
    {
        Person_t * tmp = hash_table[index];
        free(hash_table[index]->name);
        free(hash_table[index]);
        hash_table[index] = NULL;
        return tmp;
    }
    else 
        return NULL;
}


// Collisions: open addressing / external chaining