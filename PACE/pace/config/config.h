/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/config/config.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_CONFIG_CONFIG_H
#define PACE_CONFIG_CONFIG_H

#if defined (_POSIX_C_SOURCE)
# define PACE_HAS_POSIX_C_SOURCE _POSIX_C_SOURCE
#endif /* _POSIX_C_SOURCE */

#if defined (__EXTENSIONS__)
# define PACE_HAS_EXTENSIONS __EXTENSIONS__
# endif /* __EXTENSIONS__ */

#if defined (_REENTRANT)
# define PACE_HAS_REENTRANT
#endif /* _REENTRANT */

#if defined (_POSIX_PTHREAD_SEMANTICS)
# define PACE_HAS_POSIX_PTHREAD_SEMANTICS _POSIX_PTHREAD_SEMANTICS
#endif /* _POSIX_PTHREAD_SEMANTICS */

#endif /* PACE_CONFIG_CONFIG_H */
