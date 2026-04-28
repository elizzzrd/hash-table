#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hash_table.h"
#include "hash_functions.h"
#include "analyse.h"

extern FILE * log_fp;



void test_one_ht_size(int ht_size, HashStats_t * stats, const char * data)
{
    for (int i = 0; i < hf_info_size; i++)
    {
        char logger_name[32];
        snprintf(logger_name, 32, "logger/%d-%s.log", i, hash_functions[i].name);
        LOG_MESSAGE("logger_name: %s\n", logger_name);

        char csv_name[32];
        snprintf(csv_name, 32, "data_csv/%d-%s_%d.csv", i, hash_functions[i].name, ht_size);
        LOG_MESSAGE("csv_name: %s\n", csv_name);

        printf("Testing hash function: %s\n", hash_functions[i].name);
        Hashtable_t ht = {};
        if (!hashtable_init(&ht, (size_t)ht_size, hash_functions[i].func))
        {
            fprintf(stderr, "failed to init hash table for %s\n", hash_functions[i].name);
            continue;
        }
        
        LOG_MESSAGE("hashtable for %s inited\n", hash_functions[i].name);

        size_t count = build_hashtable_from_file(&ht, data);
        if (count == 0)
        {
            fprintf(stderr, "failed to load data for %s\n", hash_functions[i].name);
            hashtable_destroy(&ht);
            continue;
        }

        LOG_MESSAGE("Loaded %zu words\n", count);
        dump_ht(&ht, logger_name);
    
        LOG_MESSAGE("starting to analyse\n");
        stats[i] = analyze_table(&ht);
        LOG_MESSAGE("analysis end\n");

        export_to_csv(&ht, csv_name);
        LOG_MESSAGE("data exported to csv\n");
        hashtable_destroy(&ht);
        LOG_MESSAGE("hashtable for %s destroyed\n", hash_functions[i].name);
        LOG_MESSAGE("---------------------------------------\n\n");
    }
}



HashStats_t analyze_table(Hashtable_t * ht)
{
    assert(ht);
    HashStats_t stats = {0};

    size_t total_elements = ht->size;
    assert(ht->bucket_sizes);
    assert(ht->buckets);
    int * chain_lengths = ht->bucket_sizes; 

    for (size_t i = 0; i < ht->capacity; i++)
    {
        int len = chain_lengths[i];
        if (len > 1)
            stats.collisions += (len - 1);
        if (len > stats.max_chain)
            stats.max_chain = len;
    }

    double mean = (ht->size > 0) ? (double)(total_elements / ht->size) : 0.0;
    stats.average_chain = mean;

    double var = 0.0;
    for (size_t i = 0; i < ht->capacity; i++)
    {
        double diff = chain_lengths[i] - mean;
        var += diff * diff;
    }

    stats.variance = var / (double)ht->size;
    return stats;
}


void print_table_header()
{
    fprintf(log_fp, "----------------------------------------------------------------------------------------------------------------\n");
    fprintf(log_fp, "| %-15s | %-15s | %-15s | %-10s | %-10s | %-10s | %-12s |\n",
           "Hash function", "Running time (ms)", "Running time (s)", "Collisions", "Max chain", "Avg chain", "Variance");
    fprintf(log_fp, "----------------------------------------------------------------------------------------------------------------\n");
}

void print_table_row(const char *name, HashStats_t s)
{
    fprintf(log_fp, "| %-15s | %-15.0lf   | %-15.0lf  | %-10d | %-10d | %-10.2f | %-12.2f |\n",
           name,
           s.running_time,
           (s.running_time * 1000),
           s.collisions,
           s.max_chain,
           s.average_chain,
           s.variance);
}
