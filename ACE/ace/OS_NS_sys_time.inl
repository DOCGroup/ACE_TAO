// -*- C++ -*-
#include "ace/os_include/sys/os_time.h"
#include "ace/os_include/os_errno.h"

#if defined (ACE_VXWORKS) || defined (ACE_HAS_CLOCK_GETTIME_REALTIME) || defined (ACE_LACKS_GETTIMEOFDAY)
#  include "ace/OS_NS_time.h"
#endif /* ACE_VXWORKS || ACE_HAS_CLOCK_REALTIME */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ACE_Time_Value
ACE_OS::gettimeofday ()
{
  // ACE_OS_TRACE ("ACE_OS::gettimeofday");

#if !defined (ACE_WIN32) && !defined (ACE_LACKS_GETTIMEOFDAY)
  timeval tv;
  int result = 0;
#endif // !defined (ACE_WIN32)

#if defined (ACE_LACKS_GETTIMEOFDAY) && defined (ACE_HAS_CLOCK_GETTIME)
  timespec ts;
  if (ACE_OS::clock_gettime (CLOCK_REALTIME, &ts) == -1)
    {
      return ACE_Time_Value (-1);
    }

  return ACE_Time_Value (ts);
#elif defined (ACE_WIN32)
  FILETIME   tfile;
  ::GetSystemTimeAsFileTime (&tfile);
  return ACE_Time_Value (tfile);
#else
# if defined (ACE_HAS_TIMEZONE_GETTIMEOFDAY) || \
     defined (ACE_HAS_VOIDPTR_GETTIMEOFDAY) || \
     defined (ACE_HAS_SVR4_GETTIMEOFDAY)
  ACE_OSCALL (::gettimeofday (&tv, 0), int, result);
# elif defined (ACE_VXWORKS)
  // Assumes that struct timespec is same size as struct timeval,
  // which assumes that time_t is a long: it currently (VxWorks
  // 5.2/5.3) is.
  struct timespec ts;

  ACE_OSCALL (ACE_OS::clock_gettime (CLOCK_REALTIME, &ts), int, result);
  tv.tv_sec = ts.tv_sec;
  tv.tv_usec = ts.tv_nsec / 1000L;  // timespec has nsec, but timeval has usec
# elif defined (ACE_MQX)
  TIME_STRUCT ts;
  _time_get(&ts);
  tv.tv_sec = ts.SECONDS;
  tv.tv_usec = ts.MILLISECONDS * 1000;
# else
#  if defined (ACE_LACKS_GETTIMEOFDAY)
  ACE_NOTSUP_RETURN (ACE_Time_Value ((time_t)-1));
#  else
  ACE_OSCALL (::gettimeofday (&tv), int, result);
#  endif /* ACE_LACKS_GETTIMEOFDAY */
# endif /* ACE_HAS_SVR4_GETTIMEOFDAY */
#endif /* 0 */
#if !defined (ACE_WIN32) && !defined (ACE_LACKS_GETTIMEOFDAY)
  if (result == -1)
    return ACE_Time_Value ((time_t)-1);
  else
    return ACE_Time_Value (tv);
#endif // !defined (ACE_WIN32)
}

ACE_INLINE ACE_Time_Value
ACE_OS::gettimeofday_ ()
{
  return ACE_OS::gettimeofday ();
}

ACE_END_VERSIONED_NAMESPACE_DECL
