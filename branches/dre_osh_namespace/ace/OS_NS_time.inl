// -*- C++ -*-
// $Id$

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_errno.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_unistd.h"

ACE_INLINE char *
ACE_OS::asctime (const struct tm *t)
{
#if !defined (ACE_HAS_WINCE) && !defined(ACE_PSOS) || defined (ACE_PSOS_HAS_TIME)
  ACE_OS_TRACE ("ACE_OS::asctime");
  ACE_OSCALL_RETURN (::asctime (t), char *, 0);
#else
  // @@ WinCE doesn't have gmtime also.
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (0);
#endif /*  !ACE_HAS_WINCE && !ACE_PSOS || ACE_PSOS_HAS_TIME */
}

ACE_INLINE char *
ACE_OS::asctime_r (const struct tm *t, char *buf, int buflen)
{
  ACE_OS_TRACE ("ACE_OS::asctime_r");
#if defined (ACE_HAS_REENTRANT_FUNCTIONS)
# if defined (ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R)
  char *result;
#   if defined (DIGITAL_UNIX)
  ACE_OSCALL (::_Pasctime_r (t, buf), char *, 0, result);
#   else
  ACE_OSCALL (::asctime_r (t, buf), char *, 0, result);
#   endif /* DIGITAL_UNIX */
  ACE_OS::strsncpy (buf, result, buflen);
  return buf;
# else
#   if defined (HPUX_10)
  return (::asctime_r(t, buf, buflen) == 0 ? buf : (char *)0);
#   else
  ACE_OSCALL_RETURN (::asctime_r (t, buf, buflen), char *, 0);
#   endif /* HPUX_10 */
# endif /* ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R */
#elif ! defined (ACE_HAS_WINCE) && !defined(ACE_PSOS) || defined (ACE_PSOS_HAS_TIME)
  char *result;
  ACE_OSCALL (::asctime (t), char *, 0, result);
  ACE_OS::strsncpy (buf, result, buflen);
  return buf;
#else
  // @@ Same as ACE_OS::asctime (), you need to implement it
  //    yourself.
  ACE_UNUSED_ARG (t);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (buflen);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}

ACE_INLINE int
ACE_OS::clock_gettime (clockid_t clockid, struct timespec *ts)
{
  ACE_OS_TRACE ("ACE_OS::clock_gettime");
#if defined (ACE_HAS_CLOCK_GETTIME)
  ACE_OSCALL_RETURN (::clock_gettime (clockid, ts), int, -1);
# elif defined (ACE_PSOS) && ! defined (ACE_PSOS_DIAB_MIPS)
  ACE_UNUSED_ARG (clockid);
  ACE_PSOS_Time_t pt;
  int result = ACE_PSOS_Time_t::get_system_time (pt);
  *ts = ACE_static_cast (struct timespec, pt);
  return result;
#else
  ACE_UNUSED_ARG (clockid);
  ACE_UNUSED_ARG (ts);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_CLOCK_GETTIME */
}

// Magic number declaration and definition for ctime and ctime_r ()
static const int ctime_buf_size = 26;

ACE_INLINE ACE_TCHAR *
ACE_OS::ctime (const time_t *t)
{
  ACE_OS_TRACE ("ACE_OS::ctime");
#if defined (ACE_HAS_BROKEN_CTIME)
  ACE_OSCALL_RETURN (::asctime (::localtime (t)), char *, 0);
#elif defined(ACE_PSOS) && ! defined (ACE_PSOS_HAS_TIME)
  return "ctime-return";
#elif defined (ACE_HAS_WINCE)
  static ACE_TCHAR buf [ctime_buf_size];
  return ACE_OS::ctime_r (t,
                          buf,
                          ctime_buf_size);
#elif defined (ACE_USES_WCHAR)
  ACE_OSCALL_RETURN (::_wctime (t), wchar_t *, 0);
#else
  ACE_OSCALL_RETURN (::ctime (t), char *, 0);
# endif /* ACE_HAS_BROKEN_CTIME */
}

