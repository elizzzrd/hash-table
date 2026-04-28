#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/stat.h>
#include "/home/gardina_elizaveta/projects/2sem/hash_table/hash-table/headers/text.h"

//============================================================================
bool export_words(const StringArray_t * arr, const char * filename)
{
    assert(arr && filename);
    
    FILE * file = fopen(filename, "wb");  
    if (!file) 
    {
        fprintf(stderr, "Error: Cannot open file '%s' for writing\n", filename);
        return false;
    }
    
    const size_t BUFFER_SIZE = 2 * 1024 * 1024;
    char* buffer = (char*)malloc(BUFFER_SIZE);
    if (buffer) {
        setvbuf(file, buffer, _IOFBF, BUFFER_SIZE);
    }

    size_t written = 0;
    for (size_t i = 0; i < arr->size; i++) {
        fwrite(&arr->data[i], sizeof(Word_t), 1, file);
        written++;
    }
    
    fclose(file);
    if (buffer) free(buffer);
    
    printf("Export: %zu words written to '%s'\n", written, filename);
    return true;
}


//============================================================================
bool string_array_init(StringArray_t * array, size_t initial_capacity)
{
    assert(array);

    array->data = (Word_t*)calloc((size_t)initial_capacity, sizeof(Word_t));
    if (array->data == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        return false;
    }
    array->size = 0;
    array->capacity = initial_capacity;

    return true;
}


void string_array_free(StringArray_t * array)
{
    if (array == NULL || array->data == NULL)
        return;
    
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}


bool string_array_push(StringArray_t * array, const char * word)
{
    assert(array && word);
    string_array_resize(array);
    
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        fprintf(stderr, "Word too long: %s (len=%zu)\n", word, len);
        return false;
    }

    memset(array->data[array->size].data, 0, MAX_WORD_LEN);
    memcpy(array->data[array->size].data, word, len);
    array->data[array->size].length = (uint8_t)len;
    array->size++;
    
    return true;
}


bool string_array_resize(StringArray_t * array)
{
    assert(array);

    if (array->size >= array->capacity)
    {
        size_t new_capacity = array->capacity * 2;
        if (new_capacity == 0)
            new_capacity = 16;

        Word_t * new_data = (Word_t *) realloc(array->data, new_capacity * sizeof(Word_t));
        if (new_data == NULL)
        {
            fprintf(stderr, "realloc error\n");
            return false;
        }

        array->data = new_data;
        array->capacity = new_capacity;
    }

    return true;
}


void strings_array_peep(StringArray_t * array, int count, FILE * fp)
{
    assert(array);
    
    fprintf(fp, "\nSTRING_ARRAY (first %d):\n", count);
    for (int i = 0; i < count && i < (int)array->size; i++) {
        fprintf(fp, "[%d] %.*s (len=%d)\n", 
                i + 1, 
                array->data[i].length, 
                array->data[i].data,
                array->data[i].length);
    }
}

//============================================================================
size_t get_file_size(const char * filename) 
{
    assert(filename != NULL);
    
    struct stat file_stat = {};
    if (stat(filename, &file_stat) == -1) 
    {
        fprintf(stderr, "stat error\n");
        return 0;
    }

    return (size_t)(file_stat.st_size);
}


char * inizialize_buffer(size_t file_size)
{
    if (file_size == 0)
    {
        fprintf(stderr, "File size is 0\n");
        file_size = 1;
    }

    char * buffer = (char *)calloc(file_size + 1, sizeof(char));
    if (buffer == NULL) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    return buffer;
}


size_t read_text_from_file(const char * filename, size_t size, char * buffer)
{
    assert(filename && buffer && size > 0);

    FILE * file = fopen(filename, "r");
    if (!file) 
    {
        fprintf(stderr, "Can not open the file %s.\n", filename);
        return 0;
    }

    size_t read_symbols = fread(buffer, sizeof(char), size, file);
    buffer[size] = '\0';

    fclose(file);
    return read_symbols;
}


//============================================================================
bool collect_words(const char * filename, StringArray_t * arr) 
{
    assert(filename && arr);

    size_t file_size = get_file_size(filename);
    if (file_size == 0)
        return false;

    char * buffer = inizialize_buffer(file_size);
    if (buffer == NULL) 
        return false;

    size_t read_symbols = read_text_from_file(filename, file_size, buffer);
    if (read_symbols == 0)
    {
        free(buffer);
        return false;
    }
    buffer[read_symbols] = '\0';

    char word[WORD_BUF_SIZE];
    size_t word_len = 0;

    for (size_t i = 0; i <= read_symbols; i++)
    {
        unsigned char c = (unsigned char)buffer[i];

        if (i == read_symbols) {
            c = '\0';
        }

        if (isalnum(c))
        {
            if (word_len < WORD_BUF_SIZE - 1)       
                word[word_len++] = (char)tolower(c);
        }
        else
        {
            if (word_len > 3 && word_len < 16)
            {
                word[word_len] = '\0';
                if (word[0] != '\0') {
                    if (!string_array_push(arr, word))
                    {
                        free(buffer);
                        return false;
                    }}
            }

            word_len = 0;
        }
    }
    
    free(buffer);
    return 1;
}
