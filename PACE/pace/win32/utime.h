/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/utime.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_UTIME_H
#define PACE_UTIME_H

#include "pace/sys/types.h"
#include <utime.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_UTIMBUF
#define PACE_UTIMBUF
typedef struct utimbuf pace_utimbuf;
#endif /* PACE_UTIMBUF */

  /**
     PACE's implementation of the POSIX function utime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.6.6.
   */
  PACE_INLINE int pace_utime (const char * path,
                              const pace_utimbuf * times);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/utime.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_UTIME_H */
