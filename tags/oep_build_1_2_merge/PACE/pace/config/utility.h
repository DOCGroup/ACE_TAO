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

#if defined (PACE_TRACING) && PACE_TRACING == 1
# include <stdio.h>
#endif

#include "pace/errno.h"

# define PACE_ERRNO_NO_SUPPORT_RETURN(FAILVALUE) \
return ( (errno = ENOTSUP), FAILVALUE)

# define PACE_ERRNO_NO_SUPPORT() errno=ENOTSUP

/* ----------------------------------------------------------------------
   A couple useful inline functions for checking whether bits are
   enabled or disabled.
 * ---------------------------------------------------------------------- */

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

/* ----------------------------------------------------------------------
   Specific for (PACE_WIN32)
 * ---------------------------------------------------------------------- */

#if (PACE_WIN32)

#include <windows.h>

/* Perform a mapping of Win32 error numbers into POSIX errnos. */
# define PACE_FAIL_RETURN(RESULT) do { \
  switch (GetLastError ()) { \
  case ERROR_NOT_ENOUGH_MEMORY: errno = ENOMEM; break; \
  case ERROR_FILE_EXISTS:       errno = EEXIST; break; \
  case ERROR_SHARING_VIOLATION: errno = EACCES; break; \
  case ERROR_PATH_NOT_FOUND:    errno = ENOENT; break; \
  } \
  return RESULT; } while (0)

/* The "null" device on Win32. */
# define PACE_DEV_NULL "nul"

/* Define the pathname separator characters for Win32 (ugh). */
# define PACE_DIRECTORY_SEPARATOR_STR "\\"
# define PACE_DIRECTORY_SEPARATOR_CHAR '\\'
# define PACE_LD_SEARCH_PATH "PATH"
# define PACE_LD_SEARCH_PATH_SEPARATOR_STR ";"
# define PACE_DLL_SUFFIX ".dll"
# define PACE_DLL_PREFIX ""

# define PACE_SYSCALL_FAILED 0xFFFFFFFF

/* Turns "FALSE" into -1 */
# define PACE_ADAPT_RETVAL(OP,RESULT) \
  ((RESULT = (OP)) == FALSE ? -1 : 0)

/* If failure, sets errno before returning */
# define PACE_WIN32CALL_RETURN(X,TYPE,FAILVALUE) \
  do { \
    TYPE pace_result_; \
    pace_result_ = (TYPE) X; \
    if (pace_result_ == FAILVALUE) \
    { \
      errno = GetLastError (); \
    } \
    return pace_result_; \
  } while (0)

/* Casts return value. */
# define PACE_OSCALL_RETURN(X,TYPE,FAILVALUE) \
  return (TYPE) X;

#endif /* PACE_WIN32 */

/* ----------------------------------------------------------------------
 * debug tracing
 * ---------------------------------------------------------------------- */

#if defined (PACE_TRACING) && PACE_TRACING == 1
# define PACE_TRACE(MODULE) \
fprintf(stderr, "PACE_TRACE %s, %d, %s\n", (MODULE), __LINE__, __FILE__)
#else
# define PACE_TRACE(MODULE)
#endif /* TRACING */

#endif /* PACE_CONFIG_UTILITY_H */
