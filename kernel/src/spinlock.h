#ifndef KH_SPINLOCK_H
#define KH_SPINLOCK_H
#include "types.h"

/** The spinlock volatile type. */
typedef volatile uint8_t spn_lock_i;

/*********
 * manage context switching while "waiting" for the lock to release
 * TODO: once we have concurrency, we must support this, or the CPU will spin
 *       uselessly.
 */
#define reschedule_thread()

#if !defined(__has_builtin)
#error Required feature check "__has_builtin" missing from this compiler's version.
#endif

#if __has_builtin(__sync_swap)
/** returns 1 and 0 if the lock was successfully aquired (TRUE == FAIL). */
static inline int spn_trylock(spn_lock_i *lock) { return __sync_swap(lock, 1); }
#elif __has_builtin(__sync_fetch_and_or)
/** returns 1 and 0 if the lock was successfully aquired (TRUE == FAIL). */
static inline int spn_trylock(spn_lock_i *lock) {
  return __sync_fetch_and_or(lock, 1);
}
#else
#error Required builtin "__sync_swap" or "__sync_fetch_and_or" missing from this compiler's version.
#endif

/** Releases a lock. */
static inline __attribute__((unused)) void spn_unlock(spn_lock_i *lock) {
  __asm__ volatile("" ::: "memory");
  *lock = 0;
}
/** returns a lock's state (non 0 == Busy). */
static inline __attribute__((unused)) int spn_is_locked(spn_lock_i *lock) {
  __asm__ volatile("" ::: "memory");
  return *lock;
}
/** Busy waits for the lock. */
static inline __attribute__((unused)) void spn_lock(spn_lock_i *lock) {
  while (spn_trylock(lock)) {
    reschedule_thread();
  }
}

#endif /* KH_SPINLOCK_H */
