/*
 * $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace (posix ace)
 *
 * = FILENAME
 *    PACE_CONFIG_SUNOS_5.7.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef CONFIG_PACE_CONFIG_SUNOS_5_7_H
#define CONFIG_PACE_CONFIG_SUNOS_5_7_H


# define __EXTENSIONS__
# define PACE_EXTENSIONS

# define _REENTRANT
# define PACE_HAS_REENTRANT

# define _POSIX_PTHREAD_SEMANTICS
# define PACE_HAS_POSIX_PTHREAD_SEMANTICS

/* OS/PACE_mqueue.h */
# define PACE_HAS_RT

/* No wchar_t support. */
# if defined PACE_HAS_WCHAR
# undef PACE_HAS_WCHAR
# endif /* No wchar_t support. */

/* Sun has no wchar_t support. */
# if defined PACE_USES_WCHAR
# undef PACE_USES_WCHAR
# endif /* No wchar_t support. */

/* !PACE_HAS_SIN_LEN */
/* # define PACE_HAS_SIN_LEN */


#endif /* CONFIG_PACE_CONFIG_SUNOS_5_7_H */
