

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <chrono>
#include "hash_table.h"
#include "analyse.h"
#include "hash_functions.h"


extern FILE * log_fp;

#define HT_101 101
#define HT_307 307
#define HT_3001 3001
#define HT_30011 30011



int main(void)
{
    log_init();

    #ifdef BENCHMARk
    Hashtable_t ht = {};
    const char * path = "/home/gardina_elizaveta/projects/2sem/hash_table/hash-table/source-text/war-and-peace.txt";
    test_ht(path);

    #endif


    #ifdef COMPARISON
    const char * data = "/home/gardina_elizaveta/projects/2sem/hash_table/hash-table/src/text/data.bin";
    
    HashStats_t * stats = (HashStats_t *) calloc((size_t)hf_info_size, sizeof(HashStats_t));

    test_one_ht_size(HT_101, stats, data);
    test_one_ht_size(HT_307, stats, data);
    test_one_ht_size(HT_3001, stats, data);
    test_one_ht_size(HT_30011, stats, data);

    print_table_header();
    for (int i = 0; i < hf_info_size; i++)
    {
        print_table_row(hash_functions[i].name, stats[i]);
    }
    
    free(stats);
    #endif

    
    log_close();
    return 0;
}


