// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_time.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_TIME_H
# define ACE_OS_NS_TIME_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_time.h"

# if defined (ACE_HAS_BROKEN_R_ROUTINES)
#   undef ctime_r
#   undef asctime_r
# endif /* ACE_HAS_BROKEN_R_ROUTINES */

#include "ace/Basic_Types.h"

// Type-safe, and unsigned.
static const ACE_UINT32 ACE_U_ONE_SECOND_IN_MSECS = 1000U;
static const ACE_UINT32 ACE_U_ONE_SECOND_IN_USECS = 1000000U;
static const ACE_UINT32 ACE_U_ONE_SECOND_IN_NSECS = 1000000000U;

#if defined (ACE_PSOS_HAS_TIME)

// Use pSOS time, wrapped . . .
class ACE_OS_Export ACE_PSOS_Time_t
{
public:
    /// default ctor: date, time, and ticks all zeroed.
  ACE_PSOS_Time_t (void);

    /// ctor from a timespec_t
  ACE_PSOS_Time_t (const timespec_t& t);

    /// type cast operator (to a timespec_t)
  operator timespec_t ();

    /// static member function to get current system time
  static u_long get_system_time (ACE_PSOS_Time_t& t);

    /// static member function to set current system time
  static u_long set_system_time (const ACE_PSOS_Time_t& t);

#   if defined (ACE_PSOSIM)
    /// static member function to initialize system time, using UNIX calls
  static u_long init_simulator_time (void);
#   endif /* ACE_PSOSIM */

    /// max number of ticks supported in a single system call
  static const u_long max_ticks;
private:
  // = Constants for prying info out of the pSOS time encoding.
  static const u_long year_mask;
  static const u_long month_mask;
  static const u_long day_mask;
  static const u_long hour_mask;
  static const u_long minute_mask;
  static const u_long second_mask;
  static const int year_shift;
  static const int month_shift;
  static const int hour_shift;
  static const int minute_shift;
  static const int year_origin;
  static const int month_origin;

  // error codes
  static const u_long err_notime;   // system time not set
  static const u_long err_illdate;  // date out of range
  static const u_long err_illtime;  // time out of range
  static const u_long err_illticks; // ticks out of range

  /// date : year in bits 31-16, month in bits 15-8, day in bits 7-0
   u_long date_;

  /// time : hour in bits 31-16, minutes in bits 15-8, seconds in bits 7-0
  u_long time_;

  /// ticks: number of system clock ticks (KC_TICKS2SEC-1 max)
  u_long ticks_;
} ;
#endif /* ACE_PSOS_HAS_TIME */

#if defined (ACE_HAS_WINCE)
  /// Supporting data for ctime and ctime_r functions on WinCE.
  const wchar_t *day_of_week_name[7];
  const wchar_t *month_name[12];

// WinCE doesn't have most of the standard C library time functions. It
// also doesn't define struct tm. SYSTEMTIME has pretty much the same
// info though, so we can map it when needed. Define struct tm here and
// use it when needed. This is taken from the standard C library.
struct tm {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;      // Day of the month
  int tm_mon;
  int tm_year;
  int tm_wday;      // Day of the week
  int tm_yday;      // Day in the year
  int tm_isdst;     // >0 if dst in effet; 0 if not; <0 if unknown
};
#endif /* ACE_HAS_WINCE */

/// Helper for the ACE_OS::timezone() function
/**
 * We put all the timezone stuff that used to be in ACE_OS::timezone()
 * here because on some platforms "timezone" is a macro.  Because of this,
 * the name ACE_OS::timezone will cause errors.  So in order to use the
 * macro as it is defined but also keep the name ACE_OS::timezone, we
 * use timezone first here in this inline function, and then undefine
 * timezone.
 */
inline long ace_timezone()
{
#if !defined (VXWORKS) && !defined (ACE_PSOS) && !defined (CHORUS)
# if defined (ACE_HAS_WINCE)
  TIME_ZONE_INFORMATION tz;
  GetTimeZoneInformation (&tz);
  return tz.Bias * 60;
# elif defined (ACE_WIN32)
  return _timezone;  // For Win32.
# elif ( defined (__Lynx__) || defined (__FreeBSD__) || defined (ACE_HAS_SUNOS4_GETTIMEOFDAY) ) && ( !defined (__linux__) )
  long result = 0;
  struct timeval time;
  struct timezone zone;
  ACE_UNUSED_ARG (result);
  ACE_OSCALL (::gettimeofday (&time, &zone), int, -1, result);
  return zone.tz_minuteswest * 60;
# else  /* __Lynx__ || __FreeBSD__ ... */
# if defined (__linux__)
  // Under Linux, gettimeofday() does not correctly set the timezone
  // struct, so we should use the global variable <timezone>.
  // However, it is initialized by tzset().  I assume other systems
  // are the same (i.e., tzset() needs to be called to set
  // <timezone>), but since no one is complaining, I will only make
  // the change for Linux.
  ::tzset();
# endif
  return timezone;
# endif /* __Lynx__ || __FreeBSD__ ... */
#else
  ACE_NOTSUP_RETURN (0);
#endif /* !ACE_HAS_WINCE && !VXWORKS && !ACE_PSOS */
}


