#ifndef KH_KALLOC_H
/** ****************************************************************************
`kmem_*` function have two repsonsibilities:
 * To manage non-virtual memory allocation / deallocation.
 * To handle memory maintenence background tasks (zeroing / defragmentation)
***************************************************************************** */
#define KH_KALLOC_H
#include "types.h"
/**
Allocates general memory of **maximum** `size` pages and sets `size` to the
actual number of pages allocated.
*/
void *kmem_alloc(size_t *size);
/**
Adds general memory to the free memory pool.
*/
void kmem_free(void *mem, size_t size);

/**
Allocates system reserved memory of **maximum** `size` pages and sets `size` to
the actual number of pages allocated.
*/
void *kmem_sys_alloc(size_t *size);
/**
Adds system reserved memory to the free memory pool.
*/
void kmem_sys_free(void *mem, size_t size);
/**
Performs mentanence tasks.

Returns 1 if tasks were performed.
Returns 0 if there are no tasks to perform.
*/
int kmem_sys_maintain(void);

#endif
