/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/time.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/errno.h"
#include "pace/signal.h"
extern int errno;

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_asctime (const pace_tm * time)
{
  return asctime (time);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
char *
pace_asctime_r (const pace_tm * time, char * buf)
{
#if (PACE_LYNXOS)
  /*lynxos 3.1 has POSIX.4 Draft 9 versions of this */
  if (asctime_r (time, buf, 26) != 0)
    return (char*) 0;
  return buf;
#else
  return asctime_r (time, buf);
#endif /* PACE_LYNXOS */
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_clock_t
pace_clock (void)
{
  return clock ();
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
double
pace_difftime (pace_time_t time1, pace_time_t time2)
{
  return difftime (time1, time2);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_getres (pace_clockid_t clock_id,
                   pace_timespec * res)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (clock_id);
  PACE_UNUSED_ARG (res);
  return -1;
#else
  return clock_getres (clock_id, res);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_gettime (pace_clockid_t clock_id,
                    pace_timespec * tp)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (clock_id);
  PACE_UNUSED_ARG (tp);
  return -1;
#else
  return clock_gettime (clock_id, tp);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_settime (pace_clockid_t clock_id,
                    const pace_timespec * tp)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (clock_id);
  PACE_UNUSED_ARG (tp);
  return -1;
#else
  return clock_settime (clock_id, PACE_NONCONST_ARG_CAST (struct timespec *) tp);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_ctime (const pace_time_t * clock)
{
  return ctime (clock);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
char *
pace_ctime_r (const pace_time_t * clock, char * buf)
{
# if (PACE_LYNXOS)
  return ctime_r (clock, buf, 26);
# else
  return ctime_r (clock, buf);
# endif /* PACE_SUNOS */
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
pace_tm *
pace_gmtime (const pace_time_t * clock)
{
  return gmtime (clock);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
pace_tm *
pace_gmtime_r (const pace_time_t * clock, pace_tm * result)
{
#if (PACE_LYNXOS)
  /*lynxos 3.1 has POSIX.4 Draft 9 versions of these */
  if (gmtime_r (result, clock) != 0)
    return (pace_tm*)0;
  return result;
#else
  return gmtime_r (clock, result);
#endif /* PACE_LYNXOS */
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
pace_tm *
pace_localtime (const pace_time_t * clock)
{
  return localtime (clock);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
pace_tm *
pace_localtime_r (const pace_time_t * clock, pace_tm * result)
{
#if (PACE_LYNXOS)
  /*lynxos 3.1 has POSIX.4 Draft 9 versions of these */
  if (localtime_r (result, clock) != 0)
    return (pace_tm*)0;
  return result;
#else
  return localtime_r (clock, result);
#endif /* PACE_LYNXOS */
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
time_t
pace_mktime (pace_tm * timeptr)
{
  return mktime (timeptr);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_nanosleep (const pace_timespec * rqtp,
                pace_timespec * rmtp)
{
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
  return strftime (s, maxsize, format, timeptr);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
time_t
pace_time (pace_time_t * tloc)
{
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
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (clock_id);
  PACE_UNUSED_ARG (evp);
  PACE_UNUSED_ARG (timerid);
  return -1;
#else
  return timer_create (clock_id, evp, timerid);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_delete (pace_timer_t timerid)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (timerid);
  return -1;
#else
  return timer_delete (timerid);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_getoverrun (pace_timer_t timerid)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (timerid);
  return -1;
#else
  return timer_getoverrun (timerid);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_gettime (pace_timer_t timerid,
                    pace_itimerspec * value)
{
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (timerid);
  PACE_UNUSED_ARG (value);
  return -1;
#else
  return timer_gettime (timerid, value);
#endif /* PACE_LINUX */
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
#if PACE_LINUX
  errno = ENOSYS;
  PACE_UNUSED_ARG (timerid);
  PACE_UNUSED_ARG (flags);
  PACE_UNUSED_ARG (value);
  PACE_UNUSED_ARG (ovalue);
  return -1;
#else
  return timer_settime (timerid, flags, PACE_NONCONST_ARG_CAST (struct itimerspec *) value, ovalue);
#endif /* PACE_LINUX */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void
pace_tzset ()
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  tzset ();
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT ();
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  return;
}
#endif /* PACE_HAS_POSIX_CLS_UOF */
