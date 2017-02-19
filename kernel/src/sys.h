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
#include "dbg_print.h"
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

#endif
