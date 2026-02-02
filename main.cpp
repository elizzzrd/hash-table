#include <stdio.h>
#include <stdbool.h>
#include "src.h"

Person_t people[]
{
    {.name = "Jacob",     .age = 19},
    {.name = "Alice",     .age = 30},
    {.name = "Bob",       .age = 16},
    {.name = "Katherine", .age = 25},
    {.name = "Tim",       .age = 10},
    {.name = "Sarah",     .age = 13},
    {.name = "Thomas",    .age = 12},
};


int main(void)
{
    Person_t ** hash_table = init_hash_table(TABLE_SIZE);
    if (!hash_table)
        return -1;
    
    print_hash_table(hash_table, TABLE_SIZE);

    for (int i = 0; i < (sizeof(people)/sizeof(people[0])); i++)
    {
        insert_hash_table(hash_table, &people[i], TABLE_SIZE);
    }
    print_hash_table(hash_table, TABLE_SIZE);

    Person_t * tmp1 = lookup_hash_table(hash_table, "Thomas");
    if (!tmp1)
        printf("Thomas not found\n");
    else
        printf("Found %s.\n", tmp1->name);

    Person_t * tmp2 = lookup_hash_table(hash_table, "Alice");
    if (!tmp2)
        printf("Alice not found\n");
    else
    {
        printf("Found %s.\n", tmp2->name);
        printf("Delete %s.\n", tmp2->name);
        delete_item_hash_table(hash_table, tmp2->name);
        print_hash_table(hash_table, TABLE_SIZE);
    }

    
    

    destroy_hash_table(hash_table, TABLE_SIZE);
    return 0;
}