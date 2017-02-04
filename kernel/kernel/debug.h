#ifndef H_DEBUG_H
#define H_DEBUG_H
#if DEBUG == 1
/* *****************************************************************************
A few helper rutines for debugging...
***************************************************************************** */
#include "../miniefi/efi.h"
void dbg_print(EFI_SYSTEM_TABLE *ST, char16_t *str, ...);
#else
#define dbg_print(...)
#endif
#endif
