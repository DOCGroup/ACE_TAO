/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Profile_Timer.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_PROFILE_TIMER_H)
#define ACE_PROFILE_TIMER_H

#include "ace/ACE.h"
#include "ace/Time_Value.h"

#if !(defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE))

class ACE_Export ACE_Profile_Timer
{
public:
  struct ACE_Elapsed_Time
    {
      double real_time;
      double user_time;
      double system_time;
    };

  ACE_Profile_Timer (void) {}
  ~ACE_Profile_Timer (void) {}
  int  start (void) { errno = ENOTSUP; return -1; }
  int  stop (void) { errno = ENOTSUP; return -1; }
  int  elapsed_time (ACE_Elapsed_Time &et) { errno = ENOTSUP; return -1; }
};
#else
class ACE_Export ACE_Profile_Timer
  // = TITLE
  //     A C++ wrapper for UNIX interval timers.
{
public:
  struct ACE_Elapsed_Time
    {
      double real_time;
      double user_time;
      double system_time;
    };

#if defined (ACE_HAS_PRUSAGE_T)
    typedef prusage_t Rusage;
#elif defined (ACE_HAS_GETRUSAGE)
    typedef rusage Rusage;
#endif /* ACE_HAS_PRUSAGE_T */

  // = Initialization and termination methods.
  ACE_Profile_Timer (void);
  // Default constructor.
  
  ~ACE_Profile_Timer (void);
  // Shutdown the timer.

  // = Timer methods.
  int start (void);
  // Activate the timer.

  int stop (void);
  // Stop the timer.

  // = Resource utilization methods.
  int elapsed_time (ACE_Elapsed_Time &et);
  // Compute the time elapsed since <start>.

  void elapsed_rusage (ACE_Profile_Timer::Rusage &rusage);
  // Compute the amount of resource utilization since the start time. 
  
  void get_rusage (ACE_Profile_Timer::Rusage &rusage);
  // Return the resource utilization (don't recompute it).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  void compute_times (ACE_Elapsed_Time &et);
  // Compute how much time has elapsed.

  ACE_Profile_Timer::Rusage begin_usage_;
  // Keep track of the starting resource utilization.

  ACE_Profile_Timer::Rusage end_usage_;
  // Keep track of the ending resource utilization.

  ACE_Profile_Timer::Rusage last_usage_;
  // Keep track of the last rusage for incremental timing.

#if defined (ACE_HAS_PRUSAGE_T)
  void subtract (timestruc_t &tdiff, timestruc_t &t0, timestruc_t &t1);
  // Substract two timestructs and store their difference.

  ACE_HANDLE proc_handle_;
  // I/O handle for /proc file system.

#elif defined (ACE_HAS_GETRUSAGE)
  void subtract (timeval &tdiff, timeval &t0, timeval &t1);
  // Substract two timestructs and store their difference.

  timeval begin_time_;
  // Keep track of the beginning time.

  timeval end_time_;
  // Keep track of the ending time.

  timeval last_time_;
  // Keep track of the last time for incremental timing.
#elif defined (ACE_WIN32)
  ACE_Time_Value begin_time_;
  // Keep track of the beginning time.

  ACE_Time_Value end_time_;
  // Keep track of the ending time.

  ACE_Time_Value last_time_;
  // Keep track of the last time for incremental timing.

#endif /* ACE_HAS_PRUSAGE_T */
};

#if defined (__ACE_INLINE__)
#include "ace/Profile_Timer.i"
#endif /* __ACE_INLINE__ */

#endif /* defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE) */
#endif /* ACE_PROFILE_TIMER_H */

