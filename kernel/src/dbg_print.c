#if DEBUG == 1
/* *****************************************************************************
A few helper rutines for debugging...
***************************************************************************** */
#include "sys.h"
#define _VA_LIST
typedef __builtin_va_list va_list;
#define va_start(ap, param) __builtin_va_start(ap, param)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define __va_copy(d, s) __builtin_va_copy(d, s)
#define va_copy(dest, src) __builtin_va_copy(dest, src)
#define __GNUC_VA_LIST 1

void dbg_print(char16_t *str, ...) {
  static char16_t i2hex[] = {
      u'0', u'1', u'2', u'3', u'4', u'5', u'6', u'7',
      u'8', u'9', u'A', u'B', u'C', u'D', u'E', u'F',
  };
  va_list args;
  char16_t buff[512];
  uint16_t pos = 0;
  __builtin_va_start(args, str);
  while (*str) {
    if (pos >= 512) {
      buff[pos] = 0;
      k_write2console(buff);
      pos = 0;
    }
    if (*str == u'%') {
      str++;
      switch (*str) {
      case u's': {
        char16_t *p = va_arg(args, char16_t *);
        while (*p) {
          if (pos >= 511) {
            buff[pos] = 0;
            k_write2console(buff);
            pos = 0;
          }
          buff[pos++] = *p;
          ++p;
        }
        break;
      }
      case u'p': {
        if (pos >= 511) {
          buff[pos] = 0;
          k_write2console(buff);
          pos = 0;
        }
        buff[pos++] = u'0';
        buff[pos++] = u'x';
        uintptr_t p = va_arg(args, uintptr_t);
        for (size_t i = 0; i < 16; i++) {
          buff[pos++] = i2hex[(p >> (60 - (i * 4))) & 15];
        }
        break;
      }
#define NUM_CASE(sig, _type)                                                   \
  case (sig): {                                                                \
    if (pos >= 511) {                                                          \
      buff[pos] = 0;                                                           \
      k_write2console(buff);                                                   \
      pos = 0;                                                                 \
    }                                                                          \
    _type p = va_arg(args, _type);                                             \
    if (p < 0) {                                                               \
      buff[pos++] = u'-';                                                      \
      p = p * (-1);                                                            \
    }                                                                          \
    _type tmp = p;                                                             \
    size_t i = 1;                                                              \
    while ((tmp /= 10))                                                        \
      ++i;                                                                     \
    pos += i;                                                                  \
    size_t j = pos;                                                            \
    while (i) {                                                                \
      tmp = p;                                                                 \
      p /= 10;                                                                 \
      tmp -= (p * 10);                                                         \
      buff[--j] = u'0' + tmp;                                                  \
      --i;                                                                     \
    }                                                                          \
    break;                                                                     \
  }
        NUM_CASE(u'i', int32_t)
        NUM_CASE(u'u', uint32_t)
      case u'l': {
        str++;
        switch (*str) {
          NUM_CASE(u'i', int64_t)
          NUM_CASE(u'u', uint64_t)
        }
        break;
      }
      }
#undef NUM_CASE
    } else {
      buff[pos] = *str;
      pos++;
    }
    str++;
  }
  va_end(args);
  buff[pos] = 0;
  if (pos)
    k_write2console(buff);
}
#endif
