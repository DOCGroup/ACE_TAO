/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    compiler.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_CONFIG_COMPILER_H
#define PACE_CONFIG_COMPILER_H


#include <errno.h>
int errno;


# if defined (__cplusplus)
# define PACE_INLINE inline
/* deprecated */
# define PACE_HAS_CPLUSPLUS __cplusplus
/* current */
# define PACE_CPLUSPLUS __cplusplus
# else  /* ! __cplusplus */
# define PACE_INLINE static
# endif /* ! __cplusplus */

/* ======================================================================
 * error control
 * ====================================================================== */

/* deprecated */
# define PACE_NO_PLATFORM_SUPPORT -5000

/* current */
# define PACE_ERRNO_NO_PLATFORM_SUPPORT -5000
# define PACE_ERRNO_LACKS_REENTRANT -5001
# define PACE_ERRNO_LACKS_POSIX_PTHREAD_SEMANTICS -5002
# define PACE_ERRNO_LACKS_POSIX_C_SOURCE -5003
# define PACE_ERRNO_LACKS_RT -5004

/* deprecated */
# define PACE_RETURN_NO_SUPPORT(X) \
do {                               \
  errno = ENOTSUP;                 \
  return -1;                       \
} while (0)

# define PACE_SET_ERRNO(X)         \
errno=X

# define PACE_SET_NO_SUPPORT()     \
errno=ENOTSUP

/* current */
# define PACE_ERRNO_RETURN_NO_SUPPORT(X) \
do {                                     \
  errno = ENOTSUP;                       \
  return -1;                             \
} while (0)

# define PACE_ERRNO_SET(X)         \
errno=X

# define PACE_ERRNO_SET_NO_SUPPORT()     \
errno=ENOTSUP

/** 
 * Save for later 
 *
 * __SUNPRO_CC defines Sun CC
 *
 */

#endif /* PACE_CONFIG_COMPILER_H */
