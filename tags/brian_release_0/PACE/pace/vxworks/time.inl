/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/time.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#include "pace/errno.h"
#include "pace/signal.h"

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_asctime (const pace_tm * time)
{
  PACE_TRACE("pace_asctime");

  return asctime (time);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
char *
pace_asctime_r (const pace_tm * time, char * buf)
{
  /*VxWorks 5.3 seems to have POSIX.4 Draft 9 versions of this as well */
  size_t buflen = 26;

  PACE_TRACE("pace_asctime_r");

  if (asctime_r (time, buf, & buflen) != 0)
    return (char*) 0;
  return buf;
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_clock_t
pace_clock (void)
{
  PACE_TRACE("pace_clock");

  return clock ();
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
double
pace_difftime (pace_time_t time1, pace_time_t time2)
{
  PACE_TRACE("pace_difftime");

  return difftime (time1, time2);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_getres (pace_clockid_t clock_id,
                   pace_timespec * res)
{
  PACE_TRACE("pace_clock_getres");

  return clock_getres (clock_id, res);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_gettime (pace_clockid_t clock_id,
                    pace_timespec * tp)
{
  PACE_TRACE("pace_clock_gettime");

  return clock_gettime (clock_id, tp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_settime (pace_clockid_t clock_id,
                    const pace_timespec * tp)
{
  PACE_TRACE("pace_clock_settime");

  return clock_settime (clock_id, PACE_NONCONST_ARG_CAST (struct timespec *) tp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_ctime (const pace_time_t * clock)
{
  PACE_TRACE("pace_ctime");

  return ctime (clock);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
char *
pace_ctime_r (const pace_time_t * clock, char * buf)
{
  size_t buflen = 26;

  PACE_TRACE("pace_ctime_r");

  return ctime_r (clock, buf, & buflen);
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
pace_tm *
pace_gmtime (const pace_time_t * clock)
{
  PACE_TRACE("pace_gmtime");

  return gmtime (clock);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
pace_tm *
pace_gmtime_r (const pace_time_t * clock, pace_tm * result)
{
  PACE_TRACE("pace_gmtime_r");

  /* VxWorks 5.3 apparently has POSIX.4 Draft 9 versions of these */
  if (gmtime_r (clock, result) != 0)
    return (pace_tm*)0;
  return result;
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
pace_tm *
pace_localtime (const pace_time_t * clock)
{
  PACE_TRACE("pace_localtime");

  return localtime (clock);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
pace_tm *
pace_localtime_r (const pace_time_t * clock, pace_tm * result)
{
  PACE_TRACE("pace_localtime_r");

  /* VxWorks 5.3 apparently has POSIX.4 Draft 9 versions of these */
  if (localtime_r (clock, result) != 0)
    return (pace_tm*)0;
  return result;
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
time_t
pace_mktime (pace_tm * timeptr)
{
  PACE_TRACE("pace_mktime");

  return mktime (timeptr);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_nanosleep (const pace_timespec * rqtp,
                pace_timespec * rmtp)
{
  PACE_TRACE("pace_nanosleep");

  return nanosleep (PACE_NONCONST_ARG_CAST (struct timespec *) rqtp, rmtp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
size_t
pace_strftime (char *s, pace_size_t maxsize,
               const char *format,
               const pace_tm *timeptr)
{
  PACE_TRACE("pace_strftime");

  return strftime (s, maxsize, format, timeptr);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
time_t
pace_time (pace_time_t * tloc)
{
  PACE_TRACE("pace_time");

  return time (tloc);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_create (pace_clockid_t clock_id,
                   pace_sigevent * evp,
                   pace_timer_t *timerid)
{
  PACE_TRACE("pace_timer_create");

  return timer_create (clock_id, evp, timerid);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_delete (pace_timer_t timerid)
{
  PACE_TRACE("pace_timer_delete");

  return timer_delete (timerid);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_getoverrun (pace_timer_t timerid)
{
  PACE_TRACE("pace_timer_getoverrun");

  return timer_getoverrun (timerid);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_gettime (pace_timer_t timerid,
                    pace_itimerspec * value)
{
  PACE_TRACE("pace_timer_gettime");

  return timer_gettime (timerid, value);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_settime (pace_timer_t timerid,
                    int flags,
                    const pace_itimerspec * value,
                    pace_itimerspec * ovalue)
{
  PACE_TRACE("pace_timer_settime");

  return timer_settime (timerid, flags, PACE_NONCONST_ARG_CAST (struct itimerspec *) value, ovalue);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void
pace_tzset ()
{
  PACE_TRACE("pace_tzset");

  PACE_ERRNO_NO_SUPPORT ();
  return;
}
#endif /* PACE_HAS_POSIX_CLS_UOF */
