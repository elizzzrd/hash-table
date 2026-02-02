#pragma once

typedef struct node
{
    char * name;
    unsigned int value;
    struct node * next;
} Person_t;


unsigned int hash(char * name, int size);

void print_hashtable(Person_t ** hash_table, int size);

Person_t ** init_hashtable(int size);
void destroy_hashtable(Person_t ** hash_table, int size);

bool insert_item(Person_t ** hash_table, Person_t * person, int size);
Person_t * get_item(Person_t ** hash_table, char * name, int size);
Person_t * erase_item(Person_t ** hash_table, char * name, int size);

int find_mutually_inverse_num(int n);