#ifndef H_EFI_IMAGE_ENTRY_H
#define H_EFI_IMAGE_ENTRY_H
#include "efi_system_table.h"

/* ******************************************************
** EFI images - Types and definitions
****************************************************** */

/* PE32+ Subsystem type for EFI images */
#define EFI_IMAGE_SUBSYSTEM_EFI_APPLICATION 10
#define EFI_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER 11
#define EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER 12

/* PE32+ Machine type for EFI images */
#define EFI_IMAGE_MACHINE_IA32 0x014c
#define EFI_IMAGE_MACHINE_IA64 0x0200
#define EFI_IMAGE_MACHINE_EBC 0x0EBC
#define EFI_IMAGE_MACHINE_x64 0x8664
#define EFI_IMAGE_MACHINE_ARMTHUMB_MIXED 0x01C2
#define EFI_IMAGE_MACHINE_AARCH64 0xAA64

/* The entry point function type */
typedef EFI_STATUS(EFIAPI *EFI_IMAGE_ENTRY_POINT)(
    EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);

#endif
