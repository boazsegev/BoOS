#include "miniefi/efi.h"
#include "vmem_map.h"

#include "dbg_print.h"

// #include "sys.h"

/* *****************************************************************************
System data storage
***************************************************************************** */
typedef struct {
  EFI_SYSTEM_TABLE *ST;
  EFI_HANDLE image_handle;
} k_uefi_s;

static k_uefi_s k_uefi;

/* *****************************************************************************
After the UEFI data is loaded and handled, the initialization will call the
`kmain` function, allowing the OS to take over.
***************************************************************************** */
#pragma weak kmain
int kmain(void) { return 0; }

/* *****************************************************************************
UEFI access for other objects.
***************************************************************************** */

/**
This function returns the address for the EFI_SYSTEM_TABLE.
*/
void *k_uefi_system_table(void) { return (void *)k_uefi.ST; }
/**
This function returns the address for the EFI_HANDLE.
*/
void *k_uefi_handle(void) { return (void *)k_uefi.image_handle; }

/* *****************************************************************************
Printing to the UEFI console.
***************************************************************************** */
void k_write2console(char16_t *str) {
  if (k_uefi.ST == NULL || k_uefi.ST->ConOut == NULL)
    return;
  k_uefi.ST->ConOut->OutputString(k_uefi.ST->ConOut, str);
}

/* *****************************************************************************
System initialization - this is where the UEFI application begins
***************************************************************************** */
EFI_STATUS EFIAPI __efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *ST) {
  k_uefi = (k_uefi_s){
      .ST = ST, .image_handle = ImageHandle,
  };
  if (!vmemory_map_initialize())
    goto error;
  k_write2console(u"BoOS loading finished for now...");
  EFI_INPUT_KEY Key;
  while (ST->ConIn->ReadKeyStroke(ST->ConIn, &Key) == EFI_NOT_READY)
    ;
  return 0;
error:
  dbg_print(u"ERROR: Something went wrong, BoOS cannot load!\r\n");
  return EFI_LOAD_ERROR;
}
