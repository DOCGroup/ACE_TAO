/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/locale.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#ifndef PACE_LOCALE_H
#define PACE_LOCALE_H

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_LC_ALL LC_ALL
#define PACE_LC_COLLATE LC_COLLATE
#define PACE_LC_CTYPE LC_CTYPE
#define PACE_LC_MONETARY LC_MONETARY
#define PACE_LC_NUMERIC LC_NUMERIC
#define PACE_LC_TIME LC_TIME
#define PACE_NULL NULL

#ifndef PACE_LCONV
#define PACE_LCONV
typedef struct lconv pace_lconv;
#endif /* PACE_LCONV */

  /**
     Fills in pace_lconv with values consistent with the
     current locality.
   */
  PACE_INLINE pace_lconv* pace_localeconv (void);

  /**
     PACE's implementation of the POSIX function setlocale.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.2.
   */
  PACE_INLINE char * pace_setlocale (int category, const char * locale);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/locale.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_LOCALE_H */
