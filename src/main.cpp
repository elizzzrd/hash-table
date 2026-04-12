#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <chrono>
#include "hash_table.h"

#include "hash_funtions.h"
extern FILE * log_fp;



inline double get_time_ms(void) 
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
}

int main(void)
{
    log_init();
    const char * data = "/home/gardina_elizaveta/projects/2sem/hash_table/hash-table/src/text/data.bin";
    
    HashStats_t * stats = (HashStats_t *) calloc((size_t)hf_info_size, sizeof(HashStats_t));

    for (int i = 0; i < hf_info_size; i++)
    {
        char logger_name[32];
        snprintf(logger_name, 32, "logger/%d-%s.log", i, hash_functions[i].name);

        char csv_name[32];
        snprintf(csv_name, 32, "data_csv/%d-%s.csv", i, hash_functions[i].name);

        double start = get_time_ms();

        printf("Testing hash function: %s\n", hash_functions[i].name);
        Hashtable_t ht = {};
        if (!hashtable_init(&ht, MAX_WORDS, hash_functions[i].func))
        {
            fprintf(stderr, "failed to init hash table for %s\n", hash_functions[i].name);
            continue;
        }
        
        size_t count = build_hashtable_from_file(&ht, data);
        if (count == 0)
        {
            fprintf(stderr, "failed to load data for %s\n", hash_functions[i].name);
            hashtable_destroy(&ht);
            continue;
        }
        
        fprintf(log_fp, "Loaded %zu words\n", count);
        dump_ht(&ht, logger_name);
    
        stats[i] = analyze_table(&ht);
        double end = get_time_ms();

        export_to_csv(&ht, csv_name);
        hashtable_destroy(&ht);

        stats[i].running_time = end - start;
    }
    
    print_table_header();
    for (int i = 0; i < hf_info_size; i++)
    {
        print_table_row(hash_functions[i].name, stats[i]);
    }
        
    free(stats);
    log_close();
    return 0;
}
