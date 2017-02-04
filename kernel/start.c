#include "kernel/debug.h"
#include "kernel/sys_map.h"
#include "miniefi/efi.h"

extern EFI_STATUS efi_main(EFI_HANDLE handle,
                           EFI_SYSTEM_TABLE *SystemTable) EFIAPI;

k_system_resources_s system_resources;

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable) {
  (void)ImageHandle;
  EFI_STATUS Status = 0;
  EFI_INPUT_KEY Key;

  /* Store the system table for future use in other functions */
  // ST = SystemTable;
  // i_start();

  /* Say hi */
  // SystemTable->ConOut->SetMode(SystemTable->ConOut, 0);
  system_resources = k_collect_system_data(SystemTable);
  EFI_MEMORY_DESCRIPTOR *mem_desc;
  dbg_print(SystemTable, u"Testing printing for\r\n"
                         u"String: %s\r\n"
                         u"int: %i uint: %u long %li\r\npointer: %p\r\n",
            u"a little test", (int32_t)1, (uint32_t)42, (int64_t)(-73),
            (void *)0xFFEEDD1234000000);
  dbg_print(SystemTable, u"Memory Map is %i items long\r\n",
            (system_resources.memmap.map_size /
             system_resources.memmap.descriptor_size));
  for (size_t i = 0; i < (system_resources.memmap.map_size /
                          system_resources.memmap.descriptor_size);
       i++) {
    mem_desc = (void *)((uintptr_t)(system_resources.memmap.map) +
                        (i * system_resources.memmap.descriptor_size));
    dbg_print(system_resources.ST, u"Reading from memory at: %p\r\n", mem_desc);
    dbg_print(system_resources.ST, u"Memory Map Descriptor:\r\n"
                                   u"* Type: %i\r\n"
                                   u"* PhysicalStart: %p\r\n"
                                   u"* VirtualStart: %p\r\n"
                                   u"* NumberOfPages: %lu\r\n"
                                   u"* Attribute: %p\r\n\r\n",
              mem_desc->Type, mem_desc->PhysicalStart, mem_desc->VirtualStart,
              mem_desc->NumberOfPages, mem_desc->Attribute);
    while ((Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn,
                                                       &Key)) == EFI_NOT_READY)
      ;
  }
  if (system_resources.ST == NULL)
    return EFI_LOAD_ERROR;
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
  k_free_system_data(&system_resources);
  return 0;
}
