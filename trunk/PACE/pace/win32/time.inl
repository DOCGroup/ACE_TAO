/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/time.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <errno.h>
extern int errno;

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_asctime (const struct tm * time)
{
  return asctime (time);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
char *
pace_asctime_r (const struct tm * time, char * buf)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return asctime_r (time, buf);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (time);
  PACE_UNUSED_ARG (buf);
  PACE_ERRNO_NO_SUPPORT ();
  return 0;
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
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
pace_clock_getres (clockid_t clock_id,
                   struct timespec * res)
{
  return clock_getres (clock_id, res);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_gettime (clockid_t clock_id,
                    struct timespec * tp)
{
  return clock_gettime (clock_id, tp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_settime (clockid_t clock_id,
                    const struct timespec * tp)
{
  return clock_settime (clock_id, tp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
char *
pace_ctime (const time_t * clock)
{
  return ctime (clock);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
char *
pace_ctime_r (const time_t * clock, char * buf)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return ctime_r (clock, buf);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (clock);
  PACE_UNUSED_ARG (buf);
  PACE_ERRNO_NO_SUPPORT ();
  return 0;
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
struct tm *
pace_gmtime (const time_t * clock)
{
  return gmtime (clock);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
struct tm *
pace_gmtime_r (const time_t * clock, struct tm * result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return gmtime_r (clock, result);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (clock);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT ();
  return 0;
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
struct tm *
pace_localtime (const time_t * clock)
{
  return localtime (clock);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLSR_UOF)
PACE_INLINE
struct tm *
pace_localtime_r (const time_t * clock, struct tm * result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return localtime_r (clock, result);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (clock);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT ();
  return 0;
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
time_t
pace_mktime (struct tm * timeptr)
{
  return mktime (timeptr);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_nanosleep (const struct timespec * rqtp,
                struct timespec * rmtp)
{
  return nanosleep (rqtp, rmtp);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
size_t
pace_strftime (char *s, size_t maxsize,
               const char *format,
               const struct tm *timeptr)
{
  return strftime (s, maxsize, format, timeptr);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
time_t
pace_time (time_t * tloc)
{
  return time (tloc);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_create (clockid_t clock_id,
                   struct sigevent *evp,
                   timer_t *timerid)
{
  return timer_create (clock_id, evp, timerid);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_delete (timer_t timerid)
{
  return timer_delete (timerid);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_getoverrun (timer_t timerid)
{
  return timer_getoverrun (timerid);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_gettime (timer_t timerid,
                    struct itimerspec * value)
{
  return timer_gettime (timerid, value);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_settime (timer_t timerid,
                    int flags,
                    const struct itimerspec * value,
                    struct itimerspec * ovalue)
{
  return timer_settime (timerid, flags, value, ovalue);
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
