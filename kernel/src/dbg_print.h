/* *****************************************************************************
Debug helpers
***************************************************************************** */
#ifndef H_DBG_PRINT_H
#define H_DBG_PRINT_H

#if DEBUG == 1
#include "types.h"

/**
`dbg_print` provides a very simple "printf"-like console printing facilities,
with support being limited to:

* %s - a string object.
* %i, %u - int32_t and uint32_t respectively, printed as decimal numbers.
* %li, %lu - int64_t and uint64_t respectively, printed as decimal numbers.
* %p - 64bit numbers and pointers printed in hex notation.

*/
void dbg_print(char16_t *str, ...);

#else /* DEBUG */

/** noop for dbg_print */
#define dbg_print(...)

#endif /* DEBUG */
#endif /* H_DBG_PRINT_H */
