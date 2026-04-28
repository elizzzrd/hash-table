#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "hash_table.h"



//======================================================================================
// LOGS SETTINGS
const char * log_file = "/home/gardina_elizaveta/projects/2sem/hash_table/hash-table/logger/hashtable.log";
FILE * log_fp = NULL;



bool log_initt(void)
{
    log_fp = fopen(log_file, "a");
    if (!log_fp) 
    {
        fprintf(stderr, "Can not open log file\n");
        return false;
    } 
    return true;
}

void log_close(void)
{
    fprintf(log_fp, "\n\n\n");
    if (log_fp)
    {
        fclose(log_fp);
        log_fp = NULL;
    }
}

void log_message(const char * file, int line, const char * format, ...)
{
    if (log_fp == NULL) return; 

    time_t t = time(NULL);
    struct tm * tm_info = localtime(&t);
    char time_buf[20];
    strftime(time_buf, sizeof(time_buf), "%m-%d %H-%M-%S", tm_info);
    
    fprintf(log_fp, "[%s] (%s:%d)\t", time_buf, file, line);

    va_list args;
    va_start(args, line);
    vfprintf(log_fp, format, args);
    va_end(args);

    fprintf(log_fp, "\n");
    fflush(log_fp);
}



//======================================================================================
// HASH TABLE DUMP
void dump_ht(const Hashtable_t * ht, const char * filename)
{
    assert(ht);

    FILE * fp = fopen(filename, "w");
    if (!fp)
    {
        fprintf(stderr, "can't open %s\n", filename);
        return;
    }
     
    fprintf(fp, "\n============HASH TABLE============\n");
    fprintf(fp, "Address: %p\n", (const void*)ht);
    fprintf(fp, "Capacity: %zu, Size: %zu\n", ht->capacity, ht->size);
    fprintf(fp, "Buckets array: %p\n", (void*)ht->buckets);
    fprintf(fp, "Bucket sizes array: %p\n", (void*)ht->bucket_sizes);
    fprintf(fp, "===================================================================\n");
    
    for (size_t i = 0; i < ht->capacity; i++)
    {
        fprintf(fp, "[%-4zu] ", i);
        
        if (ht->buckets[i] == NULL)
        {
            fprintf(fp, "NULL\n");
        }
        else
        {
            fprintf(fp, "bucket_size=%d -> ", ht->bucket_sizes[i]);
            
            Node_t * current = ht->buckets[i];
            size_t count = 0;
            
            while (current != NULL)
            {
                fprintf(fp, "[%p] \"%s\" (size=%d, prev=%p, next=%p)", 
                       (void*)current, current->word, current->size,
                       (void*)current->prev, (void*)current->next);
                count++;
                
                current = current->next;
                if (current != NULL)
                    fprintf(fp, " -> ");
            }
            
            if (count != (size_t)ht->bucket_sizes[i]) {
                fprintf(fp, " [WARNING: chain length (%zu) != bucket_sizes[%zu] (%d)]", 
                       count, i, ht->bucket_sizes[i]);
            }
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "===================================================================\n\n");
    fclose(fp);
}


bool hashtable_validate(const Hashtable_t * ht)
{
    if (!ht) return false;
    
    size_t total_size = 0;
    
    for (size_t i = 0; i < ht->capacity; i++)
    {
        Node_t * current = ht->buckets[i];
        size_t chain_length = 0;
        Node_t * prev = NULL;
        
        while (current != NULL)
        {
            chain_length++;
            
            if (current->prev != prev) {
                log_message(__FILE__, __LINE__, "Invalid prev pointer at bucket %zu, node %p", i, (void*)current);
                return false;
            }
            
            // Проверяем, что слово не NULL
            if (current->word == NULL) {
                log_message(__FILE__, __LINE__, "NULL word at bucket %zu, node %p", i, (void*)current);
                return false;
            }
            
            prev = current;
            current = current->next;
        }
        
        if (chain_length != (size_t)ht->bucket_sizes[i]) {
            log_message(__FILE__, __LINE__, "Chain length mismatch at bucket %zu: chain=%zu, bucket_sizes=%d", 
                     i, chain_length, ht->bucket_sizes[i]);
            return false;
        }
        
        total_size += chain_length;
    }
    
    if (total_size != ht->size) {
        log_message(__FILE__, __LINE__, "Total size mismatch: total=%zu, ht->size=%zu", total_size, ht->size);
        return false;
    }
    
    return true;
}
    

const char * ht_error_string(HT_Err error) 
{
    static const char * list_error_strings[] = 
    {
        "HT_OK",                                      
        "HT_NULL_PTR",                                
        "HT_MEMORY_ALLOCATION_ERROR",                 
        "HT_INVALID_INSERT",                          
        "HT_INVALID_DELETE",                          
        "HT_OVERFLOW"                                 
    };
    
    if (error < HT_OK || error > HT_OVERFLOW) return "___UNKNOWN ERROR___";
    
    return list_error_strings[error];
}

//======================================================================================