/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/locale.h
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
# include "pace/posix/locale.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_LOCALE_H */
