// High_Res_Timer.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/High_Res_Timer.h"

#if !defined (__ACE_INLINE__)
#include "ace/High_Res_Timer.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_HI_RES_TIMER)

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
  (void) ACE_OS::memset (&this->temp_, 0, sizeof this->temp_);
}

#if defined (ACE_HAS_POSIX_TIME)
timespec_t &
ACE_High_Res_Timer::elapsed_time (void)
{
  elapsed_time_.tv_sec = (this->end_ - this->start_) / (1000 * 1000 * 1000);
  elapsed_time_.tv_nsec = this->end_ - this->start_ -
                          (elapsed_time_.tv_sec * (1000 * 1000 * 1000));

  return elapsed_time_;
}
#endif /* ACE_HAS_POSIX_TIME */

void 
ACE_High_Res_Timer::print_ave (const char *str, const int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_ave");
#if defined (ACE_HAS_LONGLONG_T)
  hrtime_t total       = this->end_ - this->start_;
  hrtime_t total_secs  = total / (1000 * 1000 * 1000);
  u_long extra_nsecs = total - (total_secs * (1000 * 1000 * 1000));

  char buf[100];
  if (count > 1)
    {
      hrtime_t avg_nsecs = total / count;
      ACE_OS::sprintf (buf, " count = %d, total (secs %lld, usecs %lu), avg usecs = %lld\n",
             count, total_secs, (extra_nsecs + 500) / 1000,
             (avg_nsecs + 500) / 1000);
    }
  else
    ACE_OS::sprintf (buf, " total %3lld.%06lu secs\n",
             total_secs, (extra_nsecs + 500) / 1000);

  ACE_OS::write (handle, str, strlen (str));
  ACE_OS::write (handle, buf, strlen (buf));
#else
# error must have ACE_HAS_LONGLONG_T with ACE_HAS_HI_RES_TIMER
#endif /* ACE_HAS_LONGLONG_T */
}

void 
ACE_High_Res_Timer::print_total (const char *str, const int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_total");
#if defined (ACE_HAS_LONGLONG_T)
  hrtime_t total_secs  = this->total_ / (1000 * 1000 * 1000);
  u_long extra_nsecs = this->total_ - (total_secs * (1000 * 1000 * 1000));

  char buf[100];
  if (count > 1)
    {
      hrtime_t avg_nsecs   = this->total_ / count;
      ACE_OS::sprintf (buf, " count = %d, total (secs %lld, usecs %lu), avg usecs = %lld\n",
             count, total_secs, (extra_nsecs + 500) / 1000,
             (avg_nsecs + 500) / 1000);
    }
  else
    ACE_OS::sprintf (buf, " total %3lld.%06lu secs\n",
             total_secs, (extra_nsecs + 500) / 1000);

  ACE_OS::write (handle, str, strlen (str));
  ACE_OS::write (handle, buf, strlen (buf));
#else
# error must have ACE_HAS_LONGLONG_T with ACE_HAS_HI_RES_TIMER
#endif /* ACE_HAS_LONGLONG_T */
}
#endif /* ACE_HAS_HI_RES_TIMER */
