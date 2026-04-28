#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <stdio.h>
#include <stdint.h>


typedef char * Elem_t;
typedef uint64_t (*hash_func_t)(const Elem_t);


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

#endif