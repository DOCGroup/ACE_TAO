/* $Id$ -*- C -*-

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/locale.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#ifndef PACE_LOCALE_H
#define PACE_LOCALE_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/locale.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/locale.h"
#elif (PACE_WIN32)
# include "pace/win32/locale.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

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
#  if (PACE_HAS_POSIX)
#    include "pace/posix/locale.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/locale.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/locale.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#endif /* PACE_LOCALE_H */
