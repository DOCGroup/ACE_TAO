// -*- C++ -*-
// $Id$

#include "ace/OS_NS_time.h"

ACE_RCSID(ace, OS_NS_time, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_time.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

#include "ace/OS_NS_Thread.h"
#include "ace/Object_Manager_Base.h"

// hmmm, should this be only for ACE_MT_SAFE? dhinton
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
# if defined (ACE_HAS_WINCE)
const wchar_t *ACE_OS::day_of_week_name[] = {ACE_LIB_TEXT ("Sun"), ACE_LIB_TEXT ("Mon"),
                                             ACE_LIB_TEXT ("Tue"), ACE_LIB_TEXT ("Wed"),
                                             ACE_LIB_TEXT ("Thu"), ACE_LIB_TEXT ("Fri"),
                                             ACE_LIB_TEXT ("Sat")};
const wchar_t *ACE_OS::month_name[] = {ACE_LIB_TEXT ("Jan"), ACE_LIB_TEXT ("Feb"),
                                       ACE_LIB_TEXT ("Mar"), ACE_LIB_TEXT ("Apr"),
                                       ACE_LIB_TEXT ("May"), ACE_LIB_TEXT ("Jun"),
                                       ACE_LIB_TEXT ("Jul"), ACE_LIB_TEXT ("Aug"),
                                       ACE_LIB_TEXT ("Sep"), ACE_LIB_TEXT ("Oct"),
                                       ACE_LIB_TEXT ("Nov"), ACE_LIB_TEXT ("Dec") };

static const ACE_TCHAR *ACE_OS_CTIME_R_FMTSTR = ACE_LIB_TEXT ("%3s %3s %02d %02d:%02d:%02d %04d\n");
# endif /* ACE_HAS_WINCE */
#endif /* ACE_MT_SAFE && ACE_MT_SAFE != 0 */

# if defined (ACE_PSOS)

// bit masks and shifts for prying info out of the pSOS time encoding
const u_long ACE_PSOS_Time_t::year_mask     = 0x0000FFFFul;
const u_long ACE_PSOS_Time_t::month_mask    = 0x000000FFul;
const u_long ACE_PSOS_Time_t::day_mask      = 0x000000FFul;
const u_long ACE_PSOS_Time_t::hour_mask     = 0x0000FFFFul;
const u_long ACE_PSOS_Time_t::minute_mask   = 0x000000FFul;
const u_long ACE_PSOS_Time_t::second_mask   = 0x000000FFul;
const int ACE_PSOS_Time_t::year_shift   = 16;
const int ACE_PSOS_Time_t::month_shift  = 8;
const int ACE_PSOS_Time_t::hour_shift   = 16;
const int ACE_PSOS_Time_t::minute_shift = 8;
const int ACE_PSOS_Time_t::year_origin = 1900;
const int ACE_PSOS_Time_t::month_origin = 1;

// maximum number of clock ticks supported
const u_long ACE_PSOS_Time_t::max_ticks = ~0UL;

ACE_PSOS_Time_t::ACE_PSOS_Time_t (void)
  : date_ (0),
    time_ (0),
    ticks_ (0)
{
}

// default ctor: date, time, and ticks all zeroed

ACE_PSOS_Time_t::ACE_PSOS_Time_t (const timespec_t& t)
{
  struct tm* tm_struct = ACE_OS::gmtime (&(t.tv_sec));

  // Encode date values from tm struct into pSOS date bit array.
  date_ = (ACE_PSOS_Time_t::year_mask &
           ACE_static_cast (u_long,
                            tm_struct->tm_year + ACE_PSOS_Time_t::year_origin)) <<
    ACE_PSOS_Time_t::year_shift;
  date_ |= (ACE_PSOS_Time_t::month_mask &
            ACE_static_cast (u_long,
                             tm_struct->tm_mon + ACE_PSOS_Time_t::month_origin)) <<
    ACE_PSOS_Time_t::month_shift;
  date_ |= ACE_PSOS_Time_t::day_mask &
    ACE_static_cast (u_long, tm_struct->tm_mday);
  // Encode time values from tm struct into pSOS time bit array.
  time_ = (ACE_PSOS_Time_t::hour_mask  &
            ACE_static_cast (u_long, tm_struct->tm_hour)) <<
    ACE_PSOS_Time_t::hour_shift;
  time_ |= (ACE_PSOS_Time_t::minute_mask &
            ACE_static_cast (u_long, tm_struct->tm_min)) <<
    ACE_PSOS_Time_t::minute_shift;
  time_ |= ACE_PSOS_Time_t::second_mask &
    ACE_static_cast (u_int, tm_struct->tm_sec);

  // encode nanoseconds as system clock ticks
  ticks_ = ACE_static_cast (u_long,
                            ((ACE_static_cast (double, t.tv_nsec) *
                              ACE_static_cast (double, KC_TICKS2SEC)) /
                             ACE_static_cast (double, 1000000000)));

}

// ctor from a timespec_t

ACE_PSOS_Time_t::operator timespec_t (void)
{
  struct tm tm_struct;

  // Decode date and time bit arrays and fill in fields of tm_struct.

  tm_struct.tm_year =
    ACE_static_cast (int, (ACE_PSOS_Time_t::year_mask &
                           (date_ >> ACE_PSOS_Time_t::year_shift))) -
    ACE_PSOS_Time_t::year_origin;
  tm_struct.tm_mon =
    ACE_static_cast (int, (ACE_PSOS_Time_t::month_mask &
                           (date_ >> ACE_PSOS_Time_t::month_shift))) -
    ACE_PSOS_Time_t::month_origin;
  tm_struct.tm_mday =
    ACE_static_cast (int, (ACE_PSOS_Time_t::day_mask & date_));
  tm_struct.tm_hour =
    ACE_static_cast (int, (ACE_PSOS_Time_t::hour_mask &
                           (time_ >> ACE_PSOS_Time_t::hour_shift)));
  tm_struct.tm_min =
    ACE_static_cast (int, (ACE_PSOS_Time_t::minute_mask &
                           (time_ >> ACE_PSOS_Time_t::minute_shift)));
  tm_struct.tm_sec =
    ACE_static_cast (int, (ACE_PSOS_Time_t::second_mask & time_));

  // Indicate values we don't know as negative numbers.
  tm_struct.tm_wday  = -1;
  tm_struct.tm_yday  = -1;
  tm_struct.tm_isdst = -1;

  timespec_t t;

  // Convert calendar time to time struct.
  t.tv_sec = ACE_OS::mktime (&tm_struct);

  // Encode nanoseconds as system clock ticks.
  t.tv_nsec = ACE_static_cast (long,
                               ((ACE_static_cast (double, ticks_) *
                                 ACE_static_cast (double, 1000000000)) /
                                ACE_static_cast (double, KC_TICKS2SEC)));
  return t;
}

// type cast operator (to a timespec_t)

u_long
ACE_PSOS_Time_t::get_system_time (ACE_PSOS_Time_t& t)
{
  u_long ret_val = 0;

#   if defined (ACE_PSOSIM) // system time is broken in simulator.
  timeval tv;
  int result = 0;
  ACE_OSCALL (::gettimeofday (&tv, 0), int, -1, result);
  if (result == -1)
    return 1;

  ACE_Time_Value atv (tv);
  timespec ts = atv;
  ACE_PSOS_Time_t pt (ts);
  t.date_ = pt.date_;
  t.time_ = pt.time_;
  t.ticks_ = pt.ticks_;
#   else
  ret_val = tm_get (&(t.date_), &(t.time_), &(t.ticks_));
#   endif  /* ACE_PSOSIM */
  return ret_val;
}

// Static member function to get current system time.

u_long
ACE_PSOS_Time_t::set_system_time (const ACE_PSOS_Time_t& t)
{
  return tm_set (t.date_, t.time_, t.ticks_);
}

// Static member function to set current system time.

#   if defined (ACE_PSOSIM)

u_long
ACE_PSOS_Time_t::init_simulator_time (void)
{
  // This is a hack using a direct UNIX system call, because the
  // appropriate ACE_OS method ultimately uses the pSOS tm_get
  // function, which would fail because the simulator's system time is
  // uninitialized (chicken and egg).
  timeval t;
  int result = 0;
  ACE_OSCALL (::gettimeofday (&t, 0),
              int,
              -1,
              result);

  if (result == -1)
    return 1;
  else
    {
      ACE_Time_Value tv (t);
      timespec ts = tv;
      ACE_PSOS_Time_t pt (ts);
      u_long ret_val =
        ACE_PSOS_Time_t::set_system_time (pt);
      return ret_val;

    }
}

// Static member function to initialize system time, using UNIX calls.

#   endif /* ACE_PSOSIM */
# endif /* ACE_PSOS && ! ACE_PSOS_DIAB_MIPS */

# if defined (ACE_HAS_WINCE)
ACE_TCHAR *
ACE_OS::ctime_r (const time_t *clock, ACE_TCHAR *buf, int buflen)
{
  // buflen must be at least 26 wchar_t long.
  if (buflen < 26)              // Again, 26 is a magic number.
    {
      errno = ERANGE;
      return 0;
    }
  // This is really stupid, converting FILETIME to timeval back and
  // forth.  It assumes FILETIME and DWORDLONG are the same structure
  // internally.
  ULARGE_INTEGER _100ns;
  _100ns.QuadPart = (DWORDLONG) *clock * 10000 * 1000
                     + ACE_Time_Value::FILETIME_to_timval_skew;
  FILETIME file_time;
  file_time.dwLowDateTime = _100ns.LowPart;
  file_time.dwHighDateTime = _100ns.HighPart;

  FILETIME localtime;
  SYSTEMTIME systime;
  FileTimeToLocalFileTime (&file_time, &localtime);
  FileTimeToSystemTime (&localtime, &systime);
  ACE_OS::sprintf (buf, ACE_OS_CTIME_R_FMTSTR,
                   ACE_OS::day_of_week_name[systime.wDayOfWeek],
                   ACE_OS::month_name[systime.wMonth - 1],
                   systime.wDay,
                   systime.wHour,
                   systime.wMinute,
                   systime.wSecond,
                   systime.wYear);
  return buf;
}
# endif /* ACE_HAS_WINCE */

# if defined (ACE_LACKS_DIFFTIME)
double
ACE_OS::difftime (time_t t1, time_t t0)
{
  /* return t1 - t0 in seconds */
  struct tm tms[2], *ptms[2], temp;
  double seconds;
  double days;
  int swap = 0;

  /* extract the tm structure from time_t */
  ptms[1] = gmtime_r (&t1, &tms[1]);
  if (ptms[1] == 0) return 0.0;

  ptms[0] = gmtime_r (&t0, &tms[0]);
  if (ptms[0] == 0) return 0.0;

    /* make sure t1 is > t0 */
  if (tms[1].tm_year < tms[0].tm_year)
    swap = 1;
  else if (tms[1].tm_year == tms[0].tm_year)
    {
      if (tms[1].tm_yday < tms[0].tm_yday)
        swap = 1;
      else if (tms[1].tm_yday == tms[0].tm_yday)
        {
          if (tms[1].tm_hour < tms[0].tm_hour)
            swap = 1;
          else if (tms[1].tm_hour == tms[0].tm_hour)
            {
              if (tms[1].tm_min < tms[0].tm_min)
                swap = 1;
              else if (tms[1].tm_min == tms[0].tm_min)
                {
                  if (tms[1].tm_sec < tms[0].tm_sec)
                    swap = 1;
                }
            }
        }
    }

  if (swap)
    temp = tms[0], tms[0] = tms[1], tms[1] = temp;

  seconds = 0.0;
  if (tms[1].tm_year > tms[0].tm_year)
    {
      // Accumulate the time until t[0] catches up to t[1]'s year.
      seconds = 60 - tms[0].tm_sec;
      tms[0].tm_sec = 0;
      tms[0].tm_min += 1;
      seconds += 60 * (60 - tms[0].tm_min);
      tms[0].tm_min = 0;
      tms[0].tm_hour += 1;
      seconds += 60*60 * (24 - tms[0].tm_hour);
      tms[0].tm_hour = 0;
      tms[0].tm_yday += 1;

#   define ISLEAPYEAR(y) ((y)&3u?0:(y)%25u?1:(y)/25u&12?0:1)

      if (ISLEAPYEAR(tms[0].tm_year))
        seconds += 60*60*24 * (366 - tms[0].tm_yday);
      else
        seconds += 60*60*24 * (365 - tms[0].tm_yday);

      tms[0].tm_yday = 0;
      tms[0].tm_year += 1;

      while (tms[1].tm_year > tms[0].tm_year)
        {
          if (ISLEAPYEAR(tms[0].tm_year))
            seconds += 60*60*24 * 366;
          else
            seconds += 60*60*24 * 365;

          tms[0].tm_year += 1;
        }

#   undef ISLEAPYEAR

    }
  else
    {
      // Normalize
      if (tms[1].tm_sec < tms[0].tm_sec)
        {
          if (tms[1].tm_min == 0)
            {
              if (tms[1].tm_hour == 0)
                {
                  tms[1].tm_yday -= 1;
                  tms[1].tm_hour += 24;
                }
              tms[1].tm_hour -= 1;
              tms[1].tm_min += 60;
            }
          tms[1].tm_min -= 1;
          tms[1].tm_sec += 60;
        }
      tms[1].tm_sec -= tms[0].tm_sec;

      if (tms[1].tm_min < tms[0].tm_min)
        {
          if (tms[1].tm_hour == 0)
            {
              tms[1].tm_yday -= 1;
              tms[1].tm_hour += 24;
            }
          tms[1].tm_hour -= 1;
          tms[1].tm_min += 60;
        }
      tms[1].tm_min -= tms[0].tm_min;

      if (tms[1].tm_hour < tms[0].tm_hour)
        {
          tms[1].tm_yday -= 1;
          tms[1].tm_hour += 24;
        }
      tms[1].tm_hour -= tms[0].tm_hour;

      tms[1].tm_yday -= tms[0].tm_yday;
    }

  // accumulate the seconds
  seconds += tms[1].tm_sec;
  seconds += 60 * tms[1].tm_min;
  seconds += 60*60 * tms[1].tm_hour;
  seconds += 60*60*24 * tms[1].tm_yday;

  return seconds;
}
# endif /* ACE_LACKS_DIFFTIME */

struct tm *
ACE_OS::localtime_r (const time_t *t, struct tm *res)
{
  ACE_OS_TRACE ("ACE_OS::localtime_r");
#if defined (ACE_HAS_REENTRANT_FUNCTIONS)
# if defined (DIGITAL_UNIX)
  ACE_OSCALL_RETURN (::_Plocaltime_r (t, res), struct tm *, 0);
# elif defined (HPUX_10)
  return (::localtime_r (t, res) == 0 ? res : (struct tm *)0);
# else
  ACE_OSCALL_RETURN (::localtime_r (t, res), struct tm *, 0);
# endif /* DIGITAL_UNIX */
#elif !defined (ACE_HAS_WINCE) && !defined(ACE_PSOS) || defined (ACE_PSOS_HAS_TIME)
  ACE_OS_GUARD

  ACE_UNUSED_ARG (res);
  struct tm * res_ptr;
  ACE_OSCALL (::localtime (t), struct tm *, 0, res_ptr);
  if (res_ptr == 0)
    return 0;
  else
    {
      *res = *res_ptr;
      return res;
    }
#elif defined (ACE_HAS_WINCE)
  // This is really stupid, converting FILETIME to timeval back and
  // forth.  It assumes FILETIME and DWORDLONG are the same structure
  // internally.

  TIME_ZONE_INFORMATION pTz;

  const unsigned short int __mon_yday[2][13] =
  {
    /* Normal years.  */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
    /* Leap years.  */
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
  };

  ULARGE_INTEGER _100ns;
  ::GetTimeZoneInformation (&pTz);

  _100ns.QuadPart = (DWORDLONG) *t * 10000 * 1000 + ACE_Time_Value::FILETIME_to_timval_skew;
  FILETIME file_time;
  file_time.dwLowDateTime = _100ns.LowPart;
  file_time.dwHighDateTime = _100ns.HighPart;

  FILETIME localtime;
  SYSTEMTIME systime;
  FileTimeToLocalFileTime (&file_time, &localtime);
  FileTimeToSystemTime (&localtime, &systime);

  res->tm_hour = systime.wHour;

  if(pTz.DaylightBias!=0)
    res->tm_isdst = 1;
  else
    res->tm_isdst = 1;

   int iLeap;
   iLeap = (res->tm_year % 4 == 0 && (res->tm_year% 100 != 0 || res->tm_year % 400 == 0));
   // based on leap select which group to use

   res->tm_mday = systime.wDay;
   res->tm_min = systime.wMinute;
   res->tm_mon = systime.wMonth;
   res->tm_sec = systime.wSecond;
   res->tm_wday = systime.wDayOfWeek;
   res->tm_yday = __mon_yday[iLeap][systime.wMonth] + systime.wDay;
   res->tm_year = systime.wYear;// this the correct year but bias the value to start at the 1900
   res->tm_year = res->tm_year - 1900;

   return res;
#else
  // @@ Same as ACE_OS::localtime (), you need to implement it
  //    yourself.
  ACE_UNUSED_ARG (t);
  ACE_UNUSED_ARG (res);
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_REENTRANT_FUNCTIONS */
}

time_t
ACE_OS::mktime (struct tm *t)
{
  ACE_OS_TRACE ("ACE_OS::mktime");
#   if defined (ACE_PSOS) && ! defined (ACE_PSOS_HAS_TIME)
  ACE_UNUSED_ARG (t);
  ACE_NOTSUP_RETURN (-1);
#   elif defined (ACE_HAS_WINCE)
  SYSTEMTIME t_sys;
  FILETIME t_file;
  t_sys.wSecond = t->tm_sec;
  t_sys.wMinute = t->tm_min;
  t_sys.wHour = t->tm_hour;
  t_sys.wDay = t->tm_mday;
  t_sys.wMonth = t->tm_mon + 1;  // SYSTEMTIME is 1-indexed, tm is 0-indexed
  t_sys.wYear = t->tm_year + 1900; // SYSTEMTIME is real; tm is since 1900
  t_sys.wDayOfWeek = t->tm_wday;  // Ignored in below function call.
  if (SystemTimeToFileTime (&t_sys, &t_file) == 0)
    return -1;
  ACE_Time_Value tv (t_file);
  return tv.sec ();
#   else
#     if defined (ACE_HAS_THREADS)  &&  !defined (ACE_HAS_MT_SAFE_MKTIME)
  ACE_OS_GUARD
#     endif /* ACE_HAS_THREADS  &&  ! ACE_HAS_MT_SAFE_MKTIME */

  ACE_OSCALL_RETURN (::mktime (t), time_t, (time_t) -1);
#   endif /* ACE_PSOS && ! ACE_PSOS_HAS_TIME */
}

#if defined (ACE_HAS_POWERPC_TIMER) && defined (ghs)
void
ACE_OS::readPPCTimeBase (u_long &most, u_long &least)
{
  ACE_OS_TRACE ("ACE_OS::readPPCTimeBase");

  // This function can't be inline because it depends on the arguments
  // being in particular registers (r3 and r4), in conformance with the
  // EABI standard.  It would be nice if we knew how to put the variable
  // names directly into the assembler instructions . . .
  asm("aclock:");
  asm("mftb  r5,TBU");
  asm("mftb  r6,TBL");
  asm("mftb  r7,TBU");
  asm("cmpw  r5,r7");
  asm("bne   aclock");

  asm("stw r5, 0(r3)");
  asm("stw r6, 0(r4)");
}
#elif defined (ACE_HAS_POWERPC_TIMER) && defined (__GNUG__)
void
ACE_OS::readPPCTimeBase (u_long &most, u_long &least)
{
  ACE_OS_TRACE ("ACE_OS::readPPCTimeBase");

  // This function can't be inline because it defines a symbol,
  // aclock.  If there are multiple calls to the function in a
  // compilation unit, then that symbol would be multiply defined if
  // the function was inline.
  asm volatile ("aclock:\n"
                "mftbu 5\n"     /* upper time base register */
                "mftb 6\n"      /* lower time base register */
                "mftbu 7\n"     /* upper time base register */
                "cmpw 5,7\n" /* check for rollover of upper */
                "bne aclock\n"
                "stw 5,%0\n"                        /* most */
                "stw 6,%1"                         /* least */
                : "=m" (most), "=m" (least)      /* outputs */
                :                              /* no inputs */
                : "5", "6", "7", "memory"    /* constraints */);
}
#endif /* ACE_HAS_POWERPC_TIMER  &&  (ghs or __GNUG__) */

#if defined (ACE_HAS_STRPTIME)
char *
ACE_OS::strptime (char *buf, const char *format, struct tm *tm)
{
#if !defined (ACE_HAS_WINCE)
#if defined (ACE_LACKS_NATIVE_STRPTIME)
  int bi = 0;
  int fi = 0;
  int percent = 0;

  if (!buf || !format)
    return 0;

  while (format[fi] != '\0')
    {
      if (percent)
        {
          percent = 0;
          switch (format[fi])
            {
            case '%':                        // an escaped %
              if (buf[bi] == '%')
                {
                  fi++; bi++;
                }
              else
                return buf + bi;
              break;

              /* not supported yet: weekday via locale long/short names
                 case 'a':                        / * weekday via locale * /
                 / * FALL THROUGH * /
                 case 'A':                        / * long/short names * /
                 break;
                 */

              /* not supported yet:
                 case 'b':                        / * month via locale * /
                 / * FALL THROUGH * /
                 case 'B':                        / * long/short names * /
                 / * FALL THROUGH * /
                 case 'h':
                 break;
                 */

              /* not supported yet:
                 case 'c':                        / * %x %X * /
                 break;
                 */

              /* not supported yet:
                 case 'C':                        / * date & time -  * /
                 / * locale long format * /
                 break;
                 */

            case 'd':                        /* day of month (1-31) */
              /* FALL THROUGH */
            case 'e':
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
                return buf + bi;

              break;

            case 'D':                        /* %m/%d/%y */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
                return buf + bi;

              fi--;
              tm->tm_mon--;

              if (buf[bi] != '/')
                return buf + bi;

              bi++;

              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mday, &bi, &fi, 1, 31))
                return buf + bi;

              fi--;
              if (buf[bi] != '/')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
                return buf + bi;
              if (tm->tm_year < 69)
                tm->tm_year += 100;
              break;

            case 'H':                        /* hour (0-23) */
              /* FALL THROUGH */
            case 'k':
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return buf + bi;
              break;

              /* not supported yet:
                 case 'I':                        / * hour (0-12) * /
                 / * FALL THROUGH * /
                 case 'l':
                 break;
                 */

            case 'j':                        /* day of year (0-366) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_yday, &bi, &fi, 1, 366))
                return buf + bi;

              tm->tm_yday--;
              break;

            case 'm':                        /* an escaped % */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_mon, &bi, &fi, 1, 12))
                return buf + bi;

              tm->tm_mon--;
              break;

            case 'M':                        /* minute (0-59) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return buf + bi;

              break;

              /* not supported yet:
                 case 'p':                        / * am or pm for locale * /
                 break;
                 */

              /* not supported yet:
                 case 'r':                        / * %I:%M:%S %p * /
                 break;
                 */

            case 'R':                        /* %H:%M */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return buf + bi;

              fi--;
              if (buf[bi] != ':')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return buf + bi;

              break;

            case 'S':                        /* seconds (0-61) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
                return buf + bi;
              break;

            case 'T':                        /* %H:%M:%S */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_hour, &bi, &fi, 0, 23))
                return buf + bi;

              fi--;
              if (buf[bi] != ':')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_min, &bi, &fi, 0, 59))
                return buf + bi;

              fi--;
              if (buf[bi] != ':')
                return buf + bi;
              bi++;
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_sec, &bi, &fi, 0, 61))
                return buf + bi;

              break;

            case 'w':                        /* day of week (0=Sun-6) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_wday, &bi, &fi, 0, 6))
                return buf + bi;

              break;

              /* not supported yet: date, based on locale
                 case 'x':                        / * date, based on locale * /
                 break;
                 */

              /* not supported yet:
                 case 'X':                        / * time, based on locale * /
                 break;
                 */

            case 'y':                        /* the year - 1900 (0-99) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_year, &bi, &fi, 0, 99))
                return buf + bi;

              if (tm->tm_year < 69)
                tm->tm_year += 100;
              break;

            case 'Y':                        /* the full year (1999) */
              if (!ACE_OS::strptime_getnum (buf + bi, &tm->tm_year, &bi, &fi, 0, 0))
                return buf + bi;

              tm->tm_year -= 1900;
              break;

            default:                        /* unrecognised */
              return buf + bi;
            } /* switch (format[fi]) */

        }
      else
        { /* if (percent) */
          if (format[fi] == '%')
            {
              percent = 1;
              fi++;
            }
          else
            {
              if (format[fi] == buf[bi])
                {
                  fi++;
                  bi++;
                }
              else
                return buf + bi;
            }
        } /* if (percent) */
    } /* while (format[fi] */

  return buf + bi;
#else  /* ! ACE_LACKS_NATIVE_STRPTIME */
  return ::strptime (buf,
                     format,
                     tm);
#endif /* ! ACE_LACKS_NATIVE_STRPTIME */
#else /* ! ACE_HAS_WINCE */
  ACE_UNUSED_ARG (buf);
  ACE_UNUSED_ARG (format);
  ACE_UNUSED_ARG (tm);

  ACE_NOTSUP_RETURN (0);
#endif /* ! ACE_HAS_WINCE */
}

# if defined (ACE_LACKS_NATIVE_STRPTIME)
int
ACE_OS::strptime_getnum (char *buf,
                         int *num,
                         int *bi,
                         int *fi,
                         int min,
                         int max)
{
  int i = 0, tmp = 0;

  while (isdigit (buf[i]))
    {
      tmp = (tmp * 10) + (buf[i] - '0');
      if (max && (tmp > max))
        return 0;
      i++;
    }

  if (tmp < min)
    return 0;
  else if (i)
    {
      *num = tmp;
      (*fi)++;
      *bi += i;
      return 1;
    }
  else
    return 0;
}
# endif /* ACE_LACKS_NATIVE_STRPTIME */
#endif /* ACE_HAS_STRPTIME */
