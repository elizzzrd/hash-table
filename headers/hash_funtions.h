#pragma once
#include <stdio.h>
#include <stdint.h>
#include "list.h"


typedef struct 
{
    size_t collisions;
    size_t max_chain;
    double average_chain;
} HashStats_t;




uint64_t hash_const(const Elem_t name);
uint64_t hash_first_char(const Elem_t name);
uint64_t hash_word_len(const Elem_t word);
