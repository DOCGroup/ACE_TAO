/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/utime.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_UTIME_H
#define PACE_UTIME_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/utime.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/utime.h"
#elif (PACE_WIN32)
# include "pace/win32/utime.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function utime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.6.6.
     */
#if (PACE_HAS_POSIX_FS_UOF)
  PACE_Export PACE_INLINE int pace_utime (const char * path,
                                          const pace_utimbuf * times);
#endif /* PACE_HAS_POSIX_FS_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/utime.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/utime.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/utime.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_UTIME_H */
