/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/sched.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SCHED_H
#define PACE_SCHED_H

#include <sched.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_sched_get_priority_max (int policy);

  PACE_INLINE int pace_sched_get_priority_min (int policy);

  PACE_INLINE int pace_sched_getparam (pid_t pid,
                                       struct sched_param * param);

  PACE_INLINE int pace_sched_rr_get_interval (pid_t pid,
                                              struct timespec * interval);

  PACE_INLINE int pace_sched_setparam (pid_t pid,
                                       const struct sched_param * param);

  PACE_INLINE int pace_sched_getscheduler (pid_t pid);

  PACE_INLINE int pace_sched_setscheduler (pid_t pid,
                                           int policy,
                                           const struct sched_param * param);

  PACE_INLINE int pace_sched_yield ();

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/sched.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SCHED_H */
