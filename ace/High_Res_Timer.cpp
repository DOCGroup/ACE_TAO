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

ACE_High_Res_Timer::ACE_High_Res_Timer (void)
{
  ACE_TRACE ("ACE_High_Res_Timer::ACE_High_Res_Timer");
  this->reset ();
}

void 
ACE_High_Res_Timer::print_ave (char *str, int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_ave");
#if defined (ACE_HAS_LONGLONG_T)
  hrtime_t total       = this->end_ - this->start_;
  hrtime_t avg_nsecs   = total / count;
  hrtime_t total_secs  = total / (1000 * 1000 * 1000);
  hrtime_t extra_nsecs = total - (total_secs * (1000 * 1000 * 1000));
  char	   buf[100];

  ACE_OS::sprintf (buf, "%s count = %d, total (secs %lld, usecs %lld), avg usecs = %lld\n",
             str, count, total_secs, extra_nsecs / 1000, avg_nsecs / 1000);
  ACE_OS::write (handle, buf, strlen (buf));
#endif /* ACE_HAS_LONGLONG_T */
}

void 
ACE_High_Res_Timer::print_total (char *str, int count, ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_High_Res_Timer::print_total");
#if defined (ACE_HAS_LONGLONG_T)
  hrtime_t avg_nsecs   = this->total_ / count;
  hrtime_t total_secs  = this->total_ / (1000 * 1000 * 1000);
  hrtime_t extra_nsecs = this->total_ - (total_secs * (1000 * 1000 * 1000));
  char buf[100];

  ACE_OS::sprintf (buf, "%s count = %d, total (secs %lld, usecs %lld), avg usecs = %lld\n",
             str, count, total_secs, extra_nsecs / 1000, avg_nsecs / 1000);
  ACE_OS::write (handle, buf, strlen (buf));
#endif /* ACE_HAS_LONGLONG_T */
}
#endif /* ACE_HAS_HI_RES_TIMER */
