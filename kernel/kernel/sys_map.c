#include "sys_map.h"
#include "debug.h"

k_system_resources_s k_collect_system_data(EFI_SYSTEM_TABLE *ST) {
  k_system_resources_s data = {
      .ST = ST, .memmap.map_size = 0, .memmap.map = NULL};
  EFI_STATUS ret = 0;
  dbg_print(ST, u"Checking Memory Map requirements.\r\n");
  ret = ST->BootServices->GetMemoryMap(
      &data.memmap.map_size, data.memmap.map, &data.memmap.map_key,
      &data.memmap.descriptor_size, &data.memmap.descriptor_Version);
  if (ret != EFI_BUFFER_TOO_SMALL)
    goto error;
  /* We need to loop this part, because we are allocating memory, which updates
   * the memory map... which means we need to ask for more memory, and the cycle
   * goes on.*/
  for (size_t i = 0; i < 5; i++) {
    if (data.memmap.map != NULL)
      ST->BootServices->FreePool(data.memmap.map);
    if (data.memmap.map_size > 0) {
      dbg_print(ST, u"Allocating memory to store the Memory Map.\r\n");
      ret = ST->BootServices->AllocatePool(EfiLoaderData, data.memmap.map_size,
                                           (void **)(&data.memmap.map));
      if (ret)
        goto error;
      dbg_print(ST, u"Loading the Memory Map.\r\n");
      ret = ST->BootServices->GetMemoryMap(
          &data.memmap.map_size, data.memmap.map, &data.memmap.map_key,
          &data.memmap.descriptor_size, &data.memmap.descriptor_Version);
      if (ret == EFI_BUFFER_TOO_SMALL)
        continue;
      else if (ret)
        goto error;
      else
        break;
    };
  }
  return data;
error:
  if (data.memmap.map != NULL)
    ST->BootServices->FreePool(data.memmap.map);
  data = (k_system_resources_s){.ST = NULL};
  dbg_print(ST, u"ERROR: Memory Map query error %p!\r\n", ret);
  return data;
}

void k_free_system_data(k_system_resources_s *data) {
  if (data->ST && data->ST->BootServices && data->memmap.map)
    data->ST->BootServices->FreePool(data->memmap.map);
}
