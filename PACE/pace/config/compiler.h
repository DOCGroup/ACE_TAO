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


# if defined (_POSIX_C_SOURCE)
# define PACE_POSIX_C_SOURCE _POSIX_C_SOURCE
# endif /* _POSIX_C_SOURCE */

# if defined (__cplusplus)
# define PACE_INLINE inline
# define PACE_HAS_CPLUSPLUS
# else  /* ! __cplusplus */
# define PACE_INLINE static
# endif /* ! __cplusplus */

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
