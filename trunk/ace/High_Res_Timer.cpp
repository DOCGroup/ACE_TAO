// High_Res_Timer.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/High_Res_Timer.h"

#if !defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_High_Res_Timer)

// For win32, a scale factor is required for ACE_OS::gethrtime.  Thus,
// initialize the global_scale_factor to zero.  This will prevent use
// unless someone explicitly sets it.
#if defined (ACE_WIN32)
double ACE_High_Res_Timer::global_scale_factor_ = 0;
#else
// For platforms that do not require a scale factor, we'll set it to
// one so that the divisions are no ops.
double ACE_High_Res_Timer::global_scale_factor_ = 1;
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
  tv.sec ((long) ((this->end_ - this->start_) / scale_factor_ / 1000) / 1000000);
  tv.usec ((long) ((this->end_ - this->start_) / scale_factor_ / 1000) % 1000000);
}

#if defined (ACE_HAS_POSIX_TIME)
void
ACE_High_Res_Timer::elapsed_time (struct timespec &elapsed_time)
{
  elapsed_time.tv_sec = (time_t) ((this->end_ - this->start_)  / scale_factor_ / 1000) / 1000000;
  elapsed_time.tv_nsec = (long) ((this->end_ - this->start_) / scale_factor_ / 1000) % 1000000;
}
#endif /* ACE_HAS_POSIX_TIME */

void
ACE_High_Res_Timer::elapsed_time_incr (ACE_Time_Value &tv)
{
  tv.sec ((long) (this->total_ / scale_factor_ / 1000) / 1000000);
  tv.usec ((long) (this->total_ / scale_factor_ / 1000) % 1000000);
}

void
ACE_High_Res_Timer::print_ave (const char *str, const int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_ave");
  ACE_hrtime_t total;
  total = (ACE_hrtime_t) ((this->end_ - this->start_) / scale_factor_);
  ACE_hrtime_t total_secs  = total / 1000000000;
  u_long extra_nsecs = (u_long) (total % 1000000000);

  char buf[100];
  if (count > 1)
    {
      ACE_hrtime_t avg_nsecs = total / count;
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
  ACE_hrtime_t total_secs;
  total_secs = (ACE_hrtime_t) (this->total_ / scale_factor_ / 1000000000);
  u_long extra_nsecs = (u_long) (this->total_ % 1000000000);

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
