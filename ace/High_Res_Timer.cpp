// High_Res_Timer.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/High_Res_Timer.h"

#if !defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_High_Res_Timer)

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
  if (scale_factor_ > 0) {
    tv.sec ((this->end_ - this->start_) / 1000000L / scale_factor_);
    tv.usec ((this->end_ - this->start_) % 1000000L / scale_factor_);
  } else {
    tv.sec ((this->end_ - this->start_) / 1000000L);
    tv.usec ((this->end_ - this->start_) % 1000000L);
  }
}

#if defined (ACE_HAS_POSIX_TIME)
void
ACE_High_Res_Timer::elapsed_time (struct timespec &elapsed_time)
{
  if (scale_factor_ > 0) {
    elapsed_time.tv_sec = (this->end_ - this->start_) / (1000 * 1000 * 1000) /
                          scale_factor_;
    elapsed_time.tv_nsec = (this->end_ - this->start_) % (1000 * 1000 * 1000) /
                          scale_factor_;
  } else {
    elapsed_time.tv_sec = (this->end_ - this->start_) / (1000 * 1000 * 1000);
    elapsed_time.tv_nsec = (this->end_ - this->start_) % (1000 * 1000 * 1000);
  }
}
#endif /* ACE_HAS_POSIX_TIME */

void
ACE_High_Res_Timer::elapsed_time_incr (ACE_Time_Value &tv)
{
  if (scale_factor_ > 0) {
    tv.sec (this->total_ / 1000000L / scale_factor_);
    tv.usec (this->total_ % 1000000L / scale_factor_);
  } else {
    tv.sec (this->total_ / 1000000L);
    tv.usec (this->total_ % 1000000L);
  }
}

void
ACE_High_Res_Timer::print_ave (const char *str, const int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_ave");
  ACE_hrtime_t total;
  if (scale_factor_ > 0)
    total = (this->end_ - this->start_) /scale_factor_;
  else
    total = this->end_ - this->start_;
  ACE_hrtime_t total_secs  = total / (1000 * 1000 * 1000);
  u_long extra_nsecs = total % (1000 * 1000 * 1000);

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
  if (scale_factor_ > 0)
    total_secs = this->total_ / (1000 * 1000 * 1000) / scale_factor_;
  else
    total_secs = this->total_ / (1000 * 1000 * 1000);
  u_long extra_nsecs = this->total_ % (1000 * 1000 * 1000);

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
