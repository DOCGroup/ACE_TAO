/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sched.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SCHED_H
#define PACE_SCHED_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/sched.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/sched.h"
#elif (PACE_WIN32)
# include "pace/win32/sched.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function sched_get_priority_max.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.3.6.
   */
  PACE_INLINE int pace_sched_get_priority_max (int policy);

  /**
     PACE's implementation of the POSIX function sched_get_priority_min.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.3.6.
   */
  PACE_INLINE int pace_sched_get_priority_min (int policy);

  /**
     PACE's implementation of the POSIX function sched_getparam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.3.2.
   */
  PACE_INLINE int pace_sched_getparam (pace_pid_t pid,
                                       pace_sched_param * param);

  /**
     PACE's implementation of the POSIX function sched_rr_get_interval.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.3.6.
   */
  PACE_INLINE int pace_sched_rr_get_interval (pace_pid_t pid,
                                              pace_timespec * interval);

  /**
     PACE's implementation of the POSIX function sched_setparam.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.3.1.
   */
  PACE_INLINE int pace_sched_setparam (pace_pid_t pid,
                                       const pace_sched_param * param);

  /**
     PACE's implementation of the POSIX function sched_getscheduler.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.3.4.
   */
  PACE_INLINE int pace_sched_getscheduler (pace_pid_t pid);

  /**
     PACE's implementation of the POSIX function sched_setscheduler.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.3.3.
   */
  PACE_INLINE int pace_sched_setscheduler (pace_pid_t pid,
                                           int policy,
                                           const pace_sched_param * param);

  /**
     PACE's implementation of the POSIX function sched_yield.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 13.3.5.
   */
  PACE_INLINE int pace_sched_yield ();

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/sched.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/sched.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/sched.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SCHED_H */
