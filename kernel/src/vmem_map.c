#include "vmem_map.h"
#include "miniefi/efi.h"
#include "sys.h"

#include "spinlock.h"

#if DEBUG == 1
static void print_memap(vmem_map_pt map);
#else
#define print_memap(...)
#endif
/* *****************************************************************************
System data storage
***************************************************************************** */
/**
A memory map struct.
*/
struct vmem_map_s {
  EFI_SET_VIRTUAL_ADDRESS_MAP set_vmap;
  uintptr_t map_size;
  uintptr_t map_limit;
  EFI_MEMORY_DESCRIPTOR *map;
  uintptr_t map_key;
  uintptr_t descriptor_size;
  uint32_t descriptor_version;
  unsigned virtual : 1;
};

/**
The root memory map.
*/
static struct vmem_map_s vmap_original;

/* *****************************************************************************
System memory storage
***************************************************************************** */

/* *****************************************************************************
Initialization
*****************************************************************************
*/

/**
Helper function that loads the initial UEFI memory map.
*/
static int vmemory_load_uefi(void) {
  EFI_SYSTEM_TABLE *ST = k_uefi_system_table();
  EFI_STATUS ret = 0;
  dbg_print(u"Checking Memory Map requirements.\r\n");
  ret = ST->BootServices->GetMemoryMap(
      &vmap_original.map_size, vmap_original.map, &vmap_original.map_key,
      &vmap_original.descriptor_size, &vmap_original.descriptor_version);
  if (ret != EFI_BUFFER_TOO_SMALL)
    goto error;
  /* We need to loop this part, because we are allocating memory, which
   * updates
   * the memory map... which means we need to ask for more memory, and the
   * cycle
   * goes on.*/
  for (size_t i = 0; i < 5; i++) {
    if (vmap_original.map != NULL)
      ST->BootServices->FreePool(vmap_original.map);
    if (vmap_original.map_size > 0) {
      dbg_print(u"Allocating memory to store the Memory Map.\r\n");
      ret = ST->BootServices->AllocatePool(
          EfiLoaderData, vmap_original.map_size, (void **)(&vmap_original.map));
      if (ret)
        goto error;
      vmap_original.map_limit = vmap_original.map_size;
      dbg_print(u"Loading the Memory Map.\r\n");
      ret = ST->BootServices->GetMemoryMap(
          &vmap_original.map_size, vmap_original.map, &vmap_original.map_key,
          &vmap_original.descriptor_size, &vmap_original.descriptor_version);
      if (ret == EFI_BUFFER_TOO_SMALL)
        continue;
      else if (ret)
        goto error;
      else
        break;
    };
  }
  return 0;
error:
  if (vmap_original.map != NULL)
    ST->BootServices->FreePool(vmap_original.map);
  dbg_print(u"ERROR: Memory Map query error %p!\r\n", ret);
  return -1;
}

/**
Creates the first (initial) memory map for the kernel.
*/
vmem_map_pt vmemory_map_initialize(void) {
  EFI_SYSTEM_TABLE *ST = k_uefi_system_table();
  vmap_original.set_vmap = ST->RuntimeServices->SetVirtualAddressMap;
  if (vmemory_load_uefi())
    return NULL;

  dbg_print(u"Memory map initialization IMPLEMENTATION INCOMPLETE");
  print_memap(&vmap_original);
  return &vmap_original;
}

/**
Finalizes initialization of the memory map by reclaiming the memory used by
the
UEFI Boot Services.
*/
vmem_map_pt vmemory_map_init_runtime(void) { return NULL; }

/* *****************************************************************************
System data storage
*****************************************************************************
*/

