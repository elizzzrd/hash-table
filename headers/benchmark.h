#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#include "hash_table.h"
#include "text.h"


#define MAX_BENCHMARK_COMP_NUM 2000
#define DEFAULT_BENCHMARK_TABLE_SIZE 3001

typedef enum {
    BENCHMARK_STATUS_OK = 0,
    BENCHMARK_STATUS_FAIL = 1,
    BENCHMARK_STATUS_FILE_ERROR = 2,
    BENCHMARK_STATUS_MEMORY_ERROR = 3
} Benchmark_Status_t;

typedef struct 
{
    const char * hash_func_name;
    size_t table_size;
    size_t words_count;
    
    uint64_t total_cycles;
    double total_time_ms;
    double avg_cycles_per_find;
    double avg_time_per_find;
    
    size_t total_finds;
    size_t found_count;
    size_t not_found_count;
} BenchmarkResult_t;

void print_benchmark_results(BenchmarkResult_t * result);
Benchmark_Status_t test_ht(const char * path, const char * data, hash_func_t ht_func);
Benchmark_Status_t benchmark_find_ht(Hashtable_t * ht,
                                    StringArray_t * arr, BenchmarkResult_t * result);


#endif