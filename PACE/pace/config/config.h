/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    config.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_CONFIG_CONFIG_H
#define PACE_CONFIG_CONFIG_H

# define PACE_HAS_INLINE

# if defined (_POSIX_C_SOURCE)
# define PACE_POSIX_C_SOURCE _POSIX_C_SOURCE
# endif /* _POSIX_C_SOURCE */

# if defined (__EXTENSIONS__)
# define PACE_EXTENSIONS __EXTENSIONS__
# endif

# if defined (_REENTRANT)
/* deprecated */
# define PACE_HAS_REENTRANT
/* current */
# define PACE_REENTRANT _REENTRANT
# endif

# if defined (_POSIX_PTHREAD_SEMANTICS)
/* deprecated */
# define PACE_HAS_POSIX_PTHREAD_SEMANTICS _POSIX_PTHREAD_SEMANTICS
/* current */
# define PACE_POSIX_PTHREAD_SEMANTICS _POSIX_PTHREAD_SEMANTICS
# endif

/* OS/PACE_mqueue.h */
# define PACE_HAS_RT

#endif /* PACE_CONFIG_CONFIG_H */


