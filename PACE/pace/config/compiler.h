/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/config/compiler.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_CONFIG_COMPILER_H
#define PACE_CONFIG_COMPILER_H

#if defined (__cplusplus)
# define PACE_HAS_CPLUSPLUS
# define PACE_INLINE inline
#else  /* ! __cplusplus */
# if defined (__GNUC__)
#   if defined __OPTIMIZE__
#     define PACE_INLINE static
#   else /* ! __OPTIMIZE__ */
      /* To suppress warnings about unused static functions. */
#     define PACE_INLINE
#   endif /* __GNUC__ */
# else  /* ! __GNUC__ */
#   define PACE_INLINE static
# endif /* ! __GNUC__ */
#endif /* ! __cplusplus */ 

/* ============================================================================
 * Compiler Silencing macros
 *
 * Some compilers complain about parameters that are not used.  This macro
 * should keep them quiet.
 * ============================================================================
 */

#if defined (ghs) ||         \
    defined (__GNUC__) ||    \
    defined (__hpux) ||      \
    defined (__sgi) ||       \
    defined (__DECCXX) ||    \
    defined (__KCC) ||       \
    defined (__rational__)   \
    || (__USLC__)
/* Some compilers complain about "statement with no effect" with (a).
 * This eliminates the warnings, and no code is generated for the null
 * conditional statement.  NOTE: that may only be true if -O is enabled,
 * such as with GreenHills (ghs) 1.8.8.
 */
# define PACE_UNUSED_ARG(a) {if (&a)  { /* null */ }}
#else /* ! ghs || __GNUC__ || ..... */
# define PACE_UNUSED_ARG(a) (a)
#endif /* ! ghs || __GNUC__ || ..... */

#endif /* PACE_CONFIG_COMPILER_H */









