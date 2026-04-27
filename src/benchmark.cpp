#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <time.h>
#include <x86intrin.h>

#include "benchmark.h"
#include "hash_functions.h"
#include "hash_table.h"
#include "text.h"


extern FILE *log_fp;



static uint64_t get_current_cycles(void) 
{
    _mm_lfence();
    return __rdtsc();
}


static double get_time_ms(void) 
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
}


Benchmark_Status_t test_ht(const char * path)
{ 
    LOG_MESSAGE("\n========== HASH TABLE FIND BENCHMARK ==========\n");
    LOG_MESSAGE("Input file: %s\n", path);
    LOG_MESSAGE("Table capacity: %d\n", DEFAULT_BENCHMARK_TABLE_SIZE);
    LOG_MESSAGE("Repetitions per word: %d\n", MAX_BENCHMARK_COMP_NUM);

    Hashtable_t ht = {};
    if (!hashtable_init(&ht, DEFAULT_BENCHMARK_TABLE_SIZE, hash_crc32))
    {
        fprintf(stderr, "failed to init hash table for %s\n", hash_crc32);
        return BENCHMARK_STATUS_FILE_ERROR;
    }

    size_t count = build_hashtable_from_file(&ht, path);
    if (count == 0)
    {
        fprintf(stderr, "failed to load data for %s\n", hash_crc32);
        hashtable_destroy(&ht);
        return BENCHMARK_STATUS_MEMORY_ERROR;
    }

    StringArray_t arr = {};
    if (!string_array_init(&arr, MAX_WORDS))
    {
        LOG_MESSAGE("Initialization error: string array");
        return BENCHMARK_STATUS_FILE_ERROR;
    }

    if (!collect_words(path, &arr))
    {
        fprintf(stderr, "Failed to process file: %s\n", path);
        string_array_free(&arr);
        return BENCHMARK_STATUS_FAIL;
    }

    BenchmarkResult_t result = {.table_size = DEFAULT_BENCHMARK_TABLE_SIZE, 
                                .words_count = arr.size,
                                .found_count = 0,
                                .not_found_count = 0};
    
    uint64_t cycle_start = get_current_cycles();
    double time_start = get_time_ms();

    Benchmark_Status_t status = benchmark_find_ht(&ht, path, &arr, &result);

    uint64_t cycle_end = get_current_cycles();
    double time_end = get_time_ms();

    string_array_free(&arr);
    hashtable_destroy(&ht);

    result.total_cycles = cycle_end - cycle_start;
    result.total_time_ms = time_end - time_start;

    print_benchmark_results(&result);
}
    
void print_benchmark_results(BenchmarkResult_t * result)
{
    assert(result);

    result->avg_cycles_per_find = result->total_cycles / result->total_finds;
    result->avg_time_per_find = result->total_time_ms / result->total_finds;
    result->hash_func_name = "crc32";


    LOG_MESSAGE("\n\n=================== BENCHMARK RESULTS ===================");
    LOG_MESSAGE("Hash function: %s\n", result->hash_func_name);
    LOG_MESSAGE("Table size: %zu buckets\n", result->table_size);
    LOG_MESSAGE("Words count: %zu\n", result->words_count);
    LOG_MESSAGE("Total finds: %zu", result->total_finds);

    LOG_MESSAGE("CPU cycles:");
    LOG_MESSAGE("   CPU cycles total: %lu", result->total_cycles);
    LOG_MESSAGE("   Average cycles per find: %.2f\n", result->avg_cycles_per_find);

    LOG_MESSAGE("Time statictics:");
    LOG_MESSAGE("   Total time: %.3f ms", result->total_time_ms);
    LOG_MESSAGE("   Average time per find: %.3f μs\n", result->avg_time_per_find * 1000);
    
    LOG_MESSAGE("Search results:");
    LOG_MESSAGE("   Found:  %zu (%.2f%%)\n", result->found_count,
           (double)result->found_count / result->total_finds * 100.0);
    LOG_MESSAGE("  Not found: %zu (%.2f%%)\n", result->not_found_count,
           (double)result->not_found_count / result->total_finds * 100.0);
}


// TO DO: добавить поиск несуществующих слов
// Бенчмарк поиска слов
Benchmark_Status_t benchmark_find_ht(Hashtable_t * ht, const char * path, StringArray_t * arr, BenchmarkResult_t * result) 
{
    assert(ht && path && arr && result);

    size_t arr_size = (size_t)arr->size;
    result->total_finds = arr_size * MAX_BENCHMARK_COMP_NUM;
    LOG_MESSAGE("Starting find benchmark: %zu words, %d repetitions\n", 
                arr_size , MAX_BENCHMARK_COMP_NUM);
        
    for (size_t i = 0; i < MAX_BENCHMARK_COMP_NUM; i++) 
    {
        char * volatile curr_word = arr->data[i];
        for (size_t word_num = 0; word_num < arr_size; word_num++) 
        {
            if (hashtable_find_item(ht, curr_word))
            {
                result->found_count++;
            }
            else
            {
                LOG_MESSAGE("Cant find word \"%s\"\n", curr_word);
                result->not_found_count++;
            }
        }
    }
    
    LOG_MESSAGE("Find benchmark completed\n");
    return BENCHMARK_STATUS_OK;
}
