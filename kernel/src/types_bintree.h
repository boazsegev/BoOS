#ifndef H_TYPES_BINTREE_H
#define H_TYPES_BINTREE_H
#include "types.h"

/** ****************************************************************************
A 64 bit hash value binary tree... this is the API.
***************************************************************************** */

/** A 64 bit hash based binary tree */
typedef struct bintree_node_64bit_s bt_64bit_node_s;
struct bintree_node_64bit_s {
  /** The next item in the tree who's hash value is smaller. */
  bt_64bit_node_s *smaller;
  /** The next item in the tree who's hash value is higher. */
  bt_64bit_node_s *higher;
  /** The next item in the tree who's hash value is equal. */
  bt_64bit_node_s *equal;
  /** makes each node self aware as to it's position. */
  bt_64bit_node_s **spot;
  /** a temporary data feild used internally to flatten the tree. */
  bt_64bit_node_s *tmp;
  /** The item's hash value. */
  uint64_t hash;
};

#define BINARY_TREE_INIT_HASH(hash_value)                                      \
  (bt_64bit_node_s) { .hash = (hash_value) }
/**
  * adds `item` (assumes a **leaf** node) to the binary tree starting at `root`.
  *
*/
static inline void __attribute__((unused))
bt_64bit_add(bt_64bit_node_s **root, bt_64bit_node_s *item);
/**
  * adds an item to the binary tree that starts in the `root` node UNLESS one
  * already exists for the same hash value.
  *
  * returns the item that currently holds the hash_value - either the newly
  * added item or the older one.
  *
  * returns NULL if an error occured.
*/
static inline void *__attribute__((unused))
bt_64bit_add_or_find(bt_64bit_node_s **root, bt_64bit_node_s *item);

/**
  * Removes an item from the binary tree it belongs to.
*/
static inline void __attribute__((unused))
bt_64bit_remove(bt_64bit_node_s *item);

/**
  * Finds an exact match to the requested `hash_value`. return NULL if non.
*/
static inline void *__attribute__((unused))
bt_64bit_find(bt_64bit_node_s *root, uint64_t hash_value);
/**
  * Finds the parent that will be assigned to a new item with `hash_value`.
  *
  * If an exact match exists, the exact match will be returned.
  *
  * If the binary tree is empty, NULL is returned.
*/
static inline void *__attribute__((unused))
bt_64bit_find_parent(bt_64bit_node_s *root, uint64_t hash_value);

/**
  * Creates a temporary flat linked list starting at `root` (next: `root->tmp`).
  *
  * Any `bt_64bit_*` function that might perform a change to the tree could
 * destroty the temporary linked list's structure.
*/
static inline void __attribute__((unused))
bt_64bit_tmp2list(bt_64bit_node_s *root);
/**
  * returns the `next` item in a temporary list.
*/
static inline void *__attribute__((unused))
bt_64bit_tmp_list_next(bt_64bit_node_s *item);

/** ****************************************************************************
A 64 bit hash value binary tree... this is the implementation.
***************************************************************************** */

/** adds an item to the binary tree that starts in the `root` node. */
static inline void __attribute__((unused))
bt_64bit_add(bt_64bit_node_s **root, bt_64bit_node_s *item) {
  bt_64bit_node_s *existing = bt_64bit_add_or_find(root, item);
  if (existing == item)
    return;
  /* make sure the item isn't already placed in the list */
  while (existing->equal) {
    if (existing->equal == item) {
      /*
      fprintf(stderr, "ERROR: tried to add existing item twice? %p\n", item);
      */
      return;
    }
    existing = existing->equal;
  }

  /* add the item to to equal linked list */
  existing->equal = item;
  item->spot = &existing->equal;

  /* unless `item` was a branch, we're done */
  if (!(item->higher || item->smaller)) {
    return;
  }
  /*
   * This is bad teritory, we really shouldn't get here... but... life happens.
   * we'll flatten what's left and re-insert it.
  */
  bt_64bit_node_s *tmp;
  /* faltten the items */
  bt_64bit_tmp2list(item);
  /* run through the flattened stack and push it to the tree. */
  while (item->tmp) {
    tmp = item->tmp;
    item->tmp = tmp->tmp;
    *tmp = (bt_64bit_node_s){.hash = tmp->hash};
    existing = bt_64bit_add_or_find(root, tmp);
    if (existing == tmp)
      continue;
    tmp->equal = existing->equal;
    existing->equal = tmp;
    tmp->spot = &existing->equal;
    if (tmp->equal)
      tmp->equal->spot = &tmp->equal;
  }
}

