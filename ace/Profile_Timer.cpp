// Profile_Timer.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Profile_Timer.h"

#if !defined (__ACE_INLINE__)
#include "ace/Profile_Timer.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Profile_Timer)

#if defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE) 

void
ACE_Profile_Timer::dump (void) const
{
  ACE_TRACE ("ACE_Profile_Timer::dump");
}

// Initialize interval timer. 

ACE_Profile_Timer::ACE_Profile_Timer (void)
{
  ACE_TRACE ("ACE_Profile_Timer::ACE_Profile_Timer");
  ACE_OS::memset (&this->end_usage_, 0, sizeof this->end_usage_);
  ACE_OS::memset (&this->begin_usage_, 0, sizeof this->begin_usage_);
  ACE_OS::memset (&this->last_usage_, 0, sizeof this->last_usage_);

#if defined (ACE_HAS_PRUSAGE_T)
  ACE_OS::memset (&this->last_usage_, 0, sizeof this->last_usage_);
  char buf[20];
  ACE_OS::sprintf (buf, "/proc/%d", ACE_OS::getpid ());

  if ((this->proc_handle_ = ACE_OS::open (buf, O_RDONLY, 0)) == -1)
    ACE_OS::perror (buf);
#elif defined (ACE_HAS_GETRUSAGE)
  ACE_OS::memset (&this->begin_time_, 0, sizeof this->begin_time_);
  ACE_OS::memset (&this->end_time_, 0, sizeof this->end_time_);
  ACE_OS::memset (&this->last_time_, 0, sizeof this->last_time_);
#endif /* ACE_HAS_PRUSAGE_T */
}

// Terminate the interval timer. 
ACE_Profile_Timer::~ACE_Profile_Timer (void)
{
  ACE_TRACE ("ACE_Profile_Timer::~ACE_Profile_Timer");
#if defined (ACE_HAS_PRUSAGE_T)
  if (ACE_OS::close (this->proc_handle_) == -1)
    ACE_OS::perror ("ACE_Profile_Timer::~ACE_Profile_Timer");
#endif /* ACE_HAS_PRUSAGE_T */
}

// Return the resource utilization. 

void
ACE_Profile_Timer::get_rusage (ACE_Profile_Timer::Rusage &usage)
{
  ACE_TRACE ("ACE_Profile_Timer::get_rusage");
  usage = this->end_usage_;
}

#if defined (ACE_HAS_PRUSAGE_T)

// Compute the amount of resource utilization since the start time. 

void
ACE_Profile_Timer::elapsed_rusage (ACE_Profile_Timer::Rusage &rusage)
{
  ACE_TRACE ("ACE_Profile_Timer::elapsed_rusage");
  rusage.pr_lwpid = this->end_usage_.pr_lwpid - this->last_usage_.pr_lwpid;
  rusage.pr_count = this->end_usage_.pr_count - this->last_usage_.pr_count;
  rusage.pr_minf  = this->end_usage_.pr_minf - this->last_usage_.pr_minf;
  rusage.pr_majf  = this->end_usage_.pr_majf - this->last_usage_.pr_majf;
  rusage.pr_inblk = this->end_usage_.pr_inblk - this->last_usage_.pr_inblk;
  rusage.pr_oublk = this->end_usage_.pr_oublk - this->last_usage_.pr_oublk;
  rusage.pr_msnd  = this->end_usage_.pr_msnd - this->last_usage_.pr_msnd;
  rusage.pr_mrcv  = this->end_usage_.pr_mrcv - this->last_usage_.pr_mrcv;
  rusage.pr_sigs  = this->end_usage_.pr_sigs - this->last_usage_.pr_sigs;
  this->subtract (rusage.pr_wtime, this->end_usage_.pr_wtime, this->last_usage_.pr_wtime);
  this->subtract (rusage.pr_ltime, this->end_usage_.pr_ltime, this->last_usage_.pr_ltime);
  this->subtract (rusage.pr_slptime, this->end_usage_.pr_slptime, this->last_usage_.pr_slptime);
  rusage.pr_vctx  = this->end_usage_.pr_vctx - this->last_usage_.pr_vctx;
  rusage.pr_ictx  = this->end_usage_.pr_ictx - this->last_usage_.pr_ictx;
  rusage.pr_sysc  = this->end_usage_.pr_sysc - this->last_usage_.pr_sysc;
  rusage.pr_ioch  = this->end_usage_.pr_ioch - this->last_usage_.pr_ioch;
}

// Compute the elapsed time. 

void
ACE_Profile_Timer::compute_times (ACE_Elapsed_Time &et)
{
  ACE_TRACE ("ACE_Profile_Timer::compute_times");
  timestruc_t td;

  ACE_Profile_Timer::Rusage &end = this->end_usage_;
  ACE_Profile_Timer::Rusage &begin = this->begin_usage_;

  this->subtract (td, end.pr_tstamp, begin.pr_tstamp);
  et.real_time = td.tv_sec + ((double) td.tv_nsec) / (1000 * 1000 * 1000);
  this->subtract (td, end.pr_utime, begin.pr_utime);
  et.user_time = td.tv_sec + ((double) td.tv_nsec) / (1000 * 1000 * 1000);
  this->subtract (td, end.pr_stime,  begin.pr_stime);
  et.system_time = td.tv_sec + ((double) td.tv_nsec) / (1000 * 1000 * 1000);
}

// Determine the difference between T1 and T2. 

void
ACE_Profile_Timer::subtract (timestruc_t &tdiff, timestruc_t &t1, timestruc_t &t0)
{
  ACE_TRACE ("ACE_Profile_Timer::subtract");
  tdiff.tv_sec  = t1.tv_sec - t0.tv_sec;
  tdiff.tv_nsec = t1.tv_nsec - t0.tv_nsec;

  // Normalize the time. 

  while (tdiff.tv_nsec < 0)
    {
      tdiff.tv_sec--;
      tdiff.tv_nsec += (1000 * 1000 * 1000);
    }
}

