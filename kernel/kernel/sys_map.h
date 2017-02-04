#pragma once
#ifndef H_KERNEL_SYS_MAP_H
#define H_KERNEL_SYS_MAP_H
#include "../miniefi/efi.h"
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

/**
Loads the data from the UEFI interface to a k_system_resources_s struct.

Be aware that some memory is allocated during the process. Free the memory
before destroying the struct object (of just keep the data for as long as the OS
is running).
*/
k_system_resources_s k_collect_system_data(EFI_SYSTEM_TABLE *SystemTable);
void k_free_system_data(k_system_resources_s *data);

#endif
