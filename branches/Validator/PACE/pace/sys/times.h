/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/times.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TIMES_H
#define PACE_SYS_TIMES_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/times.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/times.h"
#elif (PACE_WIN32)
# include "pace/win32/times.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE pace_clock_t pace_times (pace_tms * buffer);
#endif /* PACE_HAS_POSIX_MP_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/times.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/times.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/times.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TIMES_H */
