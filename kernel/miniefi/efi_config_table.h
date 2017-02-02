#ifndef H_EFI_CONFIG_TABLE_H
#define H_EFI_CONFIG_TABLE_H
#include "efi_required.h"

#define EFI_PROPERTIES_TABLE_VERSION 0x00010000

/* ******************************************************
** EFI Config Table related definitions
****************************************************** */

#define EFI_ACPI_20_TABLE_GUID                                                 \
  {                                                                            \
    0x8868e871, 0xe4f1, 0x11d3, {                                              \
      0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81                           \
    }                                                                          \
  }
#define ACPI_TABLE_GUID                                                        \
  {                                                                            \
    0xeb9d2d30, 0x2d88, 0x11d3, {                                              \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
#define SAL_SYSTEM_TABLE_GUID                                                  \
  {                                                                            \
    0xeb9d2d32, 0x2d88, 0x11d3, {                                              \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
#define SMBIOS_TABLE_GUID                                                      \
  {                                                                            \
    0xeb9d2d31, 0x2d88, 0x11d3, {                                              \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
#define SMBIOS3_TABLE_GUID                                                     \
  {0xf2fd1544, 0x9794, 0x4a2c,\
          {0x99,0x2e,0xe5,0xbb,0xcf,0x20,0xe3,0x94})
#define MPS_TABLE_GUID                                                         \
  {                                                                            \
    0xeb9d2d2f, 0x2d88, 0x11d3, {                                              \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
/* ACPI 2.0 or newer tables should use EFI_ACPI_TABLE_GUID */
#define EFI_ACPI_TABLE_GUID                                                    \
  {                                                                            \
    0x8868e871, 0xe4f1, 0x11d3, {                                              \
      0xbc, 0x22, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81                           \
    }                                                                          \
  }
#define ACPI_10_TABLE_GUID                                                     \
  {                                                                            \
    0xeb9d2d30, 0x2d88, 0x11d3, {                                              \
      0x9a, 0x16, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d                           \
    }                                                                          \
  }
#define EFI_PROPERTIES_TABLE_GUID                                              \
  {                                                                            \
    0x880aaca3, 0x4adc, 0x4a04, {                                              \
      0x90, 0x79, 0xb7, 0x47, 0x34, 0x8, 0x25, 0xe5                            \
    }                                                                          \
  }

/* BIT 0 – description – implies the runtime data is separated from the code */
#define EFI_PROPERTIES_RUNTIME_MEMORY_PROTECTION_NON_EXECUTABLE_PE_DATA 0x1

#define EFI_MEMORY_ATTRIBUTES_TABLE_GUID                                       \
  {                                                                            \
    0xdcfa911d, 0x26eb, 0x469f, {                                              \
      0xa2, 0x20, 0x38, 0xb7, 0xdc, 0x46, 0x12, 0x20                           \
    }                                                                          \
  }
/* ******************************************************
** EFI Config Table Types
****************************************************** */
typedef struct {
  EFI_GUID VendorGuid;
  void *VendorTable;
} EFI_CONFIGURATION_TABLE;

typedef struct {
  uint32_t Version;
  uint32_t Length;
  uint64_t MemoryProtectionAttribute;
} EFI_PROPERTIES_TABLE;

#endif
