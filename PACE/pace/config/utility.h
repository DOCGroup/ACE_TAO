/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/config/utility.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_CONFIG_UTILITY_H
#define PACE_CONFIG_UTILITY_H

/* ----------------------------------------------------------------------
 * error control
 * ---------------------------------------------------------------------- */

#include "pace/errno.h"
int errno;

# define PACE_ERRNO_NO_SUPPORT_RETURN(FAILVALUE) \
return ( (errno = ENOTSUP), FAILVALUE)

# define PACE_ERRNO_NO_SUPPORT() errno=ENOTSUP

#include <windows.h>

// Perform a mapping of Win32 error numbers into POSIX errnos.
# define PACE_FAIL_RETURN(RESULT) do { \
  switch (GetLastError ()) { \
  case ERROR_NOT_ENOUGH_MEMORY: errno = ENOMEM; break; \
  case ERROR_FILE_EXISTS:       errno = EEXIST; break; \
  case ERROR_SHARING_VIOLATION: errno = EACCES; break; \
  case ERROR_PATH_NOT_FOUND:    errno = ENOENT; break; \
  } \
  return RESULT; } while (0)

/* A couple useful inline functions for checking whether bits are
   enabled or disabled.
 */
# define PACE_POW(X) (((X) == 0)?1:(X-=1,X|=X>>1,X|=X>>2,X|=X>>4,X|=X>>8,X|=X>>16,(++X)))
# define PACE_EVEN(NUM) (((NUM) & 1) == 0)
# define PACE_ODD(NUM) (((NUM) & 1) == 1)
# define PACE_BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)
# define PACE_BIT_DISABLED(WORD, BIT) (((WORD) & (BIT)) == 0)
# define PACE_BIT_CMP_MASK(WORD, BIT, MASK) (((WORD) & (BIT)) == MASK)
# define PACE_SET_BITS(WORD, BITS) (WORD |= (BITS))
# define PACE_CLR_BITS(WORD, BITS) (WORD &= ~(BITS))

/* Turn a number into a string. */
# define PACE_ITOA(X) #X

/* Create a string of a server address with a "host:port" format. */
# define PACE_SERVER_ADDRESS(H,P) H":"P

#endif /* PACE_CONFIG_UTILITY_H */
