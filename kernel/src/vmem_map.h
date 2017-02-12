#ifndef H_VMEM_MAP_H
/* *****************************************************************************
This will contain the object oriented interface related to memory map
operations.
***************************************************************************** */
#define H_VMEM_MAP_H
#include "types.h"

typedef struct vmem_map_s *vmem_map_pt;

/**
Creates the first (initial) memory map for the kernel.

Returns NULL on failure, otherwise returns a memory map for direct access memory
(no address translation).
*/
vmem_map_pt vmemory_map_initialize(void);
/**
Copies an existing memory map for a new process. Returns the copy.
*/
vmem_map_pt vmemory_map_copy(vmem_map_pt vmap);
/**
Switches the active virtual memory map to the requested memory map.
*/
int8_t vmemory_map_switch(vmem_map_pt vmap);
/**
Destroys the indicated memory map, freeing it's resources.
*/
int8_t vmemory_map_destroy(vmem_map_pt vmap);
/**
Allocates more memory for the memory map.

Returns the (virtual) address of the allocated memory

`size` is the number of pages to allocate (each page ~4Kib).
*/
void *vmemory_map_allocate(vmem_map_pt vmap, size_t size);
/**
Frees an allocated segment.

`seg` is the (virtual) address of the fisrt page to be freed.
`size` is the number of pages to free (each page ~4Kib).
*/
int8_t vmemory_map_free(vmem_map_pt vmap, void *seg, size_t size);
/**
Shares a memory segment between two memory maps.

Returns the (virtual) address of the memory in the new map.

`seg` is the (virtual) address of the fisrt page to be shared.
`size` is the number of pages to share (each page ~4Kib).
*/
void *vmemory_map_share(vmem_map_pt dest, vmem_map_pt source, void *seg,
                        size_t size);

#endif
