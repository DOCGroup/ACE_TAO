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

#ifndef ACE_PROFILE_TIMER_H
#define ACE_PROFILE_TIMER_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"
#include "ace/High_Res_Timer.h"

class ACE_Export ACE_Profile_Timer
{
  // = TITLE
  //     This class provides both a timing mechanism and a mechanism
  //     for reporting the resource usage of a process.
public:

  class ACE_Elapsed_Time
  {
    // = TITLE
    //    Keeps track of the various user, system, and elapsed (real)
    //    times.
    //
    // = DESCRIPTION
    //   If <ACE_HAS_FLOATING_POINT> is enabled these values are in
    //   microseconds, otherwise, they are in seconds.
  public:
    ACE_timer_t real_time;
    ACE_timer_t user_time;
    ACE_timer_t system_time;
  };

  typedef ACE_Rusage Rusage;

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
  void subtract (timespec_t &tdiff, timespec_t &t0, timespec_t &t1);
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
#else
  ACE_High_Res_Timer timer_;
#endif /* ACE_HAS_PRUSAGE_T */
};

#if defined (__ACE_INLINE__)
# include "ace/Profile_Timer.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_PROFILE_TIMER_H */

