// $Id$

#define ACE_BUILD_DLL
#include "ace/High_Res_Timer.h"

#if !defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

#include "ace/Stats.h"

ACE_ALLOC_HOOK_DEFINE(ACE_High_Res_Timer)

// For Intel platforms, a scale factor is required for
// ACE_OS::gethrtime.  We'll still set this to one to prevent division
// by zero errors.
#if defined (ACE_HAS_PENTIUM)

# include "ace/Object_Manager.h"
# include "ace/Synch.h"

# if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

u_long
ACE_High_Res_Timer::get_registry_scale_factor (void)
{
  HKEY hk;
  unsigned long speed;
  unsigned long speed_size = sizeof speed;
  unsigned long speed_type = REG_DWORD;

  long rc = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE,
                            __TEXT ("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"),
                            NULL,
                            KEY_READ,
                            &hk);

  if (rc != ERROR_SUCCESS)
    // Couldn't find key
    return 1;

  rc = ::RegQueryValueEx (hk,
                          __TEXT ("~MHz"),
                          0,
                          &speed_type,
                          (LPBYTE) &speed,
                          &speed_size);

  ::RegCloseKey (hk);

  if (rc != ERROR_SUCCESS)
    // Couldn't get the value
    return 1;

  return speed;
}

/* static */
  // Initialize the global_scale_factor_ to 1.  The first
  // ACE_High_Res_Timer instance construction will override this
  // value.
  ACE_UINT32 ACE_High_Res_Timer::global_scale_factor_ = 1u;
# elif defined (ghs) || defined (__GNUG__)
  // Initialize the global_scale_factor_ to 1.  The first
  // ACE_High_Res_Timer instance construction will override this
  // value.
  ACE_UINT32 ACE_High_Res_Timer::global_scale_factor_ = 1u;
# endif /* ! ACE_WIN32 && ! ghs && ! __GNUG__ */
#elif defined (ACE_HAS_HI_RES_TIMER) || defined (ACE_HAS_AIX_HI_RES_TIMER) || \
  defined (ACE_HAS_CLOCK_GETTIME) || defined (ACE_PSOS) || \
  defined (CHORUS) || defined (linux)
  // A scale_factor of 1000 converts nanosecond ticks to microseconds.
  // That is, on these platforms, 1 tick == 1 nanosecond.
  /* static */
  ACE_UINT32 ACE_High_Res_Timer::global_scale_factor_ = 1000u;
#else
  // Don't convert at all, by default.  Application code may have to
  // override this using a call to global_scale_factor ().
  ACE_UINT32 ACE_High_Res_Timer::global_scale_factor_ = 1u;
#endif /* ACE_WIN32 */

ACE_UINT32
ACE_High_Res_Timer::global_scale_factor ()
{
#if defined (ACE_HAS_PENTIUM) && \
    ((defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || \
     defined (ghs) || defined (__GNUG__))
  // Check if the global scale factor needs to be set, and do if so.
  if (ACE_High_Res_Timer::global_scale_factor_ == 1u)
    {
      // Grab ACE's static object lock.  This doesn't have anything to
      // do with static objects; it's just a convenient lock to use.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));

      // Double check
      if (ACE_High_Res_Timer::global_scale_factor_ == 1u)
        {
#         if defined (ACE_WIN32)
            ACE_High_Res_Timer::global_scale_factor (
              ACE_High_Res_Timer::get_registry_scale_factor ());
#         elif defined (linux)
            // Get the BogoMIPS from /proc.
            FILE *cpuinfo;
            if ((cpuinfo = ACE_OS::fopen ("/proc/cpuinfo", "r")))
              {
                char buf[128];
                while (ACE_OS::fgets (buf, sizeof buf, cpuinfo))
                  {
                    ACE_UINT32 whole, fractional;
                    if (::sscanf (buf, "BogoMIPS : %d.%d\n",
                                  &whole, &fractional) == 2 ||
                        ::sscanf (buf, "bogomips : %d.%d\n",
                                  &whole, &fractional) == 2)
                      {
                        ACE_High_Res_Timer::global_scale_factor (whole);
                        break;
                      }
                  }
                ACE_OS::fclose (cpuinfo);
              }
#         endif /* ! ACE_WIN32 && ! linux */

          if (ACE_High_Res_Timer::global_scale_factor_ == 1u)
            // Failed to retrieve CPU speed from system, so calculate it.
            ACE_High_Res_Timer::calibrate ();
        }
    }
#endif /* ACE_HAS_PENTIUM && ((WIN32 && ! WINCE) || ghs || __GNUG__) */

  return global_scale_factor_;
}

ACE_High_Res_Timer::ACE_High_Res_Timer (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::ACE_High_Res_Timer");

  this->reset ();

  // Make sure that the global scale factor is set.
  (void) global_scale_factor ();
}

ACE_UINT32
ACE_High_Res_Timer::calibrate (const ACE_UINT32 usec,
                               const u_int iterations)
{
  const ACE_Time_Value sleep_time (0, usec);
  ACE_Stats delta_hrtime;
  ACE_Stats actual_sleeps; /* In units of 100 usec, to avoid overflow. */

  for (u_int i = 0; i < iterations; ++i)
    {
      const ACE_Time_Value actual_start = ACE_OS::gettimeofday ();
      const ACE_hrtime_t start = ACE_OS::gethrtime ();
      ACE_OS::sleep (sleep_time);
      const ACE_hrtime_t stop = ACE_OS::gethrtime ();
      const ACE_Time_Value actual_delta =
        ACE_OS::gettimeofday () - actual_start;

      // Store the sample.
      delta_hrtime.sample (ACE_static_cast (ACE_UINT32,
                                            (stop - start) /
                                            ACE_static_cast (ACE_UINT32, 1u)));
      actual_sleeps.sample (actual_delta.msec () * 100u);
    }

  // Calculate the mean value of the samples, with no fractional
  // precision.  Use it for the global scale factor.
  ACE_Stats_Value ticks (0);
  delta_hrtime.mean (ticks);

  ACE_Stats_Value actual_sleep (0);
  actual_sleeps.mean (actual_sleep);

  const ACE_UINT32 scale_factor = ticks.whole () / actual_sleep.whole () /
    10u /* usec/100 usec */;
  ACE_High_Res_Timer::global_scale_factor (scale_factor);

  return scale_factor;
}

void
ACE_High_Res_Timer::dump (void) const
{
  ACE_TRACE ("ACE_High_Res_Timer::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nglobal_scale_factor_: %u\n"),
             global_scale_factor ()));
