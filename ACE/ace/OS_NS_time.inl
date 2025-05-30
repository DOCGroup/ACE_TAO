// -*- C++ -*-
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_errno.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"
#include <ctime>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE char *
ACE_OS::asctime (const struct tm *t)
{
  ACE_OS_TRACE ("ACE_OS::asctime");
#if defined (ACE_LACKS_ASCTIME)
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (0);
#else
  return std::asctime (t);
#endif /* ACE_LACKS_ASCTIME */
}

ACE_INLINE char *
ACE_OS::asctime_r (const struct tm *t, char *buf, int buflen)
{
  ACE_OS_TRACE ("ACE_OS::asctime_r");
#if defined (ACE_HAS_REENTRANT_FUNCTIONS)
# if defined (ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R)
  char *result = 0;
  ace_asctime_r_helper (t, buf);
  ACE_OS::strsncpy (buf, result, static_cast<size_t> (buflen));
  return buf;
# else
#   if defined (ACE_HAS_SIZET_PTR_ASCTIME_R_AND_CTIME_R)
  return ::asctime_r (t, buf, reinterpret_cast<size_t*>(&buflen));
#   else
  return ::asctime_r (t, buf, buflen);
#   endif /* ACE_HAS_SIZET_PTR_ASCTIME_R_AND_CTIME_R */
# endif /* ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R */
#elif defined (ACE_HAS_TR24731_2005_CRT)
  char *result = buf;
  ACE_SECURECRTCALL (asctime_s (buf, static_cast<size_t> (buflen), t), \
                     char*, 0, result);
  return result;
#elif defined (ACE_LACKS_ASCTIME)
  ACE_UNUSED_ARG (t);
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (buflen);
  ACE_NOTSUP_RETURN (0);
#else
  char *result = 0;
  ACE_OSCALL (std::asctime (t), char *, result);
  ACE_OS::strsncpy (buf, result, buflen);
  return buf;
#endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}

ACE_INLINE int
ACE_OS::clock_gettime (clockid_t clockid, struct timespec *ts)
{
  ACE_OS_TRACE ("ACE_OS::clock_gettime");
#if defined (ACE_HAS_CLOCK_GETTIME)
  return ::clock_gettime (clockid, ts);
#else
  ACE_UNUSED_ARG (clockid);
  ACE_UNUSED_ARG (ts);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_CLOCK_GETTIME */
}

ACE_INLINE int
ACE_OS::clock_settime (clockid_t clockid, const struct timespec *ts)
{
#if defined (ACE_HAS_CLOCK_SETTIME)
#  if defined (ACE_HAS_NONCONST_CLOCK_SETTIME)
  return ::clock_settime (clockid, const_cast<struct timespec *>(ts));
#  else
  return ::clock_settime (clockid, ts);
#  endif /* ACE_HAS_NONCONST_CLOCK_SETTIME */
#else
  ACE_UNUSED_ARG (clockid);
  ACE_UNUSED_ARG (ts);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_CLOCK_SETTIME */
}

// Magic number declaration and definition for ctime and ctime_r ()
static constexpr int ctime_buf_size = 26;

ACE_INLINE ACE_TCHAR *
ACE_OS::ctime (const time_t *t)
{
  ACE_OS_TRACE ("ACE_OS::ctime");
#if defined (ACE_LACKS_CTIME)
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_WIN32) && defined (ACE_USES_WCHAR)
  return ::_wctime (t);
#else
#  if defined (ACE_USES_WCHAR)   /* Not Win32, else it would do the above */
  char *narrow_time;
  ACE_OSCALL (::ctime (t), char *, narrow_time);
  if (narrow_time == 0)
    return 0;
  // ACE_Ascii_To_Wide::convert allocates (via new []) a wchar_t[]. If
  // we've done this before, free the previous one. Yes, this leaves a
  // small memory leak (26 characters) but there's no way around this
  // that I know of. (Steve Huston, 12-Feb-2003).
  static wchar_t *wide_time = nullptr;
  if (wide_time != nullptr)
    delete [] wide_time;
  wide_time = ACE_Ascii_To_Wide::convert (narrow_time);
  return wide_time;
#  else
  return ::ctime (t);
#  endif /* ACE_USES_WCHAR */
# endif /* ACE_LACKS_CTIME */
}

