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

/* specific to win32 */

#if !defined (ENOSYS)
# define ENOSYS EFAULT /* Operation not supported or unknown error. */
#endif /* !ENOSYS */

#if !defined (ENOTSUP)
# define ENOTSUP ENOSYS  /* Operation not supported. */
#endif /* !ENOTSUP */

# define PACE_ERRNO_NO_SUPPORT_RETURN(FAILVALUE) \
return ( (errno = ENOTSUP), FAILVALUE)

# define PACE_ERRNO_NO_SUPPORT() errno=ENOTSUP

#endif /* PACE_CONFIG_UTILITY_H */
