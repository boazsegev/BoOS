#include "kalloc.h"
#include "spinlock.h"
#include "types_bintree.h"
#include "vmem_map.h" /* MEMORY_PAGE_SIZE , MEMORY_PAGE_BIT_SHIFT*/
/* *****************************************************************************
Data Structures
***************************************************************************** */

/**
A memory segment.
*/
typedef struct kmem_segment_s {
  /**
    * Inherit the bintree `node` properties.
    */
  bt_64bit_node_s node;
  /** The first byte outside the memory segment:
    *    total_size = vmem_segment_pt->end - (uintptr_t)vmem_segment_pt
    */
  uintptr_t end;
  /** The address of the next byte to zero out. */
  uintptr_t zero;
} kmem_segment_s;

/**
The memory "free list".
*/
typedef struct {
  kmem_segment_s *available;
  spn_lock_i lock;
} kmem_store_s;

static kmem_store_s general_mem_store = {};
static kmem_store_s system_mem_store = {};

/**
Defragments a memory store.

Returns 1 if there's more work to be done and 0 if the memory is defragmented.
*/
static inline int kmem_defragment_store_unsafe(kmem_store_s *store) {

  kmem_segment_s *pos;
  kmem_segment_s *node;
  bt_64bit_tmp2list(&store->available->node);
  pos = store->available;
  if (!pos)
    return 0;

  while (pos) {
    node = store->available;
    while (node) {
      if (node == pos) {
        node = bt_64bit_tmp_list_next(&node->node);
        continue;
      }
      if (
          /* these are connected segments */
          ((uintptr_t)node == pos->end)
          /* or these segments overlap.
           * (this might be an error except during initialization)
           */
          || ((uintptr_t)node < pos->end && node > pos)
          /* It could have been re-written as: */
          /* ((uintptr_t)node <= (*pos)->end && node > (*pos)) */
          ) {
        /* update range if applicable (overlapping segments might be different)
         */
        if (node->end > pos->end)
          pos->end = node->end;
        /* remove from the list. */
        bt_64bit_remove(&node->node);
        /* Stop using cycles, just notify that there's more work to be done. */
        return 1;
      }
    }
    pos = bt_64bit_tmp_list_next(&pos->node);
  }
  return 0;
}

/**
Zeros out the requested number of pages.
*/
static inline void __attribute__((unused))
kmem_zero_pages(size_t *addr, size_t *limit) {
  while (addr < limit) {
    *addr = 0;
    ++addr;
  }
}

/**
Adds a memory segment to a memory store.
`addr` is the address of the memory segment.
`pages` is the number of pages to add.
*/
static inline void __attribute__((unused))
kmem_checkin(kmem_store_s *store, void *addr, size_t pages, uintptr_t is_zero) {
  kmem_segment_s *seg = addr;
  seg->end = ((uintptr_t)addr + (pages << MEMORY_PAGE_BIT_SHIFT));
  seg->zero = is_zero;
  seg->node = (bt_64bit_node_s){.hash = pages};
  spn_lock(&store->lock);
  bt_64bit_add((void *)&(store->available), &seg->node);
  spn_unlock(&store->lock);
}

/**
Collectes memory from a memory store.
`addr` is the address of the memory segment.
`pages` is the number of pages to add.
*/
static inline void *__attribute__((unused))
kmem_checkout(kmem_store_s *store, size_t *pages) {

  uintptr_t ask_size = *pages << MEMORY_PAGE_BIT_SHIFT;
  kmem_segment_s *parent;
  spn_lock(&store->lock);
  parent = bt_64bit_find_parent(&store->available->node, *pages);
  if (!parent)
    goto none;
  if (parent->end - (uintptr_t)parent == ask_size)
    goto equal;
  if (parent->end - (uintptr_t)parent < ask_size) {
    *pages = (parent->end - (uintptr_t)parent) >> MEMORY_PAGE_BIT_SHIFT;
    goto equal;
  }
  /* slice the slice - we write the code twice to avoid lock-unlock */
  kmem_segment_s *seg = (void *)((uintptr_t)parent + ask_size);
  seg->end = parent->end;
  seg->zero = parent->zero;
  if (seg->zero < (uintptr_t)seg)
    seg->zero = (uintptr_t)seg;
  seg->node = (bt_64bit_node_s){
      .hash = ((seg->end - (uintptr_t)seg) >> MEMORY_PAGE_BIT_SHIFT)};
  parent->end = (uintptr_t)seg;
  bt_64bit_add((void *)&(store->available), &seg->node);

equal:
  bt_64bit_remove(&parent->node);
  spn_unlock(&store->lock);
  /* make sure the memory is zeroed out */
  if (parent && parent->zero < parent->end) {
    kmem_zero_pages((size_t *)parent->zero, (size_t *)parent->end);
  }
  return parent;
none:
  spn_unlock(&store->lock);
  *pages = 0;
  return NULL;
}

/**
Defragments a memory free list.
*/
static __attribute__((unused)) int kmem_defragment(void) {
  int ret = 0;
  spn_lock(&general_mem_store.lock);
  ret |= kmem_defragment_store_unsafe(&general_mem_store);
  spn_unlock(&general_mem_store.lock);
  spn_lock(&system_mem_store.lock);
  ret |= kmem_defragment_store_unsafe(&system_mem_store);
  spn_unlock(&system_mem_store.lock);
  return ret;
}

/**
Zeros out up to 128 pages at a time.
Returns 1 if there are more pages to to zero out and 0 if all pages are zero.
*/
static __attribute__((unused)) int kmem_zero_task(void) {
  size_t cycle_max = 128 << MEMORY_PAGE_BIT_SHIFT;
  kmem_segment_s *pos;
  if (!general_mem_store.available)
    return 0;
  spn_lock(&general_mem_store.lock);
  bt_64bit_tmp2list(&general_mem_store.available->node);
  pos = general_mem_store.available;
  while (pos) {
    if (pos->zero < pos->end) {
      if (pos->end - pos->zero >= cycle_max) {
        kmem_zero_pages((size_t *)pos->zero, (size_t *)(pos->zero + cycle_max));
        pos->zero += cycle_max;
        return 1;
      }
      cycle_max = cycle_max - (pos->end - pos->zero);
      kmem_zero_pages((size_t *)pos->zero, (size_t *)pos->end);
    }
    pos = bt_64bit_tmp_list_next(&pos->node);
  }
  spn_unlock(&general_mem_store.lock);
  return 0;
}

/* *****************************************************************************
API implementation
***************************************************************************** */

/**
Allocates general memory of **maximum** `size` pages and sets `size` to the
actual number of pages allocated.
*/
void *kmem_alloc(size_t *size) {
  return kmem_checkout(&general_mem_store, size);
}
/**
Adds general memory to the free memory pool.

*/
void kmem_free(void *mem, size_t size) {
  kmem_checkin(&general_mem_store, mem, size, (uintptr_t)mem);
}

/**
Allocates system reserved memory of **maximum** `size` pages and sets `size` to
the actual
number of pages allocated.
*/
void *kmem_sys_alloc(size_t *size) {
  return kmem_checkout(&system_mem_store, size);
}
/**
Adds system reserved memory to the free memory pool.
*/
void kmem_sys_free(void *mem, size_t size) {
  kmem_checkin(&system_mem_store, mem, size, (uintptr_t)mem);
}
/**
Performs mentanence tasks.

Returns 1 if tasks were performed.
Returns 0 if there are no tasks to perform.
*/
int kmem_sys_maintain(void) {
  if (kmem_defragment())
    return 1;
  return kmem_zero_task();
}
