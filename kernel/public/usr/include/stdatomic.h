#ifndef H_STDATOMIC_H

#define H_STDATOMIC_H
#include "types.h"

#ifdef __has_feature
#ifdef __has_feature(c_atomic)

typedef _Atomic _Bool atomic_bool;
typedef _Atomic char atomic_char;
typedef _Atomic signed char atomic_schar;
typedef _Atomic unsigned char atomic_uchar;
typedef _Atomic short atomic_short;
typedef _Atomic unsigned short atomic_ushort;
typedef _Atomic int atomic_int;
typedef _Atomic unsigned int atomic_uint;
typedef _Atomic long atomic_long;
typedef _Atomic unsigned long atomic_ulong;
typedef _Atomic long long atomic_llong;
typedef _Atomic unsigned long long atomic_ullong;
typedef _Atomic char16_t atomic_char16_t;
typedef _Atomic char32_t atomic_char32_t;
typedef _Atomic wchar_t atomic_wchar_t;

typedef _Atomic int_least8_t atomic_int_least8_t;
typedef _Atomic uint_least8_t atomic_uint_least8_t;
typedef _Atomic int_least16_t atomic_int_least16_t;
typedef _Atomic uint_least16_t atomic_uint_least16_t;
typedef _Atomic int_least32_t atomic_int_least32_t;
typedef _Atomic uint_least32_t atomic_uint_least32_t;
typedef _Atomic int_least64_t atomic_int_least64_t;
typedef _Atomic uint_least64_t atomic_uint_least64_t;
typedef _Atomic int_fast8_t atomic_int_fast8_t;
typedef _Atomic uint_fast8_t atomic_uint_fast8_t;
typedef _Atomic int_fast16_t atomic_int_fast16_t;
typedef _Atomic uint_fast16_t atomic_uint_fast16_t;
typedef _Atomic int_fast32_t atomic_int_fast32_t;
typedef _Atomic uint_fast32_t atomic_uint_fast32_t;
typedef _Atomic int_fast64_t atomic_int_fast64_t;
typedef _Atomic uint_fast64_t atomic_uint_fast64_t;
typedef _Atomic intptr_t atomic_intptr_t;
typedef _Atomic uintptr_t atomic_uintptr_t;
typedef _Atomic size_t atomic_size_t;
typedef _Atomic ptrdiff_t atomic_ptrdiff_t;
typedef _Atomic intmax_t atomic_intmax_t;
typedef _Atomic uintmax_t atomic_uintmax_t;

#define atomic_init(...) __c11_atomic_init(__VA_ARGS__)
#define atomic_thread_fence(...) __c11_atomic_thread_fence(__VA_ARGS__)
#define atomic_signal_fence(...) __c11_atomic_signal_fence(__VA_ARGS__)
#define atomic_is_lock_free(...) __c11_atomic_is_lock_free(__VA_ARGS__)

#define atomic_store(...) __c11_atomic_store(__VA_ARGS__)
#define atomic_load(...) __c11_atomic_load(__VA_ARGS__)
#define atomic_exchange(...) __c11_atomic_exchange(__VA_ARGS__)
#define atomic_compare_exchange_strong(...)                                    \
  __c11_atomic_compare_exchange_strong(__VA_ARGS__)
#define atomic_compare_exchange_weak(...)                                      \
  __c11_atomic_compare_exchange_weak(__VA_ARGS__)
#define atomic_fetch_add(...) __c11_atomic_fetch_add(__VA_ARGS__)
#define atomic_fetch_sub(...) __c11_atomic_fetch_sub(__VA_ARGS__)
#define atomic_fetch_and(...) __c11_atomic_fetch_and(__VA_ARGS__)
#define atomic_fetch_or(...) __c11_atomic_fetch_or(__VA_ARGS__)
#define atomic_fetch_xor(...) __c11_atomic_fetch_xor(__VA_ARGS__)

#else
#error Atomic operations aren't supported on this compiler.
#endif
#else
#error Atomic operations aren't supported on this compiler.
#endif

#endif
