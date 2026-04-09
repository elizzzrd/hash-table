#pragma once
#include <stdio.h>
#include <stdbool.h>

#define MAX_WORDS 40000
#define WORD_BUF_SIZE 256


typedef struct 
{
    char ** data;
    size_t size;
    size_t capacity;
} StringArray_t;


bool string_array_init(StringArray_t * array, size_t initial_capacity);
void string_array_free(StringArray_t * array);
bool string_array_push(StringArray_t * array, const char * word);
bool string_array_resize(StringArray_t * array);
void strings_array_peep(StringArray_t * array, int count, FILE * fp);

char * inizialize_buffer(size_t file_size);
size_t get_file_size(const char * filename);
size_t read_text_from_file(const char * filename, size_t size, char * buffer);

bool collect_words(const char * filename, StringArray_t * arr);