#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <smmintrin.h>

#include "text.h"
#include "hash_functions.h"


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



/*==========================================================================
Пустышка
==========================================================================*/
uint64_t hash_const(const Elem_t * word)
{
    assert(word);

    return 0;
}


/*==========================================================================
first_letter_hash
==========================================================================*/
uint64_t hash_first_char(const Elem_t * word)
{
    assert(word);

    return (uint64_t)word->data[0];
}

/*==========================================================================
word_len_hash
==========================================================================*/
uint64_t hash_word_len(const Elem_t * word)
{
    assert(word);

    return (uint64_t)word->length;
}

/*==========================================================================
checksum
==========================================================================*/
uint64_t hash_sum(const Elem_t * word)
{
    assert(word);

    uint64_t hash = 0;
    uint64_t word_length = word->length;
    for (uint64_t i = 0; i < word_length; i++)
        hash += (uint64_t)(unsigned int)word->data[i];
    
    return (uint64_t)hash;
}

/*==========================================================================
rotate left
==========================================================================*/
uint64_t hash_rotate(const Elem_t * word)
{
    assert(word);

    uint64_t word_length = word->length;
    if (word_length == 0)   return 0;
    
    uint64_t hash = (uint64_t)word->data[0];

    for (uint8_t i = 1; i < word->length; i++) {
        hash = (hash << 1) | (hash >> 63);
        hash ^= (uint64_t)word->data[i];
    }
    return hash;

    return hash;
}


/*==========================================================================
CRC_32
TODO: add table crc_32/ intrinsic_version
==========================================================================*/
uint64_t hash_crc32(const Elem_t * word)
{
    if (word == NULL)  return 0;

    uint32_t crc = 0xFFFFFFFF;
    uint32_t polynomial = 0xEDB88320;

    uint64_t word_length = word->length;
    if (word_length == 0)   return 0;
    
    for (uint8_t i = 0; i < word->length; i++) {
        crc ^= (unsigned char)word->data[i];
        for (int bits = 0; bits < 8; bits++) {
            if (crc & 1)
                crc = (crc >> 1) ^ polynomial;
            else
                crc >>= 1;
        }
    }

    return (uint64_t)crc ^ 0xFFFFFFFF;
}



/*==========================================================================
CRC_32_INTRINSICS
==========================================================================*/
uint64_t hash_crc32c_intrinsic(const Elem_t * word) 
{
    uint32_t hash = 0xFFFFFFFF;
    uint8_t len = word->length;
    const char * str = word->data;
    
    while (len >= 8) 
    {
        hash = _mm_crc32_u32(hash, *(const uint32_t*)str);
        hash = _mm_crc32_u32(hash, *(const uint32_t*)(str + 4));
        str += 8;
        len -= 8;
    }
    
    if (len >= 4) {
        hash = _mm_crc32_u32(hash, *(const uint32_t*)str);          // 4 bytes
        str += 4;
        len -= 4;
    }
    
    if (len >= 2) {
        hash = _mm_crc32_u16(hash, *(const uint16_t*)str);          // 2 bytes
        str += 2;
        len -= 2;
    }
    
    if (len >= 1) {
        hash = _mm_crc32_u8(hash, (uint8_t)*str);                   // 1 byte
    }
    
    return (uint64_t)hash;
}


/*==========================================================================
CRC_32_ASM
__asm__ volatile(
    "инструкция ассемблера"
    : выходные операнды
    : входные операнды
    : разрушаемые регистры 
);
==========================================================================*/
uint64_t hashcrc32_ASM_basic(const Elem_t* elem)
{
    
    uint32_t hash = 0xFFFFFFFF; 
    uint8_t len = elem->length;
    const char* str = elem->data;

    for (uint8_t i = 0; i < len; i++)
    {
        uint8_t byte = (uint8_t)str[i];

        __asm__ volatile(
            "crc32b %1, %0"  
            : "+r"(hash)     // %0: 32-bit register
            : "rm"(byte)     // %1: 8-bit register or memory
        );
    }

    return hash;
}


uint64_t hashcrc32_ASM(const Elem_t* elem)
{
    uint64_t hash = 0xFFFFFFFF; 
    uint32_t len = elem->length;
    const char* str = elem->data;

    
    while (len >= 8)
    {
        uint64_t chunk = *(const uint64_t*)str;
        __asm__ volatile(
            "crc32q %1, %0"
            : "+r"(hash)
            : "rm"(chunk)
        );
        str += 8;
        len -= 8;
    }

    
    while (len > 0)
    {
        uint8_t byte = *(const uint8_t*)str;
        __asm__ volatile(
            "crc32b %1, %0"
            : "+r"(hash)
            : "rm"(byte)
        );
        str++;
        len--;
    }

    return hash;
}



//======================================================================================
bool word_equal(const Elem_t * a, const Elem_t * b)
{
    if (a->length != b->length)     return false;
    if (a->length == 0)             return false;

    __m128i va = _mm_loadu_si128((const __m128i *)a->data);
    __m128i vb = _mm_loadu_si128((const __m128i *)b->data);

    __m128i cmp = _mm_cmpeq_epi8(va, vb);

    return _mm_movemask_epi8(cmp) == 0xFFFF;
}



bool word_equal_ASM(const Elem_t * a, const Elem_t * b)
{
    if (a->length != b->length)     return false;
    if (a->length == 0)             return false;

    return str_equal_16(a->data, b->data) == 1;
}
//======================================================================================
