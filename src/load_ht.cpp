#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "text.h"
#include "hash_table.h"

extern FILE * log_fp;



size_t build_hashtable_from_file(Hashtable_t * ht, const char * filename)
{
    assert(ht && filename);
    
    LOG_MESSAGE("start build hashtable from file\n");

    FILE * file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s' for reading\n", filename);
        return 0;
    }
    
    const size_t BUFFER_SIZE = 2 * 1024 * 1024;  
    char * buffer = (char *)malloc(BUFFER_SIZE);
    if (buffer) {
        setvbuf(file, buffer, _IOFBF, BUFFER_SIZE);
    }
    
    size_t inserted = 0;
    
    while (!feof(file)) {
        size_t word_len;
        if (fread(&word_len, sizeof(size_t), 1, file) != 1) {
            break;
        }
        
        if (word_len == 0 || word_len > 10000) {
            fprintf(stderr, "Warning: Skipping invalid word length %zu\n", word_len);
            fseek(file, (long)word_len, SEEK_CUR);
            continue;
        }
        
        // temporary word buffer
        char * word = (char *)malloc(word_len + 1);
        if (!word) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            break;
        }
        
        if (fread(word, sizeof(char), word_len, file) != word_len) {
            free(word);
            fprintf(stderr, "Error: Failed to read word data\n");
            break;
        }
        
        word[word_len] = '\0';
        
        HT_Err err = hashtable_insert(ht, word);
        if (err == HT_OK) {
            inserted++;
        }
        //  else if (err == HT_OVERFLOW) {
        //     fprintf(stderr, "Error: Hash table overflow at %zu words\n", inserted);
        //     free(word);
        //     break;
        // } 
        
        free(word);
    }
    
    fclose(file);
    if (buffer) free(buffer);
    
    LOG_MESSAGE("Build complete: %zu unique words inserted\n", inserted);
    return inserted;
}


void export_to_csv(Hashtable_t * ht, const char * filename)
{
    assert(ht);
    FILE *fp = fopen(filename, "w");
    if (!fp)
    {
        fprintf(stderr, "cant open %s\n", filename);
        return;
    }

    fprintf(fp, "bucket,length\n");
    

    for (size_t i = 0; i < ht->capacity; i++)
    {
        if (ht->bucket_sizes[i])
            fprintf(fp, "%zu,%d\n", i, ht->bucket_sizes[i]);
        else    
            fprintf(fp, "%zu, 0\n", i);
    }

    fclose(fp);
}