#ifndef TYPES_H
#define TYPES_H

/* basic types. bitlength represents minimal length... might be bigger :( */
typedef unsigned char uint8_t;
typedef char int8_t;
_Static_assert(sizeof(int8_t) == 1, "int8_t != 8 bits");
typedef unsigned short uint16_t;
typedef uint16_t char16_t;
typedef short int16_t;
_Static_assert(sizeof(char16_t) == 2, "char16_t != 16 bits");
typedef unsigned int uint32_t;
typedef int int32_t;
_Static_assert(sizeof(uint32_t) == 4, "int32_t != 32 bits");
typedef unsigned long long uint64_t;
typedef long long int64_t;
_Static_assert(sizeof(int64_t) == 8, "int64_t != 64 bits");
/* clang extensions for 128bit*/
// typedef __int128_t int128_t;
// typedef __uint128_t uint128_t;
_Static_assert(sizeof(void *) == 8, "void * != 64 bits");
typedef unsigned long long uintptr_t;
typedef long long intptr_t;
_Static_assert(sizeof(void *) == 8, "intptr_t != 64 bits");

#endif
