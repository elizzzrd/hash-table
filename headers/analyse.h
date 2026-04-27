#pragma once

#include "hash_table.h"

typedef struct 
{
    int collisions;
    int max_chain;
    double average_chain;
    double variance;
    double running_time;
} HashStats_t;

HashStats_t analyze_table(Hashtable_t * ht);

void print_table_row(const char *name, HashStats_t s);
void print_table_header();

void test_one_ht_size(int ht_size, HashStats_t * stats, const char * data);