#include <assert.h>
#include <string.h>
#include <stdint.h>
#include "list.h"

#define MAX_WORD_LEN 256

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

    return (uint64_t)strnlen(word, MAX_WORD_LEN);
}

/*==========================================================================
checksum
==========================================================================*/
uint64_t hash_sum(const Elem_t word)
{
    assert(word);

    uint64_t hash = 0;
    uint64_t word_length = (uint64_t) strnlen(word, MAX_WORD_LEN);
    for (uint64_t i = 0; i < word_length; i++)
        hash += word[i];
    
    return (uint64_t)hash;
}

/*==========================================================================
rotate left
==========================================================================*/
uint64_t hash_rotate(const Elem_t word)
{
    assert(word);

    uint64_t word_length = (uint64_t) strnlen(word, MAX_WORD_LEN);
    if (word_length == 0)   return 0;
    
    uint64_t hash = word[0];

    for (size_t i = 1; i <= word_length; i++)
    {
        hash = rotate_left(hash, 1);
        hash ^= word[i];
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

    uint64_t word_length = (uint64_t) strnlen(word, MAX_WORD_LEN);
    if (word_length == 0)   return 0;
    

    for (size_t i = 1; i <= word_length; i++)
    {
        crc ^= (unsigned char)word[i++];

        for (int i = 0; i < 8; i++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ polynomial;
            else
                crc >>= 1;
        }
    }

    return (uint64_t)crc ^ 0xFFFFFFFF;
}


