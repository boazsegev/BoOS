#include "miniefi/sys.h"

extern EFI_STATUS efi_main(EFI_HANDLE handle,
                           EFI_SYSTEM_TABLE *SystemTable) EFIAPI;

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable) {
  (void)ImageHandle;
  EFI_STATUS Status = 0;
  EFI_INPUT_KEY Key;

  if (k_system_init(SystemTable))
    return EFI_LOAD_ERROR;
  /* print memory map data */
  {
    size_t pages = 0;
    size_t type_size[16] = {0};
    EFI_MEMORY_DESCRIPTOR *mem_desc;
    dbg_print(u"Testing printing for\r\n"
              u"String: %s\r\n"
              u"int: %i uint: %u long %li\r\npointer: %p\r\n",
              u"a little test", (int32_t)1, (uint32_t)42, (int64_t)(-73),
              (void *)0xFFEEDD1234000000);
    dbg_print(u"Memory Map is %i items long\r\n",
              (k_system_resources.memmap.map_size /
               k_system_resources.memmap.descriptor_size));
    for (size_t i = 0; i < (k_system_resources.memmap.map_size /
                            k_system_resources.memmap.descriptor_size);
         i++) {
      mem_desc = (void *)((uintptr_t)(k_system_resources.memmap.map) +
                          (i * k_system_resources.memmap.descriptor_size));
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
      // while ((Status = SystemTable->ConIn->ReadKeyStroke(
      //             SystemTable->ConIn, &Key)) == EFI_NOT_READY)
      //   ;
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
  }
  // SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
  SystemTable->ConOut->SetAttribute(
      SystemTable->ConOut, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BACKGROUND_BLACK));
  SystemTable->ConOut->OutputString(SystemTable->ConOut,
                                    u"Loading BoOS...\r\n");
  SystemTable->ConOut->EnableCursor(SystemTable->ConOut, 0);
  SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, 5, 10);
  while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn,
                                                     &Key)) == EFI_NOT_READY)
    ;
  SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
  k_free_system_data();
  return 0;
}