#if !defined (ACE_HAS_WINCE)  /* CE version in OS.cpp */
ACE_INLINE ACE_TCHAR *
ACE_OS::ctime_r (const time_t *t, ACE_TCHAR *buf, int buflen)
{
  ACE_OS_TRACE ("ACE_OS::ctime_r");

#if defined (ACE_HAS_REENTRANT_FUNCTIONS)
#   if defined (ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R)
  if (buflen < ctime_buf_size)
    {
      errno = ERANGE;
      return 0;
    }
#      if defined (DIGITAL_UNIX)
  ACE_OSCALL_RETURN (::_Pctime_r (t, buf), ACE_TCHAR *, 0);
#      else /* DIGITAL_UNIX */
  ACE_OSCALL_RETURN (::ctime_r (t, buf), ACE_TCHAR *, 0);
#      endif /* DIGITAL_UNIX */
  return buf;
#   else /* ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R */

#      if defined (ACE_CTIME_R_RETURNS_INT)
  return (::ctime_r (t, buf, buflen) == -1 ? 0 : buf);
#      else /* ACE_CTIME_R_RETURNS_INT */
  ACE_OSCALL_RETURN (::ctime_r (t, buf, buflen), ACE_TCHAR *, 0);
#      endif /* ACE_CTIME_R_RETURNS_INT */

#   endif /* ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R */
#else /* ACE_HAS_REENTRANT_FUNCTIONS */
#   if defined(ACE_PSOS) && ! defined (ACE_PSOS_HAS_TIME)
  ACE_OS::strsncpy (buf, "ctime-return", buflen);
  return buf;
#   else /* ACE_PSOS && !ACE_PSOS_HAS_TIME */
  if (buflen < ctime_buf_size)
    {
      errno = ERANGE;
      return 0;
    }

  ACE_TCHAR *result;
#     if defined (ACE_USES_WCHAR)
  ACE_OSCALL (::_wctime (t), wchar_t *, 0, result);
#     else /* ACE_USES_WCHAR */
  ACE_OSCALL (::ctime (t), char *, 0, result);
#     endif /* ACE_USES_WCHAR */
  if (result != 0)
    ACE_OS::strsncpy (buf, result, buflen);
  return buf;
#   endif /* ACE_PSOS && !ACE_PSOS_HAS_TIME */
#endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}
#endif /* !ACE_HAS_WINCE */

#if !defined (ACE_LACKS_DIFFTIME)
ACE_INLINE double
ACE_OS::difftime (time_t t1, time_t t0)
{
  return ::ace_difftime (t1, t0);
}
#endif /* ! ACE_LACKS_DIFFTIME */

#if defined (ghs) && defined (ACE_HAS_PENTIUM) && !defined (ACE_WIN32)
  extern "C" ACE_hrtime_t ACE_gethrtime ();
#endif /* ghs && ACE_HAS_PENTIUM */

ACE_INLINE ACE_hrtime_t
ACE_OS::gethrtime (const ACE_HRTimer_Op op)
{
  ACE_OS_TRACE ("ACE_OS::gethrtime");
#if defined (ACE_HAS_HI_RES_TIMER)
  ACE_UNUSED_ARG (op);
  return ::gethrtime ();
#elif defined (ACE_HAS_AIX_HI_RES_TIMER)
  ACE_UNUSED_ARG (op);
  timebasestruct_t tb;

  ::read_real_time(&tb, TIMEBASE_SZ);
  ::time_base_to_time(&tb, TIMEBASE_SZ);

  return ACE_hrtime_t(tb.tb_high) * ACE_ONE_SECOND_IN_NSECS + tb.tb_low;
#elif defined (ghs) && defined (ACE_HAS_PENTIUM) && !defined (ACE_WIN32)
  ACE_UNUSED_ARG (op);
  // Use .obj/gethrtime.o, which was compiled with g++.
  return ACE_gethrtime ();
#elif (defined(__KCC) || defined (__GNUG__)) && !defined (__MINGW32__) && defined (ACE_HAS_PENTIUM)
  ACE_UNUSED_ARG (op);

# if defined (ACE_LACKS_LONGLONG_T)
  double now;
# else  /* ! ACE_LACKS_LONGLONG_T */
  ACE_hrtime_t now;
# endif /* ! ACE_LACKS_LONGLONG_T */

  // See comments about the RDTSC Pentium instruction for the ACE_WIN32
  // version of ACE_OS::gethrtime (), below.
  //
  // Read the high-res tick counter directly into memory variable "now".
  // The A constraint signifies a 64-bit int.
  asm volatile ("rdtsc" : "=A" (now) : : "memory");

# if defined (ACE_LACKS_LONGLONG_T)
  ACE_UINT32 least, most;
  ACE_OS::memcpy (&least, &now, sizeof (ACE_UINT32));
  ACE_OS::memcpy (&most, (u_char *) &now + sizeof (ACE_UINT32),
                  sizeof (ACE_UINT32));

  ACE_hrtime_t ret (least, most);
  return ret;
# else  /* ! ACE_LACKS_LONGLONG_T */
  return now;
# endif /* ! ACE_LACKS_LONGLONG_T */
#elif defined (linux) && defined (ACE_HAS_ALPHA_TIMER)
  // NOTE:  alphas only have a 32 bit tick (cycle) counter.  The rpcc
  // instruction actually reads 64 bits, but the high 32 bits are
  // implementation-specific.  Linux and Digital Unix, for example,
  // use them for virtual tick counts, i.e., taking into account only
  // the time that the process was running.  This information is from
  // David Mosberger's article, see comment below.
  ACE_UINT32 now;

  // The following statement is based on code published by:
  // Mosberger, David, "How to Make Your Applications Fly, Part 1",
  // Linux Journal Issue 42, October 1997, page 50.  It reads the
  // high-res tick counter directly into the memory variable.
  asm volatile ("rpcc %0" : "=r" (now) : : "memory");

  return now;
#elif defined (ACE_WIN32)
  ACE_UNUSED_ARG(op);
  LARGE_INTEGER freq;

  ::QueryPerformanceCounter (&freq);

#  if defined (ACE_LACKS_LONGLONG_T)
  ACE_UINT64 uint64_freq(freq.u.LowPart, ACE_static_cast (unsigned int, freq.u.HighPart));
  return uint64_freq;
#  else
  return freq.QuadPart;
#  endif //ACE_LACKS_LONGLONG_T

#elif defined (CHORUS)
  if (op == ACE_OS::ACE_HRTIMER_GETTIME)
    {
      struct timespec ts;

      ACE_OS::clock_gettime (CLOCK_REALTIME, &ts);

      // Carefully create the return value to avoid arithmetic overflow
      // if ACE_hrtime_t is ACE_U_LongLong.
      ACE_hrtime_t now = ts.tv_sec;
      now *= ACE_U_ONE_SECOND_IN_NSECS;
      now += ts.tv_nsec;

      return now;
    }
  else
    {
      // Use the sysBench timer on Chorus.  On MVME177, at least, it only
      // has 32 bits.  Be careful, because using it disables interrupts!
      ACE_UINT32 now;
      if (::sysBench (op, (int *) &now) == K_OK)
        {
          now *= 1000u /* nanoseconds/microsecond */;
          return (ACE_hrtime_t) now;
        }
      else
        {
          // Something went wrong.  Just return 0.
          return (ACE_hrtime_t) 0;
        }
    }

#elif defined (ACE_HAS_POWERPC_TIMER) && (defined (ghs) || defined (__GNUG__))
  // PowerPC w/ GreenHills or g++.

  ACE_UNUSED_ARG (op);
  u_long most;
  u_long least;
  ACE_OS::readPPCTimeBase (most, least);
#if defined (ACE_LACKS_LONGLONG_T)
  return ACE_U_LongLong (least, most);
#else  /* ! ACE_LACKS_LONGLONG_T */
  return 0x100000000llu * most  +  least;
#endif /* ! ACE_LACKS_LONGLONG_T */

#elif defined (ACE_HAS_CLOCK_GETTIME) || defined (ACE_PSOS)
  // e.g., VxWorks (besides POWERPC && GreenHills) . . .
  ACE_UNUSED_ARG (op);
  struct timespec ts;

  ACE_OS::clock_gettime (CLOCK_REALTIME, &ts);

  // Carefully create the return value to avoid arithmetic overflow
  // if ACE_hrtime_t is ACE_U_LongLong.
  return ACE_static_cast (ACE_hrtime_t, ts.tv_sec) *
    ACE_U_ONE_SECOND_IN_NSECS  +  ACE_static_cast (ACE_hrtime_t, ts.tv_nsec);
#else
  ACE_UNUSED_ARG (op);
  const ACE_Time_Value now = ACE_OS::gettimeofday ();

  // Carefully create the return value to avoid arithmetic overflow
  // if ACE_hrtime_t is ACE_U_LongLong.
  return (ACE_static_cast (ACE_hrtime_t, now.sec ()) * (ACE_UINT32) 1000000  +
          ACE_static_cast (ACE_hrtime_t, now.usec ())) * (ACE_UINT32) 1000;
#endif /* ACE_HAS_HI_RES_TIMER */
}

ACE_INLINE struct tm *
ACE_OS::gmtime (const time_t *t)
{
#if !defined (ACE_HAS_WINCE) && !defined (ACE_PSOS) || defined (ACE_PSOS_HAS_TIME)
  ACE_OS_TRACE ("ACE_OS::gmtime");
  ACE_OSCALL_RETURN (::gmtime (t), struct tm *, 0);
#else
  // @@ WinCE doesn't have gmtime also.
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_WINCE && !ACE_PSOS || ACE_PSOS_HAS_TIME */
}

ACE_INLINE struct tm *
ACE_OS::gmtime_r (const time_t *t, struct tm *res)
{
  ACE_OS_TRACE ("ACE_OS::gmtime_r");
#if defined (ACE_HAS_REENTRANT_FUNCTIONS)
# if defined (DIGITAL_UNIX)
  ACE_OSCALL_RETURN (::_Pgmtime_r (t, res), struct tm *, 0);
# elif defined (HPUX_10)
  return (::gmtime_r (t, res) == 0 ? res : (struct tm *) 0);
# else
  ACE_OSCALL_RETURN (::gmtime_r (t, res), struct tm *, 0);
# endif /* DIGITAL_UNIX */
#elif !defined (ACE_HAS_WINCE) && !defined(ACE_PSOS) || defined (ACE_PSOS_HAS_TIME)
  struct tm *result;
  ACE_OSCALL (::gmtime (t), struct tm *, 0, result) ;
  if (result != 0)
    *res = *result;
  return res;
#else
  // @@ Same as ACE_OS::gmtime (), you need to implement it
  //    yourself.
  ACE_UNUSED_ARG (t);
  ACE_UNUSED_ARG (res);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}

ACE_INLINE struct tm *
ACE_OS::localtime (const time_t *t)
{
#if !defined (ACE_HAS_WINCE) && !defined (ACE_PSOS) || defined (ACE_PSOS_HAS_TIME)
  ACE_OS_TRACE ("ACE_OS::localtime");
  ACE_OSCALL_RETURN (::localtime (t), struct tm *, 0);
#else
  // @@ Don't you start wondering what kind of functions
  //    does WinCE really support?
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_WINCE && !ACE_PSOS || ACE_PSOS_HAS_TIME */
}

ACE_INLINE int
ACE_OS::nanosleep (const struct timespec *requested,
                   struct timespec *remaining)
{
  ACE_OS_TRACE ("ACE_OS::nanosleep");
#if defined (ACE_HAS_CLOCK_GETTIME)
  // ::nanosleep () is POSIX 1003.1b.  So is ::clock_gettime ().  So,
  // if ACE_HAS_CLOCK_GETTIME is defined, then ::nanosleep () should
  // be available on the platform.  On Solaris 2.x, both functions
  // require linking with -lposix4.
  return ::nanosleep ((ACE_TIMESPEC_PTR) requested, remaining);
#elif defined (ACE_PSOS)
#  if ! defined (ACE_PSOS_DIAB_MIPS)
  double ticks = KC_TICKS2SEC * requested->tv_sec +
                 ( ACE_static_cast (double, requested->tv_nsec) *
                   ACE_static_cast (double, KC_TICKS2SEC) ) /
                 ACE_static_cast (double, ACE_ONE_SECOND_IN_NSECS);

  if (ticks > ACE_static_cast (double, ACE_PSOS_Time_t::max_ticks))
  {
    ticks -= ACE_static_cast (double, ACE_PSOS_Time_t::max_ticks);
    remaining->tv_sec = ACE_static_cast (time_t,
                                         (ticks /
                                          ACE_static_cast (double,
                                                           KC_TICKS2SEC)));
    ticks -= ACE_static_cast (double, remaining->tv_sec) *
             ACE_static_cast (double, KC_TICKS2SEC);

    remaining->tv_nsec =
      ACE_static_cast (long,
                       (ticks * ACE_static_cast (double,
                                                 ACE_ONE_SECOND_IN_NSECS)) /
                       ACE_static_cast (double, KC_TICKS2SEC));

    ::tm_wkafter (ACE_PSOS_Time_t::max_ticks);
  }
  else
  {
    remaining->tv_sec = 0;
    remaining->tv_nsec = 0;
    ::tm_wkafter (ACE_static_cast (u_long, ticks));
  }

  // tm_wkafter always returns 0
#  endif /* ACE_PSOS_DIAB_MIPS */
  return 0;
#else
  ACE_UNUSED_ARG (remaining);

  // Convert into seconds and microseconds.
# if ! defined(ACE_HAS_BROKEN_TIMESPEC_MEMBERS)
  ACE_Time_Value tv (requested->tv_sec,
                     requested->tv_nsec / 1000);
# else
  ACE_Time_Value tv (requested->ts_sec,
                     requested->ts_nsec / 1000);
# endif /* ACE_HAS_BROKEN_TIMESPEC_MEMBERS */
  return ACE_OS::sleep (tv);
#endif /* ACE_HAS_CLOCK_GETTIME */
}

ACE_INLINE size_t
ACE_OS::strftime (char *s, size_t maxsize, const char *format,
                  const struct tm *timeptr)
{
#if !defined (ACE_HAS_WINCE) && !defined(ACE_PSOS) || defined (ACE_PSOS_HAS_TIME)
  return ::strftime (s, maxsize, format, timeptr);
#else
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (maxsize);
  ACE_UNUSED_ARG (format);
  ACE_UNUSED_ARG (timeptr);
  ACE_NOTSUP_RETURN (0);
#endif /* !ACE_HAS_WINCE && !ACE_PSOS || ACE_PSOS_HAS_TIME */
}

ACE_INLINE time_t
ACE_OS::time (time_t *tloc)
{
#if !defined (ACE_HAS_WINCE)
  ACE_OS_TRACE ("ACE_OS::time");
#  if defined (ACE_PSOS) && ! defined (ACE_PSOS_HAS_TIME)
        unsigned long d_date, d_time, d_tick;
        tm_get(&d_date, &d_time, &d_tick); // get current time
        if (tloc)
                *tloc = d_time; // set time as time_t
        return d_time;
#  else
  ACE_OSCALL_RETURN (::time (tloc), time_t, (time_t) -1);
#  endif /* ACE_PSOS && ! ACE_PSOS_HAS_TIME */
#else
  time_t retv = ACE_OS::gettimeofday ().sec ();
  if (tloc)
    *tloc = retv;
  return retv;
#endif /* ACE_HAS_WINCE */
}

// Linux won't compile unless we explicitly use a namespace here.
#if defined (linux) || defined (sun)
namespace ACE_OS {
  ACE_INLINE long
  timezone (void)
  {
    return ::ace_timezone ();
  }
} /* namespace ACE_OS */
#else
ACE_INLINE long
ACE_OS::timezone (void)
{
  return ::ace_timezone ();
}
#endif /* linux */

ACE_INLINE void
ACE_OS::tzset (void)
{
#if !defined (ACE_HAS_WINCE) && !defined (VXWORKS) && !defined (ACE_PSOS) && ! defined(__rtems__)
#   if defined (ACE_WIN32)
  ::_tzset ();  // For Win32.
#   else
  ::tzset ();   // For UNIX platforms.
#   endif /* ACE_WIN32 */
# else
  errno = ENOTSUP;
# endif /* ACE_HAS_WINCE && !VXWORKS && !ACE_PSOS && !__rtems__ */
}
