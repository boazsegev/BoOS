#pragma once
#ifndef H_KERNEL_SYS_H
#define H_KERNEL_SYS_H
#include "efi.h"
/* *****************************************************************************
EFI Helper to map the system resources into an accessible common struct for
safe-keeping.

Specifically this reads and stores the memory map.
***************************************************************************** */

/**
The system resources data
*/
typedef struct {
  EFI_SYSTEM_TABLE *ST;
  struct {
    uintptr_t map_size;
    EFI_MEMORY_DESCRIPTOR *map;
    uintptr_t map_key;
    uintptr_t descriptor_size;
    uint32_t descriptor_Version;
  } memmap;
} k_system_resources_s;

extern k_system_resources_s k_system_resources;
/**
Loads the data from the UEFI interface to a k_system_resources_s struct.

Be aware that some memory is allocated during the process. Free the memory
before destroying the struct object (of just keep the data for as long as the OS
is running).
*/
ssize_t k_system_init(EFI_SYSTEM_TABLE *SystemTable);
void k_free_system_data(void);

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
