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

# define PACE_ERRNO_NO_SUPPORT_RETURN(FAILVALUE) \
return ( (errno = ENOTSUP), FAILVALUE)

# define PACE_ERRNO_NO_SUPPORT() errno=ENOTSUP

#endif /* PACE_CONFIG_UTILITY_H */