#if defined (ACE_LACKS_LONGLONG_T)
  ACE_DEBUG ((LM_DEBUG,
             ASYS_TEXT ("start_.hi (): %u; start_.lo (): %u;\n"
                        "end_.hi (): %u; end_.lo (): %u;\n"
                        "total_.hi (): %u; total_.lo (): %u;\n"
                        "start_incr_.hi () %u; start_incr_.lo (): %u\n"),
             start_.hi (), start_.lo (),
             end_.hi (), end_.lo (),
             total_.hi (), total_.lo (),
             start_incr_.hi (), start_incr_.lo ()));
#else  /* ! ACE_LACKS_LONGLONG_T */
  ACE_DEBUG ((LM_DEBUG,
             ASYS_TEXT ("start_: %u; end_: %u; total_: %u; start_incr_: %u\n"),
             start_, end_, total_, start_incr_));
#endif /* ! ACE_LACKS_LONGLONG_T */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_High_Res_Timer::reset (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::reset");

  start_ = 0;
  end_ = 0;
  total_ = 0;
  start_incr_ = 0;
}

void
ACE_High_Res_Timer::elapsed_time (ACE_Time_Value &tv)
{
  hrtime_to_tv (tv, end_ - start_);
}

#if defined (ACE_HAS_POSIX_TIME)
void
ACE_High_Res_Timer::elapsed_time (struct timespec &elapsed_time)
{
  // See elapsed_time (ACE_hrtime_t &nanoseconds) implementation below;
  // this implementation is based on that.

  // Just grab the nanoseconds.  That is, leave off all values above
  // microsecond.  This equation is right!  Don't mess with me!  (It
  // first strips off everything but the portion less than 1 usec.
  // Then it converts that to nanoseconds by dividing by the scale
  // factor to convert to usec, and multiplying by 1000.)  The cast
  // avoids a MSVC 4.1 compiler warning about narrowing.
  u_long nseconds = (u_long) ((this->end_ - this->start_) %
                                global_scale_factor () * 1000u /
                                global_scale_factor ());

  // Get just the microseconds (dropping any left over nanoseconds).
  ACE_UINT32 useconds = (ACE_UINT32) ((this->end_ - this->start_) / global_scale_factor ());

#if ! defined(ACE_HAS_BROKEN_TIMESPEC_MEMBERS)
  elapsed_time.tv_sec = (time_t) (useconds / ACE_ONE_SECOND_IN_USECS);
  // Transforms one second in microseconds into nanoseconds.
  elapsed_time.tv_nsec = (time_t) ((useconds % ACE_ONE_SECOND_IN_USECS) * 1000u + nseconds);
#else
  elapsed_time.ts_sec = (time_t) (useconds / ACE_ONE_SECOND_IN_USECS);
  // Transforms one second in microseconds into nanoseconds.
  elapsed_time.ts_nsec = (time_t) ((useconds % ACE_ONE_SECOND_IN_USECS) * 1000u + nseconds);
#endif /* ACE_HAS_BROKEN_TIMESPEC_MEMBERS */
}
#endif /* ACE_HAS_POSIX_TIME */

