// $Id$

#include "Profile_Timer.h"

ACE_RCSID(client, Profile_Timer, "$Id$")

/* Initialize interval timer. */

Profile_Timer::Profile_Timer (void)
{
  char buf[20];
  ::sprintf(buf, "/proc/%d", ::getpid ());

  ::memset (&this->end_usage_, 0, sizeof this->end_usage_);
  ::memset (&this->begin_usage_, 0, sizeof this->begin_usage_);
  ::memset (&this->last_usage_, 0, sizeof this->last_usage_);
  if ((this->proc_fd_ = ::open (buf, O_RDONLY, 0)) == -1)
    ::perror (buf);
}

/* Terminate the interval timer. */
Profile_Timer::~Profile_Timer (void)
{
  if (::close (this->proc_fd_) == -1)
    ::perror ("Profile_Timer::~Profile_Timer");
}

/* Return the resource utilization. */

void
Profile_Timer::get_rusage (prusage_t &rusage)
{
  rusage = this->end_usage_;
}

/* Compute the amount of resource utilization since the start time. */

void
Profile_Timer::elapsed_rusage (prusage_t &rusage)
{
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

/* Compute the elapsed time. */

void
Profile_Timer::compute_times (Elapsed_Time &et, prusage_t &end, prusage_t &begin)
{
  timespec_t td;

  this->subtract (td, end.pr_tstamp, begin.pr_tstamp);
  et.real_time = td.tv_sec + ((double) td.tv_nsec) / (1000 * 1000 * 1000);
  this->subtract (td, end.pr_utime, begin.pr_utime);
  et.user_time = td.tv_sec + ((double) td.tv_nsec) / (1000 * 1000 * 1000);
  this->subtract (td, end.pr_stime,  begin.pr_stime);
  et.system_time = td.tv_sec + ((double) td.tv_nsec) / (1000 * 1000 * 1000);
}

/* Compute the amount of time that has elapsed between start and stop. */

int
Profile_Timer::elapsed_time (Elapsed_Time &et)
{
  this->compute_times (et, this->end_usage_, this->begin_usage_);
  return 0;
}

/* Determine the difference between T1 and T2. */

void
Profile_Timer::subtract (timespec_t &tdiff, timespec_t &t1, timespec_t &t0)
{
  tdiff.tv_sec  = t1.tv_sec - t0.tv_sec;
  tdiff.tv_nsec = t1.tv_nsec - t0.tv_nsec;

  /* Normalize the time. */

  while (tdiff.tv_nsec < 0)
    {
      tdiff.tv_sec--;
      tdiff.tv_nsec += (1000 * 1000 * 1000);
    }
}

#if defined (DEBUG)
#include <stdlib.h>
extern "C" int gettimeofday (timeval *tp);

const int DEFAULT_ITERATIONS = 1000000;

int
main (int argc, char *argv[])
{
  Profile_Timer timer;
  int           iterations = argc > 1 ? atoi (argv[1]) : DEFAULT_ITERATIONS;
  timeval	tv;

  timer.start ();

  for (int i = 0; i < iterations; i++)
    gettimeofday (&tv);

  timer.stop ();

  Profile_Timer::Elapsed_Time et;

  timer.elapsed_time (et);

  printf ("iterations = %d\n", iterations);
  printf ("real time = %f secs, user time = %f secs, system time = %f secs\n",
	  et.real_time, et.user_time, et.system_time);

  printf ("time per call = %f usecs\n", (et.real_time / double (iterations)) * 1000000);
  return 0;
}
#endif /* DEBUG */
