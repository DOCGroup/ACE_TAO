/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    codes.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_CONFIG_CODES_H
#define PACE_CONFIG_CODES_H


/* Generic lack of support. */
# define PACE_NO_PLATFORM_SUPPORT -5000

/* Specific lack of support. */
# define PACE_ERRNO_LACKS_REENTRANT -5001
# define PACE_ERRNO_LACKS_POSIX_PTHREAD_SEMANTICS -5002
# define PACE_ERRNO_LACKS_POSIX_C_SOURCE -5003
# define PACE_ERRNO_LACKS_RT -5004


#endif /* PACE_CONFIG_CODES_H */