/** adds an item to ... OR finds it ... */
static inline void *__attribute__((unused))
bt_64bit_add_or_find(bt_64bit_node_s **root, bt_64bit_node_s *item) {
  /* empty list */
  if (!*root) {
    *root = item;
    item->spot = root;
    return item;
  }

  bt_64bit_node_s *parent = bt_64bit_find_parent(*root, item->hash);
  /* parent is always exists unless list is empty */
  if (parent->hash == item->hash)
    return parent;

  else if (item->hash < parent->hash) {
    if (parent->smaller)
      return NULL;
    item->spot = &parent->smaller;
    parent->smaller = item;
    return item;
  }
  /* item->hash >  parent->hash */
  if (parent->higher)
    return NULL;
  item->spot = &parent->higher;
  parent->higher = item;
  return item;
}

/** Removes an item from the binary tree. */
static inline void __attribute__((unused))
bt_64bit_remove(bt_64bit_node_s *item) {
  /*
    bt_64bit_node_s **root = item->spot;
    bt_64bit_node_s *tmp;
    bt_64bit_tmp2list(item);
    item = item->tmp;
    *root = NULL;
    while (item) {
      tmp = item;
      item = item->tmp;
      *tmp = (bt_64bit_node_s){.hash = tmp->hash};
      fprintf(stderr, "adding %p to root\n", tmp);
      bt_64bit_add(root, tmp);
    }
  */
  bt_64bit_node_s **root = item->spot;
  if ((*root = item->equal))
    item->equal->spot = root;

  if (item->smaller)
    bt_64bit_add(root, item->smaller);
  if (item->higher)
    bt_64bit_add(root, item->higher);
  *item = (bt_64bit_node_s){.hash = item->hash};
  return;
}

/** Finds an exact match to the requested `hash_value`. return NULL if non. */
static inline void *__attribute__((unused))
bt_64bit_find(bt_64bit_node_s *root, uint64_t hash_value) {
  bt_64bit_node_s *item = bt_64bit_find_parent(root, hash_value);
  if (item && item->hash == hash_value)
    return item;
  return NULL;
}

/** Finds the nearest match ... */
static inline void *__attribute__((unused))
bt_64bit_find_parent(bt_64bit_node_s *root, uint64_t hash_value) {
  if (!root)
    return NULL;
check_root:
  if (root->hash == hash_value)
    return root;
  if (root->hash > hash_value) {
    /* if the smaller one exists, nest inside of it*/
    if (root->smaller) {
      root = root->smaller;
      goto check_root;
    }
    return root;
  } else {
    /* if the higher one exists, nest inside of it*/
    if (root->higher) {
      root = root->higher;
      goto check_root;
    }
    return root;
  }

  return NULL;
}

/** helper macro for the `bt_64bit_tmp2list` function */
#define BT_64_FLAT_ADD_TO(target, itm)                                         \
  {                                                                            \
    (itm)->tmp = (target);                                                     \
    (target) = (itm);                                                          \
  }
/**
  * Creates a temporary flat linked list starting at `root` (next: `root->tmp`).
*/
static inline void __attribute__((unused))
bt_64bit_tmp2list(bt_64bit_node_s *root) {
  if (!root)
    return;
  bt_64bit_node_s *to_flat = NULL;
  bt_64bit_node_s *tmp = NULL;
  root->tmp = NULL;
  if (root->higher)
    BT_64_FLAT_ADD_TO(to_flat, root->higher);
  if (root->smaller)
    BT_64_FLAT_ADD_TO(to_flat, root->smaller);
  if (root->equal)
    BT_64_FLAT_ADD_TO(to_flat, root->equal);

  while (to_flat) {
    /* rotate it all */
    tmp = to_flat;
    to_flat = to_flat->tmp;
    tmp->tmp = root->tmp;
    root->tmp = tmp;

    while (tmp) {
      if (tmp->higher)
        BT_64_FLAT_ADD_TO(to_flat, tmp->higher);
      if (tmp->smaller)
        BT_64_FLAT_ADD_TO(to_flat, tmp->smaller);
      tmp = tmp->equal;
    }
  }
  return;
}
#undef BT_64_FLAT_ADD_TO

/**
  * returns the `next` item in a temporary list.
*/
static inline void *__attribute__((unused))
bt_64bit_tmp_list_next(bt_64bit_node_s *item) {
  return item->tmp;
}

#endif /* H_TYPES_BINTREE_H */
