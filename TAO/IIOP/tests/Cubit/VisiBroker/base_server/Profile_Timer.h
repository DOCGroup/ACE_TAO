/* -*- C++ -*- */
/* An interval timer class using C++. */

#if !defined (ACE_PROFILE_TIMER_H)
#define ACE_PROFILE_TIMER_H

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/procfs.h>
#include <stdio.h>
#include <memory.h>

class Profile_Timer
{
public:
  struct Elapsed_Time
    {
      double real_time;
      double user_time;
      double system_time;
    };

  Profile_Timer (void);
  ~Profile_Timer (void);
  int  start (void);
  int  stop (void);
  int  elapsed_time (Elapsed_Time &et);
  void elapsed_rusage (prusage_t &rusage);
  void get_rusage (prusage_t &rusage);

private:
  void subtract (timestruc_t &tdiff, timestruc_t &t0, timestruc_t &t1);
  void compute_times (Elapsed_Time &et, prusage_t &, prusage_t &);

  prusage_t begin_usage_;
  prusage_t end_usage_;
  prusage_t last_usage_;
  int	    proc_fd_;
};

/* Start timing */

inline int 
Profile_Timer::start (void) 
{
  return ::ioctl (this->proc_fd_, PIOCUSAGE, &this->begin_usage_);
}

/* Stop timing */

inline int 
Profile_Timer::stop (void) 
{
  this->last_usage_ = this->end_usage_;
  return ::ioctl (this->proc_fd_, PIOCUSAGE, &this->end_usage_);
}

#endif /* ACE_PROFILE_TIMER_H */

