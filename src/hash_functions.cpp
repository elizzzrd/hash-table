#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "hash_table.h"
#include "hash_functions.h"

#define MAX_WORD_LEN 256

HashFunctionInfo hash_functions[] = 
{
    {"const",       hash_const},
    {"first_char",  hash_first_char},
    {"word_len",    hash_word_len},
    {"sum",         hash_sum},
    {"rotate",      hash_rotate},
    {"crc32",       hash_crc32}
};
int hf_info_size = sizeof(hash_functions)/sizeof(hash_functions[0]);


static uint64_t rotate_left(uint64_t value, int shift)
{
    return (value << shift) | (value >> (64 - shift));
}


/*==========================================================================
Пустышка
==========================================================================*/
uint64_t hash_const(const Elem_t word)
{
    assert(word);

    return 0;
}


/*==========================================================================
first_letter_hash
==========================================================================*/
uint64_t hash_first_char(const Elem_t word)
{
    assert(word);

    return (uint64_t)word[0];
}

/*==========================================================================
word_len_hash
==========================================================================*/
uint64_t hash_word_len(const Elem_t word)
{
    assert(word);

    return strnlen(word, MAX_WORD_LEN);
}

/*==========================================================================
checksum
==========================================================================*/
uint64_t hash_sum(const Elem_t word)
{
    assert(word);

    uint64_t hash = 0;
    uint64_t word_length =strnlen(word, MAX_WORD_LEN);
    for (uint64_t i = 0; i < word_length; i++)
        hash += (uint64_t)word[i];
    
    return (uint64_t)hash;
}

/*==========================================================================
rotate left
==========================================================================*/
uint64_t hash_rotate(const Elem_t word)
{
    assert(word);

    uint64_t word_length = strnlen(word, MAX_WORD_LEN);
    if (word_length == 0)   return 0;
    
    uint64_t hash = (uint64_t)word[0];

    for (size_t i = 1; i <= word_length; i++)
    {
        hash = rotate_left(hash, 1);
        hash ^= (uint64_t)word[i];
    }

    return hash;
}


/*==========================================================================
CRC_32
TODO: add table crc_32/ intrinsic_version
==========================================================================*/
uint64_t hash_crc32(const Elem_t word)
{
    assert(word);

    uint32_t crc = 0xFFFFFFFF;
    uint32_t polynomial = 0xEDB88320;

    uint64_t word_length = strnlen(word, MAX_WORD_LEN);
    if (word_length == 0)   return 0;
    

    for (size_t byte_index = 1; byte_index <= word_length; byte_index++)
    {
        crc ^= (unsigned char)word[byte_index];

        for (int bits = 0; bits < 8; bits++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ polynomial;
            else
                crc >>= 1;
        }
    }

    return (uint64_t)crc ^ 0xFFFFFFFF;
}


