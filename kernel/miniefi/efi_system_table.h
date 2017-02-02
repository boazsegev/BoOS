#ifndef H_EFI_SYSTEM_TABLE_H
#define H_EFI_SYSTEM_TABLE_H
#include "efi_console.h"
#include "efi_runtime_services.h"

#include "efi_boot_services.h"

/* ******************************************************
** EFI System Table
****************************************************** */
typedef struct {
  EFI_TABLE_HEADER Hdr;
  uint16_t *FirmwareVendor;
  uint32_t FirmwareRevision;
  EFI_HANDLE ConsoleInHandle;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
  EFI_HANDLE ConsoleOutHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
  EFI_HANDLE StandardErrorHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
  EFI_RUNTIME_SERVICES *RuntimeServices;
  EFI_BOOT_SERVICES *BootServices;
  uintptr_t NumberOfTableEntries;
  EFI_CONFIGURATION_TABLE *ConfigurationTable;
} EFI_SYSTEM_TABLE;

#endif
