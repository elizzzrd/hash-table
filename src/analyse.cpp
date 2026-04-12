#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"
#include "hash_funtions.h"

extern FILE * log_fp;

HashStats_t analyze_table(Hashtable_t * ht)
{
    HashStats_t stats = {0};

    size_t total_elements = ht->size;
    int * chain_lengths = ht->bucket_sizes;

    for (size_t i = 0; i < ht->size; i++)
    {
        int len = chain_lengths[i];
        if (len > 1)
            stats.collisions += (len - 1);
        if (len > stats.max_chain)
            stats.max_chain = len;
    }

    double mean = (double)(total_elements / ht->size);
    stats.average_chain = mean;

    double var = 0.0;
    for (size_t i = 0; i < ht->size; i++)
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