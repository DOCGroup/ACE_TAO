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

#include <errno.h>
extern int errno;

PACE_INLINE
char *
pace_asctime_r (const struct tm * time, char * buf)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return asctime_r (time, buf);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT ();
  return 0;
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_clock_getres (clockid_t clock_id,
                   struct timespec * res)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return clock_getres (clock_id, res);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_clock_gettime (clockid_t clock_id,
                    struct timespec * tp)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return clock_gettime (clock_id, tp);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_clock_settime (clockid_t clock_id,
                    const struct timespec * tp)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return clock_settime (clock_id, tp);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
char *
pace_ctime_r (const time_t * clock, char * buf)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return ctime_r (clock, buf);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT ();
  return 0;
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
struct tm *
pace_gmtime_r (const time_t * clock, struct tm * result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return gmtime_r (clock, result);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT ();
  return 0;
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
struct tm *
pace_localtime_r (const time_t * clock, struct tm * result)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return localtime_r (clock, result);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT ();
  return 0;
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_nanosleep (const struct timespec * rqtp,
                struct timespec * rmtp)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return nanosleep (rqtp, rmtp);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_time (time_t * tloc)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return time (tloc);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_timer_create (clockid_t clock_id,
                   struct sigevent *evp,
                   timer_t *timerid)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return timer_create (clock_id, evp, timerid);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_timer_delete (timer_t timerid)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return timer_delete (timerid);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_timer_getoverrun (timer_t timerid)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return timer_getoverrun (timerid);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_timer_gettime (timer_t timerid,
                    struct itimerspec * value)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return timer_gettime (timerid, value);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_timer_settime (timer_t timerid,
                    int flags,
                    const struct itimerspec * value,
                    struct itimerspec * ovalue)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return timer_settime (timerid, flags, value, ovalue);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

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