ACE_INLINE ACE_TCHAR *
ACE_OS::ctime_r (const time_t *t, ACE_TCHAR *buf, int buflen)
{
  ACE_OS_TRACE ("ACE_OS::ctime_r");

#if defined (ACE_HAS_REENTRANT_FUNCTIONS)

  char *bufp = nullptr;
#   if defined (ACE_USES_WCHAR)
  char narrow_buf[ctime_buf_size];
  bufp = narrow_buf;
#   else
  bufp = buf;
#   endif /* ACE_USES_WCHAR */

  if (buflen < ctime_buf_size)
    {
      errno = ERANGE;
      return 0;
    }
#   if defined (ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R)
  ACE_OSCALL (::ctime_r (t, bufp), char *, bufp);
#   else /* ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R */

#      if defined (ACE_HAS_SIZET_PTR_ASCTIME_R_AND_CTIME_R)
  bufp = ::ctime_r (t, bufp, reinterpret_cast<size_t*>(&buflen));
#      else /* ACE_CTIME_R_RETURNS_INT */
  bufp = ::ctime_r (t, bufp, buflen);
#      endif /* ACE_CTIME_R_RETURNS_INT */

#   endif /* ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R */

  if (bufp == nullptr)
    return nullptr;

#   if defined (ACE_USES_WCHAR)
  ACE_Ascii_To_Wide wide_buf (bufp);
  ACE_OS::strcpy (buf, wide_buf.wchar_rep ());
  return buf;
#   else
  return bufp;
#   endif /* ACE_USES_WCHAR */

#elif defined (ACE_HAS_TR24731_2005_CRT)
  if (buflen < ctime_buf_size)
    {
      errno = ERANGE;
      return nullptr;
    }
  ACE_TCHAR *result = buf;
#  if defined (ACE_USES_WCHAR)
  ACE_SECURECRTCALL (_wctime_s (buf, buflen, t), wchar_t *, 0, result);
#  else
  ACE_SECURECRTCALL (ctime_s (buf, buflen, t), char *, 0, result);
#  endif
  return result;

#else /* ACE_HAS_REENTRANT_FUNCTIONS */
  if (buflen < ctime_buf_size)
    {
      errno = ERANGE;
      return nullptr;
    }

  ACE_TCHAR *result = nullptr;
#     if defined (ACE_USES_WCHAR)
  ACE_OSCALL (::_wctime (t), wchar_t *, result);
#     else /* ACE_USES_WCHAR */
  ACE_OSCALL (::ctime (t), char *, result);
#     endif /* ACE_USES_WCHAR */
  if (result != nullptr)
    ACE_OS::strsncpy (buf, result, buflen);
  return buf;
#endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}

#if defined (ACE_USES_ULONG_FOR_STAT_TIME)
ACE_INLINE ACE_TCHAR *
ACE_OS::ctime (const unsigned long *t)
{
  return ACE_OS::ctime (reinterpret_cast<const time_t *>(t));
}

ACE_INLINE ACE_TCHAR *
ACE_OS::ctime_r (const unsigned long *clock, ACE_TCHAR *buf, int buflen)
{
  return ACE_OS::ctime_r (reinterpret_cast<const time_t *>(clock), buf, buflen);
}

ACE_INLINE struct tm *
ACE_OS::gmtime (const unsigned long *clock)
{
  return ACE_OS::gmtime (reinterpret_cast<const time_t *>(clock));
}

ACE_INLINE struct tm *
ACE_OS::gmtime_r (const unsigned long *clock,
                  struct tm *res)
{
  return ACE_OS::gmtime_r (reinterpret_cast<const time_t *>(clock), res);
}

ACE_INLINE struct tm *
ACE_OS::localtime (const unsigned long *clock)
{
  return ACE_OS::localtime (reinterpret_cast<const time_t *>(clock));
}

ACE_INLINE struct tm *
ACE_OS::localtime_r (const unsigned long *clock,
                     struct tm *res)
{
  return ACE_OS::localtime_r (reinterpret_cast<const time_t *>(clock), res);
}


#endif

ACE_INLINE double
ACE_OS::difftime (time_t t1, time_t t0)
{
  return ::ace_difftime (t1, t0);
}

ACE_INLINE ACE_hrtime_t
ACE_OS::gethrtime (const ACE_HRTimer_Op op)
{
  ACE_OS_TRACE ("ACE_OS::gethrtime");
#if defined (ACE_HAS_HI_RES_TIMER)
  ACE_UNUSED_ARG (op);
  return ::gethrtime ();
#elif defined (ACE_WIN32)
  ACE_UNUSED_ARG(op);
  LARGE_INTEGER freq;

  ::QueryPerformanceCounter (&freq);

  return freq.QuadPart;
#elif defined (__ghs__) && defined (ACE_HAS_PENTIUM)
  ACE_UNUSED_ARG (op);
  // Use .obj/gethrtime.o, which was compiled with g++.
  return ACE_GETHRTIME_NAME ();
#elif (defined (__GNUG__) || defined (__INTEL_COMPILER)) && \
  !defined (ACE_VXWORKS) && defined (ACE_HAS_PENTIUM) && \
  !defined (ACE_LACKS_PENTIUM_RDTSC)
  ACE_UNUSED_ARG (op);
  ACE_hrtime_t now;

# if defined (__amd64__) || defined (__x86_64__)
  // Read the high res tick counter into 32 bit int variables "eax" and
  // "edx", and then combine them into 64 bit int "now"
  ACE_UINT32 eax, edx;
  asm volatile ("rdtsc" : "=a" (eax), "=d" (edx) : : "memory");
  now = (((ACE_UINT64) eax) | (((ACE_UINT64) edx) << 32));
# else
  // Read the high-res tick counter directly into memory variable "now".
  // The A constraint signifies a 64-bit int.
  asm volatile ("rdtsc" : "=A" (now) : : "memory");
# endif

  return now;
#elif defined (ACE_HAS_POWERPC_TIMER) && (defined (__ghs__) || defined (__GNUG__))
  // PowerPC w/ GreenHills or g++.

  ACE_UNUSED_ARG (op);
  u_long most;
  u_long least;

#  if defined (__ghs__)
  ACE_OS::readPPCTimeBase (most, least);
#  else
  u_long scratch;

  do {
    asm volatile ("mftbu %0\n"
          "mftb  %1\n"
          "mftbu %2"
          : "=r" (most), "=r" (least), "=r" (scratch));
  } while (most != scratch);
#  endif

  return 0x100000000llu * most  +  least;

#elif defined (ACE_HAS_CLOCK_GETTIME)
  // e.g., VxWorks (besides POWERPC && GreenHills) . . .
  ACE_UNUSED_ARG (op);
  struct timespec ts;

  ACE_OS::clock_gettime (
#  if defined (ACE_HAS_CLOCK_GETTIME_MONOTONIC)
         CLOCK_MONOTONIC,
#  else
         CLOCK_REALTIME,
#  endif /* !ACE_HAS_CLOCK_GETTIME_MONOTONIC */
         &ts);

  // Carefully create the return value to avoid arithmetic overflow
  return static_cast<ACE_hrtime_t> (ts.tv_sec) *
    ACE_U_ONE_SECOND_IN_NSECS  +  static_cast<ACE_hrtime_t> (ts.tv_nsec);
#else
  ACE_UNUSED_ARG (op);
  ACE_Time_Value const now = ACE_OS::gettimeofday ();

  // Carefully create the return value to avoid arithmetic overflow
  return (static_cast<ACE_hrtime_t> (now.sec ()) * (ACE_UINT32) 1000000  +
          static_cast<ACE_hrtime_t> (now.usec ())) * (ACE_UINT32) 1000;
#endif /* ACE_HAS_HI_RES_TIMER */
}

ACE_INLINE struct tm *
ACE_OS::gmtime (const time_t *t)
{
  ACE_OS_TRACE ("ACE_OS::gmtime");
#if defined (ACE_LACKS_GMTIME)
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (0);
#else
  return ::gmtime (t);
#endif /* ACE_LACKS_GMTIME */
}

ACE_INLINE struct tm *
ACE_OS::gmtime_r (const time_t *t, struct tm *res)
{
  ACE_OS_TRACE ("ACE_OS::gmtime_r");
#if defined (ACE_HAS_REENTRANT_FUNCTIONS)
  return ace_gmtime_r_helper (t, res);
#elif defined (ACE_HAS_TR24731_2005_CRT)
  struct tm *tm_p = res;
  ACE_SECURECRTCALL (gmtime_s (res, t), struct tm *, 0, tm_p);
  return tm_p;
#elif defined (ACE_LACKS_GMTIME)
  ACE_UNUSED_ARG (t);
  ACE_UNUSED_ARG (res);
  ACE_NOTSUP_RETURN (0);
#else
  struct tm *result;
  ACE_OSCALL (::gmtime (t), struct tm *, result) ;
  if (result != 0)
    *res = *result;
  return res;
#endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}

ACE_INLINE struct tm *
ACE_OS::localtime (const time_t *t)
{
  ACE_OS_TRACE ("ACE_OS::localtime");
#if defined (ACE_LACKS_LOCALTIME)
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (0);
#else
  return ::localtime (t);
#endif /* ACE_LACKS_LOCALTIME */
}

ACE_INLINE int
ACE_OS::nanosleep (const struct timespec *requested,
                   struct timespec *remaining)
{
  ACE_OS_TRACE ("ACE_OS::nanosleep");
#if defined (ACE_HAS_CLOCK_GETTIME)
  // ::nanosleep () is POSIX 1003.1b.  So is ::clock_gettime ().  So,
  // if ACE_HAS_CLOCK_GETTIME is defined, then ::nanosleep () should
  // be available on the platform
  return ::nanosleep ((ACE_TIMESPEC_PTR) requested, remaining);
#else
  ACE_UNUSED_ARG (remaining);

  // Convert into seconds and microseconds.
  ACE_Time_Value tv (requested->tv_sec,
                     requested->tv_nsec / 1000);
  return ACE_OS::sleep (tv);
#endif /* ACE_HAS_CLOCK_GETTIME */
}

ACE_INLINE size_t
ACE_OS::strftime (char *s, size_t maxsize, const char *format,
                  const struct tm *timeptr)
{
#if defined (ACE_LACKS_STRFTIME)
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (maxsize);
  ACE_UNUSED_ARG (format);
  ACE_UNUSED_ARG (timeptr);
  ACE_NOTSUP_RETURN (0);
#else
#  ifdef __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wformat-nonliteral"
#  endif
  return std::strftime (s, maxsize, format, timeptr);
#  ifdef __GNUC__
#    pragma GCC diagnostic pop
#  endif
#endif /* ACE_LACKS_STRFTIME */
}

ACE_INLINE char *
ACE_OS::strptime (const char *buf, const char *format, struct tm *tm)
{
  ACE_OS::memset (tm, 0, sizeof (struct tm));
#if defined (ACE_LACKS_STRPTIME)
  return ACE_OS::strptime_emulation (buf, format, tm);
#else
  return ::strptime (buf, format, tm);
#endif /* ACE_LACKS_STRPTIME */
}

ACE_INLINE time_t
ACE_OS::time (time_t *tloc)
{
  ACE_OS_TRACE ("ACE_OS::time");
#if defined (ACE_LACKS_TIME)
  time_t const retv = ACE_OS::gettimeofday ().sec ();
  if (tloc)
    *tloc = retv;
  return retv;
#else
  return ::time (tloc);
#endif /* ACE_LACKS_TIME */
}

// Linux won't compile unless we explicitly use a namespace here.
#if defined (__GNUG__)
namespace ACE_OS {
  ACE_INLINE long
  timezone ()
  {
    return ::ace_timezone ();
  }
} /* namespace ACE_OS */
#else
ACE_INLINE long
ACE_OS::timezone ()
{
  return ::ace_timezone ();
}
#endif /* ACE_LINUX */

ACE_INLINE void
ACE_OS::tzset ()
{
#if defined (ACE_LACKS_TZSET)
  errno = ENOTSUP;
#elif defined (ACE_WIN32)
  ::_tzset ();  // For Win32.
#else
  ::tzset ();   // For UNIX platforms.
#endif /* ACE_LACKS_TZSET */
}

ACE_END_VERSIONED_NAMESPACE_DECL
