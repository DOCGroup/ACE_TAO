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

#if !defined (PACE_HAS_POSIX_C_SOURCE)
# if defined (_POSIX_C_SOURCE)
#   define PACE_HAS_POSIX_C_SOURCE _POSIX_C_SOURCE
# endif /* _POSIX_C_SOURCE */
#endif /* ! PACE_HAS_POSIX_C_SOURCE */

#if !defined (PACE_HAS_EXTENSIONS)
# if defined (__EXTENSIONS__)
#   define PACE_HAS_EXTENSIONS __EXTENSIONS__
# endif /* __EXTENSIONS__ */
#endif /* ! PACE_HAS_EXTENSIONS */

#if !defined (PACE_HAS_REENTRANT)
# if defined (PACE_LINUX)
#   ifdef __USE_POSIX
#     define PACE_HAS_REENTRANT 1
#   else  /* ! __USE_POSIX */
#   endif /* ! __USE_POSIX */
# elif defined (PACE_LYNXOS)
    // LynxOS uses _POSIX_REENTRANT_FUNCTIONS, but not _REENTRANT
# elif defined (PACE_SUNOS)
#   if defined (__EXTENSIONS__) || defined (_REENTRANT) || \
       (_POSIX_C_SOURCE - 0 >= 199506L)
#     define PACE_HAS_REENTRANT 1
#   else  /* ! __EXTENSIONS__ && ! _REENTRANT && _POSIX_C_SOURCE < 199506L */
#   endif /* ! __EXTENSIONS__ && ! _REENTRANT && _POSIX_C_SOURCE < 199506L */
# else  /* other POSIX */
#   define PACE_HAS_REENTRANT 1
# endif /* other POSIX */
#endif /* ! PACE_HAS_REENTRANT */

#if !defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
# if defined (_POSIX_PTHREAD_SEMANTICS)
#   define PACE_HAS_POSIX_PTHREAD_SEMANTICS _POSIX_PTHREAD_SEMANTICS
# endif /* _POSIX_PTHREAD_SEMANTICS */
#endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */

#endif /* PACE_CONFIG_CONFIG_H */
