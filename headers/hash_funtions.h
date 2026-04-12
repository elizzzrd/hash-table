#pragma once

#include <stdio.h>
#include <stdint.h>
#include "hash_table.h"

typedef struct 
{
    int collisions;
    int max_chain;
    double average_chain;
    double variance;
    double running_time;
} HashStats_t;

typedef struct 
{
    const char * name;
    hash_func_t func;
} HashFunctionInfo;

extern HashFunctionInfo hash_functions[];
extern int hf_info_size;

uint64_t hash_const(const Elem_t name);
uint64_t hash_first_char(const Elem_t name);
uint64_t hash_word_len(const Elem_t word);
uint64_t hash_sum(const Elem_t word);
uint64_t hash_rotate(const Elem_t word);
uint64_t hash_crc32(const Elem_t word);

HashStats_t analyze_table(Hashtable_t * ht);
void print_table_row(const char *name, HashStats_t s);
void print_table_header();