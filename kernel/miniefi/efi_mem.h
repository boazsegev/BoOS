#ifndef H_EFI_VIRTUAL_MEM_H
#define H_EFI_VIRTUAL_MEM_H

#include "efi_required.h"

/* ******************************************************
** EFI Memory Services - Types and definitions
****************************************************** */
#define EFI_OPTIONAL_PTR 0x00000001

// These types can be “ORed” together as needed.
#define EFI_MEMORY_UC 0x0000000000000001
#define EFI_MEMORY_WC 0x0000000000000002
#define EFI_MEMORY_WT 0x0000000000000004
#define EFI_MEMORY_WB 0x0000000000000008
#define EFI_MEMORY_UCE 0x0000000000000010
#define EFI_MEMORY_WP 0x0000000000001000
#define EFI_MEMORY_RP 0x0000000000002000
#define EFI_MEMORY_XP 0x0000000000004000
#define EFI_MEMORY_NV 0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_RO 0x0000000000020000
#define EFI_MEMORY_RUNTIME 0x8000000000000000

#define EFI_MEMORY_DESCRIPTOR_VERSION 1

#define EFI_VIRTUAL_ADDRESS uint64_t
#define EFI_PHYSICAL_ADDRESS uint64_t

typedef struct {
  uint32_t Type;
  EFI_PHYSICAL_ADDRESS PhysicalStart;
  EFI_VIRTUAL_ADDRESS VirtualStart;
  uint64_t NumberOfPages;
  uint64_t Attribute;
} EFI_MEMORY_DESCRIPTOR;

typedef struct {
  uint32_t Version;
  uint32_t NumberOfEntries;
  uint32_t DescriptorSize;
  uint32_t Reserved;
  EFI_MEMORY_DESCRIPTOR *Entry;
} EFI_MEMORY_ATTRIBUTES_TABLE;

typedef enum {
  AllocateAnyPages,
  AllocateMaxAddress,
  AllocateAddress,
  MaxAllocateType
} EFI_ALLOCATE_TYPE;

typedef enum {
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType
} EFI_MEMORY_TYPE;

/* ******************************************************
** EFI Memory Services - API
****************************************************** */

typedef EFI_STATUS(EFIAPI *EFI_GET_MEMORY_MAP)(uintptr_t *MemoryMapSize,
                                               EFI_MEMORY_DESCRIPTOR *MemoryMap,
                                               uintptr_t *MapKey,
                                               uintptr_t *DescriptorSize,
                                               uint32_t *DescriptorVersion);

typedef EFI_STATUS(EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP)(
    uintptr_t MemoryMapSize, uintptr_t DescriptorSize,
    uint32_t DescriptorVersion, EFI_MEMORY_DESCRIPTOR *VirtualMap);

typedef EFI_STATUS(EFIAPI *EFI_CONVERT_POINTER)(uintptr_t DebugDisposition,
                                                void **Address);

typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_PAGES)(EFI_ALLOCATE_TYPE Type,
                                               EFI_MEMORY_TYPE MemoryType,
                                               uintptr_t Pages,
                                               EFI_PHYSICAL_ADDRESS *Memory);

typedef EFI_STATUS(EFIAPI *EFI_FREE_PAGES)(EFI_PHYSICAL_ADDRESS Memory,
                                           uintptr_t Pages);
typedef EFI_STATUS(EFIAPI *EFI_ALLOCATE_POOL)(EFI_MEMORY_TYPE PoolType,
                                              uintptr_t Size, void **Buffer);
typedef EFI_STATUS(EFIAPI *EFI_FREE_POOL)(void *Buffer);

#endif