#elif defined (ACE_HAS_GETRUSAGE)
// Compute the amount of resource utilization since the start time. 

void
ACE_Profile_Timer::elapsed_rusage (ACE_Profile_Timer::Rusage &usage)
{
  ACE_TRACE ("ACE_Profile_Timer::elapsed_rusage");
#if !defined (ACE_WIN32) && !defined (ACE_HAS_LIMITED_RUSAGE_T)
  // integral shared memory size
  usage.ru_ixrss = this->end_usage_.ru_ixrss - this->last_usage_.ru_ixrss;
  // integral unshared data "
  usage.ru_idrss = this->end_usage_.ru_idrss - this->last_usage_.ru_idrss;
  // integral unshared stack "
  usage.ru_isrss = this->end_usage_.ru_isrss - this->last_usage_.ru_isrss;
  // page reclaims - total vmfaults
  usage.ru_minflt = this->end_usage_.ru_minflt - this->last_usage_.ru_minflt;
  // page faults
  usage.ru_majflt = this->end_usage_.ru_majflt - this->last_usage_.ru_majflt;
  // swaps
  usage.ru_nswap = this->end_usage_.ru_nswap - this->last_usage_.ru_nswap;
  // block input operations
  usage.ru_inblock = this->end_usage_.ru_inblock - 
                       this->last_usage_.ru_inblock;
  // block output operations
  usage.ru_oublock = this->end_usage_.ru_oublock - 
                       this->last_usage_.ru_oublock;
  // messages sent
  usage.ru_msgsnd = this->end_usage_.ru_msgsnd - this->last_usage_.ru_msgsnd;
  // messages received
  usage.ru_msgrcv = this->end_usage_.ru_msgrcv - this->last_usage_.ru_msgrcv;
  // signals received
  usage.ru_nsignals = this->end_usage_.ru_nsignals -
                        this->last_usage_.ru_nsignals;
  // voluntary context switches
  usage.ru_nvcsw = this->end_usage_.ru_nvcsw - this->last_usage_.ru_nvcsw;
  // involuntary context switches
  usage.ru_nivcsw = this->end_usage_.ru_nivcsw - this->last_usage_.ru_nivcsw;
#else
	ACE_UNUSED_ARG(usage);
#endif /* ACE_WIN32 */
}

void
ACE_Profile_Timer::compute_times (ACE_Elapsed_Time &et)
{
  ACE_TRACE ("ACE_Profile_Timer::compute_times");

#if defined (ACE_WIN32)
  ACE_Time_Value atv = this->end_time_ - this->begin_time_;
  et.real_time = atv.sec () + ((double) atv.usec ()) / (1000 * 1000);

  atv = ACE_Time_Value (this->end_usage_.ru_utime)
        - ACE_Time_Value (this->begin_usage_.ru_utime);

  et.user_time = atv.sec () + ((double) atv.usec ()) / (1000 * 1000);

  atv = ACE_Time_Value (this->end_usage_.ru_stime)
        - ACE_Time_Value (this->begin_usage_.ru_stime);
  et.system_time = atv.sec () + ((double) atv.usec ()) / (1000 * 1000);
#else
  timeval td;

  this->subtract (td, this->end_time_, this->begin_time_);
  et.real_time = td.tv_sec + ((double) td.tv_usec) / (1000 * 1000);

  this->subtract (td, this->end_usage_.ru_utime, this->begin_usage_.ru_utime);
  et.user_time = td.tv_sec + ((double) td.tv_usec) / (1000 * 1000);

  this->subtract (td, this->end_usage_.ru_stime,  this->begin_usage_.ru_stime);
  et.system_time = td.tv_sec + ((double) td.tv_usec) / (1000 * 1000);
#endif /* ACE_WIN32 */
}

// Determine the difference between T1 and T2. 

void
ACE_Profile_Timer::subtract (timeval &tdiff, timeval &t1, timeval &t0)
{
  ACE_TRACE ("ACE_Profile_Timer::subtract");
  tdiff.tv_sec  = t1.tv_sec - t0.tv_sec;
  tdiff.tv_usec = t1.tv_usec - t0.tv_usec;

  // Normalize the time. 

  while (tdiff.tv_usec < 0)
    {
      tdiff.tv_sec--;
      tdiff.tv_usec += (1000 * 1000);
    }
}

#endif /* ACE_HAS_PRUSAGE_T */

// Compute the amount of time that has elapsed between start and stop. 

int
ACE_Profile_Timer::elapsed_time (ACE_Elapsed_Time &et)
{
  ACE_TRACE ("ACE_Profile_Timer::elapsed_time");
  this->compute_times (et);
  return 0;
}

#else

ACE_Profile_Timer::ACE_Profile_Timer (void)
  : timer_ ()
{
  ACE_TRACE ("ACE_Profile_Timer::ACE_Profile_Timer");
}

int
ACE_Profile_Timer::elapsed_time (ACE_Elapsed_Time &et)
{
  ACE_TRACE ("ACE_Profile_Timer::elapsed_time");

  ACE_hrtime_t delta_t; /* nanoseconds */
  timer_.elapsed_time (delta_t);

#if defined (ACE_HAS_LONGLONG_T)
  et.real_time = delta_t / 1000000000.0;
#else
  et.real_time = (double) ULONG_MAX / 1000000000.0 * (double) delta_t.hi () +
                 (double) delta_t.lo () / 1000000000.0;
#endif /* ACE_HAS_LONGLONG_T */
  et.user_time = 0;
  et.system_time = 0;

  return 0;
}


#endif /* defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE) */
