#include "miniefi/efi.h"

extern EFI_STATUS efi_main(EFI_HANDLE handle,
                           EFI_SYSTEM_TABLE *SystemTable) EFIAPI;

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
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
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
  return 0;
}
