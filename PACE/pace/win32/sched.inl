/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/sched.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

PACE_INLINE
int
pace_sched_get_priority_max (int policy)
{
  return sched_get_priority_max (policy);
}

PACE_INLINE
int
pace_sched_get_priority_min (int policy)
{
  return sched_get_priority_min (policy);
}

PACE_INLINE
int
pace_sched_getparam (pace_pid_t pid,
                     pace_sched_param * param)
{
  return sched_getparam (pid, param);
}

PACE_INLINE
int
pace_sched_rr_get_interval (pace_pid_t pid,
                            pace_timespec * interval)
{
  return sched_rr_get_interval (pid, interval);
}

PACE_INLINE
int
pace_sched_setparam (pace_pid_t pid,
                     const pace_sched_param * param)
{
  return sched_setparam (pid, param);
}

PACE_INLINE
int
pace_sched_getscheduler (pace_pid_t pid)
{
  return sched_getscheduler (pid);
}

PACE_INLINE
int
pace_sched_setscheduler (pace_pid_t pid,
                         int policy,
                         const pace_sched_param * param)
{
  return sched_setscheduler (pid, policy, param);
}

PACE_INLINE
int
pace_sched_yield ()
{
  return sched_yield ();
}