void
ACE_High_Res_Timer::elapsed_time_incr (ACE_Time_Value &tv)
{
  hrtime_to_tv (tv, total_);
}

// The whole point is to get nanoseconds.  However, our scale_factor
// gives us microseconds.  We could use floating points to get the
// nanosecond precision if some compilers didn't barf on 64-bit
// divisions with doubles.  So, we just extract out the nanoseconds
// first and then add them to the standard microsecond calculation.
void
ACE_High_Res_Timer::elapsed_time (ACE_hrtime_t &nanoseconds)
{
  // Just grab the nanoseconds.  That is, leave off all values above
  // microsecond.  This equation is right!  Don't mess with me!
  // (It first strips off everything but the portion less than 1 usec.
  // Then it converts that to nanoseconds by dividing by the scale
  // factor to convert to usec, and multiplying by 1000.)
  // The cast avoids a MSVC 4.1 compiler warning about narrowing.
  u_long nseconds = (u_long) ((this->end_ - this->start_) %
                                global_scale_factor () * 1000u /
                                global_scale_factor ());

  // Get just the microseconds (dropping any left over nanoseconds).
  ACE_UINT32 useconds = (ACE_UINT32) ((this->end_ - this->start_) / global_scale_factor ());

  // Total nanoseconds in a single 64-bit value.
  nanoseconds = useconds * 1000u + nseconds;
}

#if !defined (ACE_HAS_WINCE)
void
ACE_High_Res_Timer::print_ave (const char *str, const int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_ave");

  // Get the total number of nanoseconds elapsed.
  ACE_hrtime_t total_nanoseconds;
  this->elapsed_time (total_nanoseconds);

  // Separate to seconds and nanoseconds.
  u_long total_secs  = (u_long) (total_nanoseconds / (ACE_UINT32) ACE_ONE_SECOND_IN_NSECS);
  ACE_UINT32 extra_nsecs = (ACE_UINT32) (total_nanoseconds % (ACE_UINT32) ACE_ONE_SECOND_IN_NSECS);

  char buf[100];
  if (count > 1)
    {
      ACE_hrtime_t avg_nsecs = total_nanoseconds / (ACE_UINT32) count;
      ACE_OS::sprintf (buf, " count = %d, total (secs %lu, usecs %u), avg usecs = %lu\n",
             count, total_secs, (extra_nsecs + 500u) / 1000u,
             (u_long) ((avg_nsecs + 500u) / 1000u));
    }
  else
    ACE_OS::sprintf (buf, " total %3lu.%06lu secs\n",
             total_secs, (extra_nsecs + 500u) / 1000u);

  ACE_OS::write (handle, str, ACE_OS::strlen (str));
  ACE_OS::write (handle, buf, ACE_OS::strlen (buf));
}

void
ACE_High_Res_Timer::print_total (const char *str, const int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_total");

  // Get the total number of nanoseconds elapsed.
  ACE_hrtime_t total_nanoseconds;
  this->elapsed_time (total_nanoseconds);

  // Separate to seconds and nanoseconds.
  u_long total_secs  = (u_long) (total_nanoseconds / (ACE_UINT32) ACE_ONE_SECOND_IN_NSECS);
  ACE_UINT32 extra_nsecs = (ACE_UINT32) (total_nanoseconds % (ACE_UINT32) ACE_ONE_SECOND_IN_NSECS);

  char buf[100];
  if (count > 1)
    {
      ACE_hrtime_t avg_nsecs   = this->total_ / (ACE_UINT32) count;
      ACE_OS::sprintf (buf, " count = %d, total (secs %lu, usecs %u), avg usecs = %lu\n",
             count, total_secs, (extra_nsecs + 500u) / 1000u,
             (u_long) ((avg_nsecs + 500u) / 1000u));
    }
  else
    ACE_OS::sprintf (buf, " total %3lu.%06u secs\n",
             total_secs, (extra_nsecs + 500u) / 1000u);

  ACE_OS::write (handle, str, ACE_OS::strlen (str));
  ACE_OS::write (handle, buf, ACE_OS::strlen (buf));
}
#endif /* !ACE_HAS_WINCE */

int
ACE_High_Res_Timer::get_env_global_scale_factor (const char *env)
{
#if !defined (ACE_HAS_WINCE)
  if (env != 0)
    {
      const char *env_value = ACE_OS::getenv (env);
      if (env_value != 0)
        {
          int value = ACE_OS::atoi (env_value);
          if (value > 0)
            {
              ACE_High_Res_Timer::global_scale_factor (value);
              return 0;
            }
        }
    }
#else
  ACE_UNUSED_ARG (env);
#endif /* !ACE_HAS_WINCE */
  return -1;
}
