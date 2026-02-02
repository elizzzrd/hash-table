#pragma once

#define MAX_NAME 256
#define TABLE_SIZE 10

typedef struct 
{
    char * name;
    int age;
} Person_t;

unsigned int hash(char * name);
void print_hash_table(Person_t ** hash_table, int size);
Person_t ** init_hash_table(int size);
void destroy_hash_table(Person_t ** hash_table, int size);
bool insert_hash_table(Person_t ** hash_table, Person_t * person, int size);
Person_t * lookup_hash_table(Person_t ** hash_table, char * name);
Person_t * delete_item_hash_table(Person_t ** hash_table, char * name);