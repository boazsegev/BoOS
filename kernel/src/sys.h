#ifndef H_KERNEL_SYS_H
#define H_KERNEL_SYS_H
/* *****************************************************************************
This should be rewritten as an UEFI abstraction, so that UEFI system calls to
Runtime Services are protected (see the Sepcs, table 32, section 7.1, page 284
of the PDF).

EFI Helper to map the system resources into an accessible common struct for
safe-keeping.

Specifically this reads and stores the memory map.
***************************************************************************** */
#include "types.h"

/**
This function will write a NUL terminated string to the console.
*/
void k_write2console(char16_t *str);

/**
This function returns the address for the EFI_SYSTEM_TABLE.
*/
void *k_uefi_system_table(void);
/**
This function returns the address for the EFI_HANDLE.
*/
void *k_uefi_handle(void);

#if DEBUG == 1
/* *****************************************************************************
A few helper rutines for debugging...
***************************************************************************** */

/**
`dbg_print` provides a very simple "printf"-like console printing facilities,
with support being limited to:

* %s - a string object.
* %i, %u - int32_t and uint32_t respectively, printed as decimal numbers.
* %li, %lu - int64_t and uint64_t respectively, printed as decimal numbers.
* %p - 64bit numbers and pointers printed in hex notation.

*/
void dbg_print(char16_t *str, ...);
#else
/** noop, unless DEBUG == 1 is defined. */
#define dbg_print(...)
#endif

#endif