/**
Copies an existing memory map for a new process. Returns the copy.
*/
vmem_map_pt vmemory_map_copy(vmem_map_pt vmap) {
  dbg_print(u"NOT IMPLEMENTED");
  (void)(vmap);
  return NULL;
}
/**
Switches the active virtual memory map to the requested memory map.
*/
int8_t vmemory_map_switch(vmem_map_pt vmap) {
  dbg_print(u"NOT IMPLEMENTED");
  (void)(vmap);
  return -1;
}
/**
Destroys the indicated memory map, freeing it's resources.
*/
int8_t vmemory_map_destroy(vmem_map_pt vmap) {
  dbg_print(u"NOT IMPLEMENTED");
  (void)(vmap);
  return -1;
}
/**
Allocates more memory for the memory map.

Returns the (virtual) address of the allocated memory

`size` is the number of pages to allocate (each page ~4Kib).
*/
void *vmemory_map_allocate(vmem_map_pt vmap, size_t size) {
  dbg_print(u"NOT IMPLEMENTED");
  (void)(vmap);
  (void)(size);
  return NULL;
}
/**
Frees an allocated segment.

`seg` is the (virtual) address of the fisrt page to be freed.
`size` is the number of pages to free (each page ~4Kib).
*/
int8_t vmemory_map_free(vmem_map_pt vmap, void *seg, size_t size) {
  dbg_print(u"NOT IMPLEMENTED");
  (void)(vmap);
  (void)(seg);
  (void)(size);
  return -1;
}
/**
Shares a memory segment between two memory maps.

Returns the (virtual) address of the memory in the new map.

`seg` is the (virtual) address of the fisrt page to be shared.
`size` is the number of pages to share (each page ~4Kib).
*/
void *vmemory_map_share(vmem_map_pt dest, vmem_map_pt source, void *seg,
                        size_t size) {
  dbg_print(u"NOT IMPLEMENTED");
  (void)(dest);
  (void)(source);
  (void)(seg);
  (void)(size);
  return NULL;
}

/* *****************************************************************************
Debug helper implementation
***************************************************************************** */
#if DEBUG == 1

static char16_t *uefi_mem_types[] = {u"EfiReservedMemoryType",
                                     u"EfiLoaderCode",
                                     u"EfiLoaderData",
                                     u"EfiBootServicesCode",
                                     u"EfiBootServicesData",
                                     u"EfiRuntimeServicesCode",
                                     u"EfiRuntimeServicesData",
                                     u"EfiConventionalMemory",
                                     u"EfiUnusableMemory",
                                     u"EfiACPIReclaimMemory",
                                     u"EfiACPIMemoryNVS",
                                     u"EfiMemoryMappedIO",
                                     u"EfiMemoryMappedIOPortSpace",
                                     u"EfiPalCode",
                                     u"EfiPersistentMemory",
                                     u"EfiMaxMemoryType"};

static void print_memap(vmem_map_pt map) {
  // EFI_SYSTEM_TABLE *ST = k_uefi_system_table();
  // EFI_INPUT_KEY Key;
  size_t pages = 0;
  size_t type_size[16] = {0};
  EFI_MEMORY_DESCRIPTOR *mem_desc;
  dbg_print(u"\r\nMemory Map is %i items long\r\n"
            u"Memory Map requires %i Bytes\r\n"
            u"   each object requiring %i Bytes\r\n"
            u"Memory Map container uses %i Bytes more\r\n",
            (map->map_size / map->descriptor_size), map->map_size,
            map->descriptor_size, sizeof(*map));
  for (size_t i = 0; i < (map->map_size / map->descriptor_size); i++) {
    mem_desc = (void *)((uintptr_t)(map->map) + (i * map->descriptor_size));
    dbg_print(u"Reading from memory at: %p\r\n", mem_desc);
    dbg_print(u"Memory Map Descriptor:\r\n"
              u"* Type: %i\r\n"
              u"* PhysicalStart: %p\r\n"
              u"* VirtualStart: %p\r\n"
              u"* NumberOfPages: %lu\r\n"
              u"* Attribute: %p\r\n\r\n",
              mem_desc->Type, mem_desc->PhysicalStart, mem_desc->VirtualStart,
              mem_desc->NumberOfPages, mem_desc->Attribute);
    type_size[mem_desc->Type] += mem_desc->NumberOfPages;
    pages += mem_desc->NumberOfPages;
    // while (ST->ConIn->ReadKeyStroke(ST->ConIn, &Key) == EFI_NOT_READY)
    //   ;
  }
  dbg_print(u"======\r\n"
            u"Memory map size: %lu Bytes\r\n"
            u"Memory map items: %lu items X %lu Bytes, Ver.%i.\r\n"
            u"Memory storage size: %lu Bytes\r\n",
            map->map_size, (map->map_size / map->descriptor_size),
            map->descriptor_size, map->descriptor_version, sizeof(*map));
  dbg_print(u"======\r\nTotal Number of Pages: %lu\r\n"
            u"Total memory in map: %lu\r\n"
            u"(some memory might have been mapped more than once)\r\n",
            pages, pages * 4096);
  for (size_t i = 0; i < 16; i++) {
    dbg_print(u"* %lu (%luKib) pages for memory type %lu (%s):\r\n",
              type_size[i], type_size[i] * 4, i, uefi_mem_types[i]);
  }
  dbg_print(u"\r\n");
}
#endif
