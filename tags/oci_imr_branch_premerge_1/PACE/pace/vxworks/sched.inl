/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/sched.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sched_get_priority_max (int policy)
{
  PACE_TRACE("pace_sched_get_priority_max");

  return sched_get_priority_max (policy);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sched_get_priority_min (int policy)
{
  PACE_TRACE("pace_sched_get_priority_min");

  return sched_get_priority_min (policy);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sched_getparam (pace_pid_t pid,
                     pace_sched_param * param)
{
  PACE_TRACE("pace_sched_getparam");

  return sched_getparam (pid, param);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sched_rr_get_interval (pace_pid_t pid,
                            pace_timespec * interval)
{
  PACE_TRACE("pace_sched_rr_get_interval");

  return sched_rr_get_interval (pid, interval);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sched_setparam (pace_pid_t pid,
                     const pace_sched_param * param)
{
  PACE_TRACE("pace_sched_setparam");

  return sched_setparam (pid, param);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sched_getscheduler (pace_pid_t pid)
{
  PACE_TRACE("pace_sched_getscheduler");

  return sched_getscheduler (pid);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sched_setscheduler (pace_pid_t pid,
                         int policy,
                         const pace_sched_param * param)
{
  PACE_TRACE("pace_sched_setscheduler");

  return sched_setscheduler (pid, policy, param);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sched_yield ()
{
  PACE_TRACE("pace_sched_yield");

  return sched_yield ();
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
