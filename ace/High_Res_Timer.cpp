// $Id$

#define ACE_BUILD_DLL
#include "ace/High_Res_Timer.h"

#if !defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_High_Res_Timer)

// For Intel platforms, a scale factor is required for
// ACE_OS::gethrtime.  We'll still set this to one to prevent division
// by zero errors.
#if defined (ACE_WIN32)

// This is used to find out the Mhz of the machine for the scale factor.  If
// there are any problems getting it, we just return 1 (the default).
static u_long get_registry_scale_factor (void)
{
  HKEY hk; 
  DWORD buf_type = REG_DWORD;
  DWORD buf_len = 10;
  TCHAR *buffer = new TCHAR[10];
  
  LONG rc = RegOpenKeyEx (HKEY_LOCAL_MACHINE, 
                          TEXT("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 
                          NULL, 
                          KEY_READ, 
                          &hk);

  if (rc != ERROR_SUCCESS) 
  {
    // Couldn't find key
    delete buffer;
    return 1; 
  }


  rc = RegQueryValueEx (hk, 
                        TEXT("~MHz"), 
                        0, 
                        &buf_type, 
		        (unsigned char *) buffer, 
                        &buf_len);

  RegCloseKey (hk);

  if (rc != ERROR_SUCCESS) 
  {
    // Couldn't get the value
    delete buffer;
    return 1;
  }

  u_long mhz = (DWORD (*buffer) & 0xFF);
  delete buffer;
  return mhz;
}

u_long ACE_High_Res_Timer::global_scale_factor_ = get_registry_scale_factor ();

#else
// A scale_factor of 1000 converts nanosecond ticks to microseconds.
// That is, on these platforms, 1 tick == 1 nanosecond.
u_long ACE_High_Res_Timer::global_scale_factor_ = 1000;
#endif /* ACE_WIN32 */

void
ACE_High_Res_Timer::dump (void) const
{
  ACE_TRACE ("ACE_High_Res_Timer::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_High_Res_Timer::reset (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::reset");
  (void) ACE_OS::memset (&this->start_, 0, sizeof this->start_);
  (void) ACE_OS::memset (&this->end_, 0, sizeof this->end_);
  (void) ACE_OS::memset (&this->total_, 0, sizeof this->total_);
  (void) ACE_OS::memset (&this->start_incr_, 0, sizeof this->start_incr_);
}

void
ACE_High_Res_Timer::elapsed_time (ACE_Time_Value &tv)
{
  tv.sec ((long) ((this->end_ - this->start_) / global_scale_factor_) / 1000000);
  tv.usec ((long) ((this->end_ - this->start_) / global_scale_factor_) % 1000000);
}

#if defined (ACE_HAS_POSIX_TIME)
void
ACE_High_Res_Timer::elapsed_time (struct timespec &elapsed_time)
{
  // See elapsed_time (ACE_hrtime_t &nanoseconds) implementation below;
  // this implementation is based on that.

  // Just grab the nanoseconds.  That is, leave off all values above
  // microsecond.  This equation is right!  Don't mess with me!
  // The cast avoids a MSVC 4.1 compiler warning about narrowing.
  u_long nseconds = (u_long) ((this->end_ - this->start_) % global_scale_factor_ * 1000 / global_scale_factor_);

  // Get just the microseconds (dropping any left over nanoseconds).
  ACE_hrtime_t useconds; 
  useconds = (this->end_ - this->start_) / global_scale_factor_;

#if ! defined(ACE_HAS_BROKEN_TIMESPEC_MEMBERS)
  elapsed_time.tv_sec = (time_t) (useconds / 1000000);
  elapsed_time.tv_nsec = (time_t) (useconds % 1000000 * 1000  +  nseconds);
#else
  elapsed_time.ts_sec = (time_t) (useconds / 1000000);
  elapsed_time.ts_nsec = (time_t) (useconds % 1000000 * 1000  +  nseconds);
#endif /* ACE_HAS_BROKEN_TIMESPEC_MEMBERS */
}
#endif /* ACE_HAS_POSIX_TIME */

void
ACE_High_Res_Timer::elapsed_time_incr (ACE_Time_Value &tv)
{
  // Leave off any microseconds using DIV.
  tv.sec ((long) (this->total_ / global_scale_factor_) / 1000000);
  // Leave off any seconds using MOD.
  tv.usec ((long) (this->total_ / global_scale_factor_) % 1000000);
}

// The whole point is to get nanoseconds.  However, our scale_factor
// gives us microseconds.  We could use floating points to get the
// nanosecond precision if some compilers didn't barf on 64-bit
// divisions with doubles.  So, we just extract out the nanoseconds
// first and then add them to the standard miscrosecond calculation.
void
ACE_High_Res_Timer::elapsed_time (ACE_hrtime_t &nanoseconds)
{
  // Just grab the nanoseconds.  That is, leave off all values above
  // microsecond.  This equation is right!  Don't mess with me!
  // The cast avoids a MSVC 4.1 compiler warning about narrowing.
  u_long nseconds = (u_long) ((this->end_ - this->start_) % global_scale_factor_ * 1000 / global_scale_factor_);

  // Get just the microseconds (dropping any left over nanoseconds).
  ACE_hrtime_t useconds; 
  useconds = (this->end_ - this->start_) / global_scale_factor_;

  // Total nanoseconds in a single 64-bit value.
  nanoseconds = useconds * 1000 + nseconds;
}


void
ACE_High_Res_Timer::print_ave (const char *str, const int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_ave");

  // Get the total number of nanoseconds elapsed.
  ACE_hrtime_t total_nanoseconds;
  this->elapsed_time (total_nanoseconds);

  // Separate to seconds and nanoseconds.
  ACE_hrtime_t total_secs  = total_nanoseconds / 1000000000;
  u_long extra_nsecs = (u_long) (total_nanoseconds % 1000000000);

  char buf[100];
  if (count > 1)
    {
      ACE_hrtime_t avg_nsecs = total_nanoseconds / count;
      ACE_OS::sprintf (buf, " count = %d, total (secs %lld, usecs %lu), avg usecs = %lld\n",
             count, total_secs, (extra_nsecs + 500) / 1000,
             (avg_nsecs + 500) / 1000);
    }
  else
    ACE_OS::sprintf (buf, " total %3lld.%06lu secs\n",
             total_secs, (extra_nsecs + 500) / 1000);

  ACE_OS::write (handle, str, strlen (str));
  ACE_OS::write (handle, buf, strlen (buf));
}

void
ACE_High_Res_Timer::print_total (const char *str, const int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_total");

  // Get the total number of nanoseconds elapsed.
  ACE_hrtime_t total_nanoseconds;
  this->elapsed_time (total_nanoseconds);

  // Separate to seconds and nanoseconds.
  ACE_hrtime_t total_secs  = total_nanoseconds / 1000000000;
  u_long extra_nsecs = (u_long) (total_nanoseconds % 1000000000);

  char buf[100];
  if (count > 1)
    {
      ACE_hrtime_t avg_nsecs   = this->total_ / count;
      ACE_OS::sprintf (buf, " count = %d, total (secs %lld, usecs %lu), avg usecs = %lld\n",
             count, total_secs, (extra_nsecs + 500) / 1000,
             (avg_nsecs + 500) / 1000);
    }
  else
    ACE_OS::sprintf (buf, " total %3lld.%06lu secs\n",
             total_secs, (extra_nsecs + 500) / 1000);

  ACE_OS::write (handle, str, strlen (str));
  ACE_OS::write (handle, buf, strlen (buf));
}

int
ACE_High_Res_Timer::get_env_global_scale_factor (const char *env)
{
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

  return -1;
}
