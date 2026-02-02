#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "open_hash.h"

Person_t people[]
{
    {.name = strdup("Jacob"),     .value = 43735909, .next = NULL},
    {.name = strdup("Alice"),     .value = 24938495, .next = NULL},
    {.name = strdup("Bob"),       .value = 92839843, .next = NULL},
    {.name = strdup("Katherine"), .value = 59834959, .next = NULL},
    {.name = strdup("Tim"),       .value = 23894034, .next = NULL},
    {.name = strdup("Sarah"),     .value = 19329743, .next = NULL},
    {.name = strdup("Thomas"),    .value = 40838958, .next = NULL},
};
int people_size = sizeof(people)/sizeof(people[0]);

Person_t prs1 = people[2];
Person_t prs2 = {.name = strdup("Amber"), .value = 48888958, .next = NULL};

void cleanup_people(Person_t* arr, int size) 
{
    for (int i = 0; i < size; i++) 
        free(arr[i].name);
}


int main(void)
{
    int size = 10;
    Person_t ** hash_table = init_hashtable(size);
    if (!hash_table)
        return -1;
    
    print_hashtable(hash_table, size);

    for (int i = 0; i < people_size; i++)
        insert_item(hash_table, &people[i], size);
    print_hashtable(hash_table, size);

    printf("Searching for: %s\n", prs1.name);
    Person_t * tmp1 = get_item(hash_table, prs1.name, size);
    if (!tmp1)
        printf("%s not found\n", prs1.name);
    else
        printf("Found %s.\n", tmp1->name);

    printf("Searching for: %s\n", prs2.name);
    Person_t * tmp2 = get_item(hash_table, prs2.name, size);
    if (!tmp2)
        printf("%s not found\n", prs2.name);
    else
        printf("Found %s.\n", tmp2->name);
        
    printf("Deleting item %s\n", prs1.name);
    Person_t * tmp3 = erase_item(hash_table, prs1.name, size);
    if (!tmp3)
        printf("Item was not found\n");
    else
    {
        free(tmp3->name);
        free(tmp3);
    }
    print_hashtable(hash_table, size);


    cleanup_people(people, people_size);
    destroy_hashtable(hash_table, size);
    return 0;
}