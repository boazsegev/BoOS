#include "miniefi/efi.h"

// #include "sys.h"

/* *****************************************************************************
System data storage
***************************************************************************** */
typedef struct {
  EFI_SYSTEM_TABLE *ST;
  EFI_HANDLE image_handle;
  struct {
    uintptr_t map_size;
    EFI_MEMORY_DESCRIPTOR *map;
    uintptr_t map_key;
    uintptr_t descriptor_size;
    uint32_t descriptor_Version;
  } memmap;
} k_uefi_s;

k_uefi_s k_uefi;

/* *****************************************************************************
Debug helpers
***************************************************************************** */
#if DEBUG == 1
#pragma weak dbg_print
void dbg_print(char16_t *str, ...) {
  (void)str;
  return;
}
void print_memap(void);
#else
#define dbg_print(...)
#define print_memap()
#endif

/* *****************************************************************************
After the UEFI data is loaded and handled, the initialization will call the
`kmain` function, allowing the OS to take over.
***************************************************************************** */
#pragma weak kmain
int kmain(void) { return 0; }

/* *****************************************************************************
System initialization - this is where the UEFI application begins
***************************************************************************** */
EFI_STATUS EFIAPI __efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *ST) {
  k_uefi = (k_uefi_s){.ST = ST,
                      .image_handle = ImageHandle,
                      .memmap.map_size = 0,
                      .memmap.map = NULL};
  EFI_STATUS ret = 0;
  dbg_print(u"Checking Memory Map requirements.\r\n");
  ret = ST->BootServices->GetMemoryMap(
      &k_uefi.memmap.map_size, k_uefi.memmap.map, &k_uefi.memmap.map_key,
      &k_uefi.memmap.descriptor_size, &k_uefi.memmap.descriptor_Version);
  if (ret != EFI_BUFFER_TOO_SMALL)
    goto error;
  /* We need to loop this part, because we are allocating memory, which updates
   * the memory map... which means we need to ask for more memory, and the cycle
   * goes on.*/
  for (size_t i = 0; i < 5; i++) {
    if (k_uefi.memmap.map != NULL)
      ST->BootServices->FreePool(k_uefi.memmap.map);
    if (k_uefi.memmap.map_size > 0) {
      dbg_print(u"Allocating memory to store the Memory Map.\r\n");
      ret = ST->BootServices->AllocatePool(
          EfiLoaderData, k_uefi.memmap.map_size, (void **)(&k_uefi.memmap.map));
      if (ret)
        goto error;
      dbg_print(u"Loading the Memory Map.\r\n");
      ret = ST->BootServices->GetMemoryMap(
          &k_uefi.memmap.map_size, k_uefi.memmap.map, &k_uefi.memmap.map_key,
          &k_uefi.memmap.descriptor_size, &k_uefi.memmap.descriptor_Version);
      if (ret == EFI_BUFFER_TOO_SMALL)
        continue;
      else if (ret)
        goto error;
      else
        break;
    };
  }
  print_memap();
  return 0;
error:
  if (k_uefi.memmap.map != NULL)
    ST->BootServices->FreePool(k_uefi.memmap.map);
  k_uefi = (k_uefi_s){.ST = NULL};
  dbg_print(u"ERROR: Memory Map query error %p!\r\n", ret);
  return EFI_LOAD_ERROR;
}

/* *****************************************************************************
Printing to the UEFI console.
***************************************************************************** */
void k_write2console(char16_t *str) {
  if (k_uefi.ST == NULL || k_uefi.ST->ConOut == NULL)
    return;
  k_uefi.ST->ConOut->OutputString(k_uefi.ST->ConOut, str);
}

#if DEBUG == 1
void print_memap(void) {
  size_t pages = 0;
  size_t type_size[16] = {0};
  EFI_INPUT_KEY Key;
  EFI_MEMORY_DESCRIPTOR *mem_desc;
  dbg_print(u"Testing printing for\r\n"
            u"String: %s\r\n"
            u"int: %i uint: %u long %li\r\npointer: %p\r\n",
            u"a little test", (int32_t)1, (uint32_t)42, (int64_t)(-73),
            (void *)0xFFEEDD1234000000);
  dbg_print(u"Memory Map is %i items long\r\n",
            (k_uefi.memmap.map_size / k_uefi.memmap.descriptor_size));
  for (size_t i = 0;
       i < (k_uefi.memmap.map_size / k_uefi.memmap.descriptor_size); i++) {
    mem_desc = (void *)((uintptr_t)(k_uefi.memmap.map) +
                        (i * k_uefi.memmap.descriptor_size));
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
    while (k_uefi.ST->ConIn->ReadKeyStroke(k_uefi.ST->ConIn, &Key) ==
           EFI_NOT_READY)
      ;
  }
  dbg_print(u"======\r\nTotal Number of Pages: %lu\r\n"
            u"Total memory in map: %lu\r\n"
            u"(some memory might have been mapped more than once)\r\n",
            pages, pages * 4096);
  for (size_t i = 0; i < 16; i++) {
    dbg_print(u"* %lu (%luKib) pages for memory type %lu:\r\n", type_size[i],
              type_size[i] * 4, i);
  }
  dbg_print(u"\r\n");
  while (k_uefi.ST->ConIn->ReadKeyStroke(k_uefi.ST->ConIn, &Key) ==
         EFI_NOT_READY)
    ;
}
#endif
