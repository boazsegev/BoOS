#include "sys.h"

k_system_resources_s k_system_resources;

#pragma weak dbg_print
void dbg_print(char16_t *str, ...) {
  (void)str;
  return;
}

ssize_t k_system_init(EFI_SYSTEM_TABLE *ST) {
  k_system_resources = (k_system_resources_s){
      .ST = ST, .memmap.map_size = 0, .memmap.map = NULL};
  EFI_STATUS ret = 0;
  dbg_print(u"Checking Memory Map requirements.\r\n");
  ret = ST->BootServices->GetMemoryMap(
      &k_system_resources.memmap.map_size, k_system_resources.memmap.map,
      &k_system_resources.memmap.map_key,
      &k_system_resources.memmap.descriptor_size,
      &k_system_resources.memmap.descriptor_Version);
  if (ret != EFI_BUFFER_TOO_SMALL)
    goto error;
  /* We need to loop this part, because we are allocating memory, which updates
   * the memory map... which means we need to ask for more memory, and the cycle
   * goes on.*/
  for (size_t i = 0; i < 5; i++) {
    if (k_system_resources.memmap.map != NULL)
      ST->BootServices->FreePool(k_system_resources.memmap.map);
    if (k_system_resources.memmap.map_size > 0) {
      dbg_print(u"Allocating memory to store the Memory Map.\r\n");
      ret = ST->BootServices->AllocatePool(
          EfiLoaderData, k_system_resources.memmap.map_size,
          (void **)(&k_system_resources.memmap.map));
      if (ret)
        goto error;
      dbg_print(u"Loading the Memory Map.\r\n");
      ret = ST->BootServices->GetMemoryMap(
          &k_system_resources.memmap.map_size, k_system_resources.memmap.map,
          &k_system_resources.memmap.map_key,
          &k_system_resources.memmap.descriptor_size,
          &k_system_resources.memmap.descriptor_Version);
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
  if (k_system_resources.memmap.map != NULL)
    ST->BootServices->FreePool(k_system_resources.memmap.map);
  k_system_resources = (k_system_resources_s){.ST = NULL};
  dbg_print(u"ERROR: Memory Map query error %p!\r\n", ret);
  return -1;
}

void k_free_system_data(void) {
  if (k_system_resources.ST && k_system_resources.ST->BootServices &&
      k_system_resources.memmap.map)
    k_system_resources.ST->BootServices->FreePool(
        k_system_resources.memmap.map);
  k_system_resources = (k_system_resources_s){.ST = NULL};
}
