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
  char * retval = (char*)0;
  PACE_UNUSED_ARG (time);
  PACE_UNUSED_ARG (buf);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
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
  PACE_UNUSED_ARG (clock_id);
  PACE_UNUSED_ARG (res);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_gettime (clockid_t clock_id,
                    struct timespec * tp)
{
  PACE_UNUSED_ARG (clock_id);
  PACE_UNUSED_ARG (tp);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_clock_settime (clockid_t clock_id,
                    const struct timespec * tp)
{
  PACE_UNUSED_ARG (clock_id);
  PACE_UNUSED_ARG (tp);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
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
  char * retval = (char*)0;
  PACE_UNUSED_ARG (clock);
  PACE_UNUSED_ARG (buf);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
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
  struct tm * retval = (struct tm*)0;
  PACE_UNUSED_ARG (clock);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
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
  struct tm * retval = (struct tm*)0;
  PACE_UNUSED_ARG (clock);
  PACE_UNUSED_ARG (result);
  PACE_ERRNO_NO_SUPPORT_RETURN (retval);
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
  PACE_UNUSED_ARG (rqtp);
  PACE_UNUSED_ARG (rmtp);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
size_t
pace_strftime (char * s, size_t maxsize,
               const char * format,
               const struct tm * timeptr)
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
                   struct sigevent * evp,
                   pace_timer_t * timerid)
{
  PACE_UNUSED_ARG (clock_id);
  PACE_UNUSED_ARG (evp);
  PACE_UNUSED_ARG (timerid);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_delete (pace_timer_t timerid)
{
  PACE_UNUSED_ARG (timerid);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_getoverrun (pace_timer_t timerid)
{
  PACE_UNUSED_ARG (timerid);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_timer_gettime (pace_timer_t timerid,
                    pace_itimerspec * value)
{
  PACE_UNUSED_ARG (timerid);
  PACE_UNUSED_ARG (value);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
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
  PACE_UNUSED_ARG (timerid);
  PACE_UNUSED_ARG (flags);
  PACE_UNUSED_ARG (value);
  PACE_UNUSED_ARG (ovalue);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
void
pace_tzset ()
{
  _tzset ();
  return;
}
#endif /* PACE_HAS_POSIX_CLS_UOF */