#if !defined (ACE_LACKS_DIFFTIME)
/// Helper for the ACE_OS::difftime() function
/**
 * We moved the difftime code that used to be in ACE_OS::difftime()
 * here because on some platforms "difftime" is a macro.  Because of this,
 * the name ACE_OS::difftime will cause errors.  So in order to use the
 * macro as it is defined but also keep the name ACE_OS::difftime, we
 * use difftime first here in this inline function, and then undefine
 * it.
 */
inline double ace_difftime(time_t t1, time_t t0)
{
# if defined (ACE_PSOS) && ! defined (ACE_PSOS_HAS_TIME)
  // simulate difftime ; just subtracting ; ACE_PSOS case
  return ((double)t1) - ((double)t0);
# else
  return difftime (t1, t0);
# endif /* ACE_PSOS !ACE_PSOS_HAS_TIME */
}
#endif /* !ACE_LACKS_DIFFTIME */

# if defined (ACE_WIN32)
#   if !defined (ACE_LACKS_LONGLONG_T)
// 64-bit quad-word definitions.
typedef unsigned __int64 ACE_QWORD;
typedef unsigned __int64 ACE_hrtime_t;
inline ACE_QWORD ACE_MAKE_QWORD (DWORD lo, DWORD hi) { return ACE_QWORD (lo) | (ACE_QWORD (hi) << 32); }
inline DWORD ACE_LOW_DWORD  (ACE_QWORD q) { return (DWORD) q; }
inline DWORD ACE_HIGH_DWORD (ACE_QWORD q) { return (DWORD) (q >> 32); }
#   else
// Can't find ANY place that ACE_QWORD is used, but hrtime_t is.
typedef ACE_UINT64 ACE_hrtime_t;
#   endif // ACE_LACKS_LONGLONG_T
# elif defined (ACE_PSOS)
typedef ACE_UINT64 ACE_hrtime_t;
# else /* !ACE_WIN32 && !ACE_PSOS */
#   if defined (ACE_HAS_HI_RES_TIMER) &&  !defined (ACE_LACKS_LONGLONG_T)
  /* hrtime_t is defined on systems (Suns) with ACE_HAS_HI_RES_TIMER */
  typedef hrtime_t ACE_hrtime_t;
#   else  /* ! ACE_HAS_HI_RES_TIMER  ||  ACE_LACKS_LONGLONG_T */
  typedef ACE_UINT64 ACE_hrtime_t;
#   endif /* ! ACE_HAS_HI_RES_TIMER  ||  ACE_LACKS_LONGLONG_T */
# endif /* ACE_WIN32 */



namespace ACE_OS {

# if defined (CHORUS) && !defined (CHORUS_4)
  // We must format this code as follows to avoid confusing OSE.
  enum ACE_HRTimer_Op
    {
      ACE_HRTIMER_START = K_BSTART,
      ACE_HRTIMER_INCR = K_BPOINT,
      ACE_HRTIMER_STOP = K_BSTOP,
      ACE_HRTIMER_GETTIME = 0xFFFF
    };
# else  /* ! CHORUS */
  enum ACE_HRTimer_Op
    {
      ACE_HRTIMER_START = 0x0,  // Only use these if you can stand
      ACE_HRTIMER_INCR = 0x1,   // for interrupts to be disabled during
      ACE_HRTIMER_STOP = 0x2,   // the timed interval!!!!
      ACE_HRTIMER_GETTIME = 0xFFFF
    };
# endif /* ! CHORUS */

  //@{ @name A set of wrappers for operations on time.

  char *asctime (const struct tm *tm);

  char *asctime_r (const struct tm *tm,
                   char *buf, int buflen);

  int clock_gettime (clockid_t,
                     struct timespec *);

  ACE_TCHAR *ctime (const time_t *t);

  ACE_TCHAR *ctime_r (const time_t *clock, ACE_TCHAR *buf, int buflen);

# if defined (difftime)
#   undef difftime
# endif /* difftime */

  double difftime (time_t t1,
                   time_t t0);

  ACE_hrtime_t gethrtime (const ACE_HRTimer_Op = ACE_HRTIMER_GETTIME);

  struct tm *gmtime (const time_t *clock);

  struct tm *gmtime_r (const time_t *clock,
                       struct tm *res);

  struct tm *localtime (const time_t *clock);

  struct tm *localtime_r (const time_t *clock,
                          struct tm *res);

  // Get the current time.
  time_t mktime (struct tm *timeptr);

  int nanosleep (const struct timespec *requested,
                 struct timespec *remaining = 0);

# if defined (ACE_HAS_POWERPC_TIMER) && (defined (ghs) || defined (__GNUG__))
  void readPPCTimeBase (u_long &most,
                        u_long &least);
# endif /* ACE_HAS_POWERPC_TIMER  &&  (ghs or __GNUG__) */

  size_t strftime (char *s,
                   size_t maxsize,
                   const char *format,
                   const struct tm *timeptr);

#if defined (ACE_HAS_STRPTIME)
  char *strptime (char *buf,
                  const char *format,
                  struct tm *tm);

# if defined (ACE_LACKS_NATIVE_STRPTIME)
  int strptime_getnum (char *buf, int *num, int *bi,
                       int *fi, int min, int max);
# endif /* ACE_LACKS_NATIVE_STRPTIME */
#endif /* ACE_HAS_STRPTIME */

  time_t time (time_t *tloc = 0);

# if defined (timezone)
#   undef timezone
# endif /* timezone */

  long timezone (void);

  // wrapper for time zone information.
  void tzset (void);

  //@}
} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_time.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_TIME_H */
