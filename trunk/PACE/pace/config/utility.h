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

#include <errno.h>
int errno;

/* deprecated */
# define PACE_NO_PLATFORM_SUPPORT -5000

/* current */
# define PACE_ERRNO_NO_PLATFORM_SUPPORT -5000
# define PACE_ERRNO_LACKS_REENTRANT -5001
# define PACE_ERRNO_LACKS_POSIX_PTHREAD_SEMANTICS -5002
# define PACE_ERRNO_LACKS_POSIX_C_SOURCE -5003
# define PACE_ERRNO_LACKS_RT -5004

/* deprecated */
# define PACE_RETURN_NO_SUPPORT(X)       \
do {                                     \
  errno = ENOTSUP;                       \
  return -1;                             \
} while (0)

# define PACE_SET_ERRNO(X) errno=X

# define PACE_SET_NO_SUPPORT() errno=ENOTSUP

/* current */
# define PACE_ERRNO_SET_NO_SUPPORT_RETURN(FAILVALUE) \
do {                                                 \
  errno = ENOTSUP;                                   \
  return FAILVALUE;                                  \
} while (0)

# define PACE_ERRNO_SET(X) errno=X

# define PACE_ERRNO_SET_NO_SUPPORT() errno=ENOTSUP

#endif /* PACE_CONFIG_UTILITY_H */










